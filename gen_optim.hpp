#ifndef GEN_OPTIM_HPP
#define GEN_OPTIM_HPP

#include <future>
#include <chrono>
#include <string_view>
#include <fstream>
#include "ratio_mag_model.hpp"
#include "consts.hpp"
#include "utils.hpp"
#include "fit_func.hpp"


namespace
{
    auto cr(const auto val1, const auto val2)
    {
        return (val1 + val2 + std::min(val1, val2))/3;
    }
}

///
/// Run through the optimization flow by using genetic algorithm.
///
template<typename FitFuncT = PowerFit>
class GenOptimizer
{
    public:
        GenOptimizer() : mFitFunc() {}
        ~GenOptimizer() = default;

    private:
        void populateWithRandom()
        {
            mPopulation.clear();
            mPopulation.reserve(mInitCount);

            const auto populate = [this]() {
                ratio_model_t rm(200000, 20);
                while (true)
                {
                    auto sp_data = std::make_unique<ratio_susp_data>();
                    rm.runAll(*sp_data);

                    if (!passes(*sp_data)) continue;

                    {
                        std::lock_guard<std::mutex> guard(mMutex);
                        if (mPopulation.size() >= mInitCount) return;
                        // the individum is fine add to population
                        mPopulation.push_back(std::move(sp_data));
                    }
                }
            };

            const auto hardNum = std::thread::hardware_concurrency();
            {
                std::vector<std::future<void>> f {hardNum};
                for(size_t i = 0; i < hardNum; ++i)
                  f[i] = std::async(std::launch::async, populate);
            }

            // we need to sort by power
            std::sort(mPopulation.begin(), mPopulation.end(), mFitFunc);
        }

        auto doCrossing()
        {
            const auto endIt = (mPopulation.size() > (mCrossPortion + 2)) ?
                std::next(mPopulation.begin(), mCrossPortion) :
                mPopulation.end();

            // Lambda for crossing operation.
            auto crossOp = [](const auto& in1, const auto& in2)
            {
                auto sp = std::make_unique<ratio_susp_data>();

                sp->cpack.k_e = cr(in1->cpack.k_e.value(), in2->cpack.k_e.value());
                sp->cpack.B_air = cr(in1->cpack.B_air.value(), in2->cpack.B_air.value());
                sp->cpack.k_m = cr(in1->cpack.k_m.value(), in2->cpack.k_m.value());
                sp->cpack.k_mm = cr(in1->cpack.k_mm.value(), in2->cpack.k_mm.value());
                sp->cpack.k_x = cr(in1->cpack.k_x.value(), in2->cpack.k_x.value());
                sp->cpack.k_h = cr(in1->cpack.k_h.value(), in2->cpack.k_h.value());
                sp->cpack.k_delta = cr(in1->cpack.k_delta.value(), in2->cpack.k_delta.value());
                sp->cpack.k_p = cr(in1->cpack.k_p.value(), in2->cpack.k_p.value());

                return sp;
            };

            std::vector<std::shared_ptr<ratio_susp_data>> cPopulation;
            for (auto it = mPopulation.begin(); it != endIt; ++it)
            {
                for(auto it1 = std::next(it); it1 != endIt; ++it1)
                {
                    auto crossed = crossOp(*it, *it1);
                    cPopulation.push_back(std::move(crossed));
                }
            }

            std::for_each(std::begin(cPopulation), std::end(cPopulation),
                    [this](auto& sp_data) { doMutation(*sp_data); });

            DEBUGP();

            mPopulation.erase(mPopulation.begin(), endIt);

            return cPopulation;
        }

        void doMutation(auto& sp_data, const size_t optToMutate = 2)
        {
            static const std::vector<const frange*> optionsRanges = {&range_k_e,
                &range_B_air,
                &range_k_m,
                &range_k_mm,
                &range_k_x,
                &range_k_h,
                &range_k_delta,
                &range_k_p};

            std::vector<std::optional<float>*> options = {&sp_data.cpack.k_e,
                &sp_data.cpack.B_air,
                &sp_data.cpack.k_m,
                &sp_data.cpack.k_mm,
                &sp_data.cpack.k_x,
                &sp_data.cpack.k_h,
                &sp_data.cpack.k_delta,
                &sp_data.cpack.k_p};

            //choose random option to mutate
            static std::random_device rd = {};
            static auto mtgen = std::mt19937{rd()};

            auto oldRandomVal = -1;
            for(size_t i = 0; i < optToMutate; ++i)
            {
                const auto newRandomVal = std::uniform_int_distribution<>{0, 7}(mtgen);
                if (newRandomVal == oldRandomVal) continue;

                oldRandomVal = newRandomVal;
                // mutate some value
                *options[oldRandomVal] = random_from(*optionsRanges[oldRandomVal]);
            }
        }

        void updatePopulation(auto& crossedPopulation)
        {
            if (!crossedPopulation.size()) return;

            ratio_model_t rm(200000, 20);
            std::shared_ptr<ratio_susp_data> minData = nullptr;

            //TODO: rework this part
            auto it = std::remove_if(std::begin(crossedPopulation),
                    std::end(crossedPopulation),
                    [&fitFunc = mFitFunc, &rm, &minData](const auto& sp_data) {
                        rm.runAll(*sp_data);

                        if (!passes(*sp_data)) return true;

                        minData = (minData == nullptr)?
                        sp_data : std::min(minData, sp_data, fitFunc);

                        return false;
                    });

            crossedPopulation.erase(it, std::end(crossedPopulation));

            DEBUGM("alived remained %zu\n", crossedPopulation.size());

            if (!crossedPopulation.size()) return;

            mIterationMin.push_back(minData);
           
            TPopContainer temp;
            temp.reserve(crossedPopulation.size() + mPopulation.size());

            std::merge(std::begin(mPopulation), std::end(mPopulation),
                    std::begin(crossedPopulation), std::end(crossedPopulation),
                    std::back_inserter<TPopContainer>(temp), mFitFunc);

            mPopulation = std::move(temp);
        }

        void finishOptimization()
        {
            std::shared_ptr<ratio_susp_data> minVal = mIterationMin[0];
            std::for_each(std::begin(mIterationMin), std::end(mIterationMin),
                    [&fitFunc = mFitFunc, &minVal](const auto& val)
                    { minVal = std::min(val, minVal, fitFunc); });
           
            printf("Final P = %f\n", minVal->coil_out.P);
            printf("Final price = %f$\n", minVal->other.price);
            printf("B_air = %f\n", minVal->cpack.B_air.value());
            printf("k_e = %f\n", minVal->cpack.k_e.value());
            printf("k_m = %f\n", minVal->cpack.k_m.value());
            printf("k_mm = %f\n", minVal->cpack.k_mm.value());
            printf("k_x = %f\n", minVal->cpack.k_x.value());
            printf("k_h = %f\n", minVal->cpack.k_h.value());
            printf("k_delta = %f\n", minVal->cpack.k_delta.value());
            printf("k_p = %f\n", minVal->cpack.k_p.value());
//            const std::string fileToAppend = "/home/narekc/tmp/susp_data";
//            printf("Appended to the %s\n", fileToAppend.c_str()); 
//
//            appendToFile(fileToAppend, *minVal);
        }

    public:
        void createInitialPopulation(TSharedDataVec&& inVec = {})
        { 
            if (inVec.size() == 0) //TODO: make run and add to pop parallel
            {
                populateWithRandom();
                return;
            }

            // The case when we have init population to calculate.
            mPopulation.clear();
            mPopulation = std::move(inVec);

            ratio_model_t rm(200000, 20);
            auto DoCalc = [rm](auto& sp_data) mutable { rm.runAll(*sp_data); };

            // Now do calculation parallel
            calcRange(std::begin(mPopulation), std::end(mPopulation),
                    DoCalc, 8);

            // we need to sort by power
            std::sort(mPopulation.begin(), mPopulation.end(), mFitFunc);
        }

        void runOptimization(size_t inCount = 1000)
        {
            // create initial population
            mIterationMin.push_back(mPopulation[0]);

            // create crossed population
            auto initP = mPopulation[0]->coil_out.P;
            auto initPrice = mPopulation[0]->other.price;
            for(size_t i = 0; i < inCount; ++i)
            {
                printf("\r");
                printf("%zu/%zu", i, inCount);
                fflush(stdout);

                using namespace std::literals;
                auto cPopulation = doCrossing();

                if (!mPopulation.size() && !cPopulation.size())
                    break;

                // fit new population to the existing one
                updatePopulation(cPopulation);
            }

            printf("\n");
            printf("Init P = %f\n", initP);
            printf("Init price = %f$\n", initPrice);
            finishOptimization();

        }

    private:
        static const inline auto passes = [](const auto& dt) 
        { return dt.coil_out.T >= 130 && dt.coil_out.T <= 160; };

    private:
        /// Fitness function.
        FitFuncT mFitFunc;

    private:
        using TPopContainer = std::vector<std::shared_ptr<ratio_susp_data>>;
        const size_t mCrossPortion = 15;
        const size_t mInitCount = 100;
        TPopContainer mPopulation;
        TPopContainer mIterationMin;
        std::mutex mMutex;
};

#endif // GEN_OPTIM_HPP
