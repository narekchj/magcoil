#ifndef RATIO_MAG_MODEL
#define RATIO_MAG_MODEL

#include <iostream>
#include <utility>
#include <cmath>
#include <random>
#include "mag_model.hpp"

//#define show_size

namespace 
{
    // Function to return random value in the given range.
    float random_from(const std::pair<float,float>& in_range, const int precision = 100)
    {
        static std::random_device rd = {};
        static auto mtgen = std::mt19937{rd()};
        auto md = std::uniform_int_distribution<>{0, precision};
        auto random_val = in_range.first + (in_range.second - in_range.first) / (float)precision * (float)md(mtgen);

        return random_val;
    }

    using frange = std::pair<float,float>;

    static constexpr frange range_k_e = {1.0f, 1.1f};
    static constexpr frange range_k_m = {1.0f, 1.1f};
    static constexpr frange range_k_mm = {1.5f, 2.0f};
    static constexpr frange range_k_x = {0.6f, 0.8f};
    static constexpr frange range_k_h = {1.0f, 1.1f};
    static constexpr frange range_k_delta = {0.02f, 0.05f};
    static constexpr frange range_k_p = {1.2f, 2.0f};
    static constexpr frange range_B_air = {0.9f, 1.2f};
}

///
/// Extended data type to store also coff values.
///
struct ratio_susp_data : public susp_data
{
    // need to represent the cof values for every suspension
    struct 
    {
        std::optional<float> k_e;
        std::optional<float> B_air;
        std::optional<float> k_m;
        std::optional<float> k_mm;
        std::optional<float> k_x;
        std::optional<float> k_h;
        std::optional<float> k_delta;
        std::optional<float> k_p;
    } cpack;

};

// This model used to initialize the magnetic suspension via size coeff.
template <typename T>
class ratio_model : public mag_model<T>
{
    using base_class = mag_model<T>;

    public:
        ratio_model(float P_vagon, float sus_count) 
            : P_v(P_vagon)
            , n(sus_count) {}
        ~ratio_model() = default;

    public:
        void runAll(T& data) override 
        {
            init_suspension(data);
            base_class::calculate_direct(data);
            base_class::calculate_reverse(data);
            base_class::calculate_coil(data);

            // finally calculate the price
            calculate_price(data, *base_class::m_susp);
        }

        void init_suspension(T& data) override
        {
            if (!data.cpack.k_e) data.cpack.k_e = random_from(range_k_e);

            auto susp = std::make_unique<mag_suspension>(); //TODO: put somewhere else
            auto P_e = P_v*data.cpack.k_e.value()/n;

            if (!data.cpack.B_air) data.cpack.B_air = random_from(range_B_air);
            data.dir_in.B = data.cpack.B_air.value();
            auto S_m = myu_0 * P_e / std::pow(data.dir_in.B, 2); // of a single polus, so divided by 2
            
            if (!data.cpack.k_m) data.cpack.k_m = random_from(range_k_m);
            susp->set_a_m(sqrt(S_m/data.cpack.k_m.value()));
            susp->set_b_m(S_m/susp->get_a_m());

            if (!data.cpack.k_mm) data.cpack.k_mm = random_from(range_k_mm);
            susp->set_l_m(data.cpack.k_mm.value()*susp->get_a_m());

            if (!data.cpack.k_x) data.cpack.k_x = random_from(range_k_x);
            auto S_x = data.cpack.k_x.value()*S_m;
            susp->set_a_x(susp->get_a_m());
            susp->set_b_x(S_x/susp->get_a_x());

            if (!data.cpack.k_h) data.cpack.k_h = random_from(range_k_h);
            auto S_h = data.cpack.k_h.value()*S_m;
            susp->set_b_h(S_h/susp->get_b_m());

            if (!data.cpack.k_delta) data.cpack.k_delta = random_from(range_k_delta);
            susp->set_Delta_p(data.cpack.k_delta.value()*susp->get_l_m());
            susp->set_Delta_m(0.005f);
            auto h_p = susp->get_l_m()-susp->get_b_h()-susp->get_Delta_m()-susp->get_Delta_p();
            susp->set_h_p(h_p);
            if (!data.cpack.k_p) data.cpack.k_p = random_from(range_k_p);
            susp->set_l_p(susp->get_h_p()*data.cpack.k_p.value());
            susp->set_l_h(susp->get_l_p()+2*(susp->get_b_m()+susp->get_Delta_m()));
            susp->set_l_x(susp->get_l_h());

            susp->set_air_gap(0.01f);

            data.coil_in.U = 220.0f;
            data.coil_in.k_fill = 0.95f;
            data.coil_in.T_allow = 160.0f;

            // initiated suspension for the model
            base_class::m_susp = std::move(susp);
        }

    private:
        float P_v = 0.0f;
        float n = 0.0f;

};
#endif //RATIO_MAG_MODEL
