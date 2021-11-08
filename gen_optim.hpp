#ifndef GEN_OPTIM_HPP
#define GEN_OPTIM_HPP
#include <future>
#include "ratio_mag_model.hpp"

std::ostream& operator<<(std::ostream& s, const ratio_susp_data& dt)
{
    s << dt.coil_out.P;
    return s;
}

///
/// Run through the optimization flow by using genetic algorithm.
///
class gen_optimizer
{
    using ratio_model_t = ratio_model<ratio_susp_data>;

    public:
        gen_optimizer() = default;
        ~gen_optimizer() = default;

        void createInitialPopulation()
        {
            mPopulation.clear();
            mPopulation.reserve(mInitCount);

            const auto hardNum = std::thread::hardware_concurrency();
            {
                std::vector<std::future<void>> f {hardNum};
                for(auto i = 0u; i < hardNum; ++i)
                {
                  f[i] = std::async(std::launch::async,
                          &gen_optimizer::populate, this);
                }
            }

            // we need to sort by power
            std::sort(mPopulation.begin(), mPopulation.end(),
                    [](const auto& ld, const auto& rd)
                    {return (*ld).coil_out.P < (*rd).coil_out.P;});
        }

        void doCrossing()
        {


        }

    private:
        void populate()
        {
            ratio_model_t rm(200000, 20);
            while (true)
            {
                auto sp_data = std::make_unique<ratio_susp_data>();
                rm.init_suspension(*sp_data);
                rm.calculate_direct(*sp_data);
                rm.calculate_reverse(*sp_data);
                rm.calculate_coil(*sp_data);

                if (!passes(*sp_data)) continue;

                {
                    std::lock_guard<std::mutex> guard(mMutex);
                    if (mPopulation.size() >= mInitCount) return;
                    // the individum is fine add to population
                    mPopulation.push_back(std::move(sp_data));
                }
            }
        }

    private:
        static const inline auto passes = [](const auto& dt) 
        { return dt.coil_out.T >= 130 && dt.coil_out.T <= 160; };

    private:
        const size_t mInitCount = 100;
        std::vector<std::unique_ptr<ratio_susp_data>> mPopulation;
        std::mutex mMutex;
};

#endif // GEN_OPTIM_HPP
