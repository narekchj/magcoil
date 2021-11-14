#ifndef GEN_OPTIM_HPP
#define GEN_OPTIM_HPP
#include <QObject>

#include <future>
#include "ratio_mag_model.hpp"
#include "consts.hpp"

namespace
{
    std::ostream& operator<<(std::ostream& s, const ratio_susp_data& dt)
    {
        s << dt.coil_out.P;
        return s;
    }
}

///
/// Run through the optimization flow by using genetic algorithm.
///
class gen_optimizer : public QObject
{
    Q_OBJECT

    using ratio_model_t = ratio_model<ratio_susp_data>;

    public:
        gen_optimizer() = default;
        ~gen_optimizer() = default;

    private:
        void createInitialPopulation()
        {
            mPopulation.clear();
            mPopulation.reserve(mInitCount);

            const auto populate = [this]() {
                ratio_model_t rm(200000, 20);
                while (true)
                {
                    auto sp_data = std::make_unique<ratio_susp_data>();
                    rm.init_suspension(*sp_data);
                    rm.calculate_direct(*sp_data);
                    rm.calculate_reverse(*sp_data);
                    rm.calculate_coil(*sp_data);

                    if (!passes(*sp_data)) continue;

                    emit resultReady(getSizes(rm));

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
                {
                  f[i] = std::async(std::launch::async, populate);
                }
            }

            // we need to sort by power
            std::sort(mPopulation.begin(), mPopulation.end(),fit);
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

                sp->cpack.k_e = (in1->cpack.k_e.value() + in2->cpack.k_e.value()) / 2.0f;
                sp->cpack.B_air = (in1->cpack.B_air.value() + in2->cpack.B_air.value()) / 2.0f;
                sp->cpack.k_m = (in1->cpack.k_m.value() + in2->cpack.k_m.value()) / 2.0f;
                sp->cpack.k_mm = (in1->cpack.k_mm.value() + in2->cpack.k_mm.value()) / 2.0f;
                sp->cpack.k_x = (in1->cpack.k_x.value() + in2->cpack.k_x.value()) / 2.0f;
                sp->cpack.k_h = (in1->cpack.k_h.value() + in2->cpack.k_h.value()) / 2.0f;
                sp->cpack.k_delta = (in1->cpack.k_delta.value() + in2->cpack.k_delta.value()) / 2.0f;
                sp->cpack.k_p = (in1->cpack.k_p.value() + in2->cpack.k_p.value()) / 2.0f;

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

            mPopulation.erase(mPopulation.begin(), endIt);

            return cPopulation;
        }

        void doMutation(auto& sp_data, size_t optToMutate = 1)
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
            ratio_model_t rm(200000, 20);

            auto minData = crossedPopulation[0];
            for(size_t i = 0; i < mMaxMutation; ++i)
            {
                auto it = std::remove_if(std::begin(crossedPopulation), std::end(crossedPopulation),
                        [this, &rm, &minData](const auto& sp_data) {
                            rm.init_suspension(*sp_data);
                            rm.calculate_direct(*sp_data);
                            rm.calculate_reverse(*sp_data);
                            rm.calculate_coil(*sp_data);

                            emit resultReady(getSizes(rm));
                            if (!passes(*sp_data)) return true;

                            minData = std::min(minData, sp_data, fit);
                            return false;
                        });

                crossedPopulation.erase(it, std::end(crossedPopulation));

                const bool bdec = (std::min(minData, mIterationMin.back(), fit) == minData);
                if (bdec || !crossedPopulation.size()) break;
              
                minData = crossedPopulation[0];
                // if not decreasing then mutation
                std::for_each(std::begin(crossedPopulation), std::end(crossedPopulation),
                        [this](auto& sp_data) {this->doMutation(*sp_data, 3);});
            }
                
            mIterationMin.push_back(minData);
           
            TPopContainer temp;
            temp.reserve(crossedPopulation.size() + mPopulation.size());

            std::merge(std::begin(mPopulation), std::end(mPopulation),
                    std::begin(crossedPopulation), std::end(crossedPopulation),
                    std::back_inserter<TPopContainer>(temp), fit);

            mPopulation = std::move(temp);
        }

        void finishOptimization()
        {
            std::cout << "final results\n";
            std::shared_ptr<ratio_susp_data> minVal = mIterationMin[0];
            std::for_each(std::begin(mIterationMin), std::end(mIterationMin),
                    [&minVal](const auto& val)
                    { minVal = std::min(val, minVal, fit); });

            ratio_model_t rm(200000, 20);
            rm.init_suspension(*minVal);
            emit finalResult(getSizes(rm));
        }

    private:
        static const inline auto passes = [](const auto& dt) 
        { return dt.coil_out.T >= 130 && dt.coil_out.T <= 160; };

        static const inline auto fit = [](const auto& ld, const auto& rd)
        { return (*ld).coil_out.P < (*rd).coil_out.P; };


    private:
        using TPopContainer = std::vector<std::shared_ptr<ratio_susp_data>>;
        const size_t mMaxMutation = 5;
        const size_t mCrossPortion = 10;
        const size_t mInitCount = 100;
        TPopContainer mPopulation;
        TPopContainer mIterationMin;
        std::mutex mMutex;
    public:
        void runOptimization(size_t inCount = 20)
        {
            // create initial population
            createInitialPopulation();
            mIterationMin.push_back(mPopulation[0]);


            // create crossed population
            std::cout << "Init P is " << mPopulation[0]->coil_out.P << std::endl;
            for(size_t i = 0; i < inCount; ++i)
            {
                auto cPopulation = doCrossing();

                if (!cPopulation.size()) break;

                // fit new population to the existing one
                updatePopulation(cPopulation);
            }


            finishOptimization();
        }
    signals:
        void resultReady(const tsizes&);
        void finalResult(const tsizes&);
};

#endif // GEN_OPTIM_HPP
