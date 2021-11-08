#ifndef GEN_OPTIM_HPP
#define GEN_OPTIM_HPP

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
            ratio_model_t rm(200000, 20);
            mPopulation.clear();

            while (mPopulation.size() < 10)
            {
                std::unique_ptr<ratio_susp_data> sp_data = std::make_unique<ratio_susp_data>();
                rm.init_suspension(*sp_data);
                rm.calculate_direct(*sp_data);
                rm.calculate_reverse(*sp_data);
                rm.calculate_coil(*sp_data);
                if (!passes(*sp_data)) continue;

                // the individum is fine add to population
                mPopulation.push_back(std::move(sp_data));
            }

            // we need to sort by power
            std::sort(mPopulation.begin(), mPopulation.end(),
                    [](const auto& ld, const auto& rd)
                    {return (*ld).coil_out.P < (*rd).coil_out.P;});

//            std::transform(mPopulation.begin(), mPopulation.end(),
//                    std::ostream_iterator<ratio_susp_data>(std::cout, "\n"),
//                    [](const auto& dt) {return *dt;});
        }


    private:
        void createInitPopulation()
        {

        }

    private:
        static const inline auto passes = [](const auto& dt) 
        { return dt.coil_out.T >= 130 && dt.coil_out.T <= 160; };

    private:
        std::vector<std::unique_ptr<ratio_susp_data>> mPopulation;
};

#endif // GEN_OPTIM_HPP
