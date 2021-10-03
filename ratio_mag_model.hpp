#ifndef RATIO_MAG_MODEL
#define RATIO_MAG_MODEL

#include <iostream>
#include <utility>
#include <cmath>
#include <random>
#include "mag_model.hpp"

#define show_size

// Function to return random value in the given range.
float random_from(std::pair<float,float> in_range, const int precision = 100)
{
    static std::random_device rd = {};
    static auto mtgen = std::mt19937{rd()};
    auto md = std::uniform_int_distribution<>{0, precision};
    auto random_val = in_range.first + (in_range.second - in_range.first) / (float)precision * (float)md(mtgen);

    std::cout << in_range.first <<" -- "<< in_range.second <<" : " << random_val << std::endl;
    return random_val;
}

// need to represent the cof values for every suspension
typedef struct 
{
    float k_e = 0.0f;
    float k_m = 0.0f;
    float k_mm = 0.0f;
    float k_x = 0.0f;
    float k_h = 0.0f;
    float k_delta = 0.0f;
    float k_p = 0.0f;
} cof_pack;

// This model used to initialize the magnetic suspension via size coeff.
class ratio_model : public mag_model
{
    public:
        ratio_model(float P_vagon, float sus_count) 
            : P_v(P_vagon)
              , n(sus_count) {}
        ~ratio_model() = default;

    public:
        virtual void init_suspension() override
        {
            cof_pack cpack;
            cpack.k_e = random_from(range_k_e);

            m_susp = std::make_unique<mag_suspension>();
            m_susp->set_P_e(P_v*cpack.k_e/n);

            m_susp->set_B_air(random_from(range_B_air));
            auto S_m = myu_0*m_susp->get_P_e()/std::pow(m_susp->get_B_air(), 2); // of a single polus, so divided by 2
            
            cpack.k_m = random_from(range_k_m);
            m_susp->set_a_m(sqrt(S_m/cpack.k_m));
            m_susp->set_b_m(S_m/m_susp->get_a_m());

            cpack.k_mm = random_from(range_k_mm);
            m_susp->set_l_m(cpack.k_mm*m_susp->get_a_m());

            cpack.k_x = random_from(range_k_x);
            auto S_x = cpack.k_x*S_m;
            m_susp->set_a_x(m_susp->get_a_m());
            m_susp->set_b_x(S_x/m_susp->get_a_x());

            cpack.k_h = random_from(range_k_h);
            auto S_h = cpack.k_h*S_m;
            m_susp->set_b_h(S_h/m_susp->get_b_m());

            cpack.k_delta = random_from(range_k_delta);
            m_susp->set_Delta_p(cpack.k_delta*m_susp->get_l_m());
            m_susp->set_Delta_m(0.005f);
            auto h_p = m_susp->get_l_m()-m_susp->get_b_h()-m_susp->get_Delta_m()-m_susp->get_Delta_p();
            m_susp->set_h_p(h_p);
            cpack.k_p = random_from(range_k_p);
            m_susp->set_l_p(m_susp->get_h_p()*cpack.k_p);
            m_susp->set_l_h(m_susp->get_l_p()+2*(m_susp->get_b_m()+m_susp->get_Delta_m()));
            m_susp->set_l_x(m_susp->get_l_h());

            m_susp->set_air_gap(0.01f);

            //finally set the steel curve to be used for calculations
            m_curve = std::make_unique<Curve_BH>("./steel_bh/steel_10.txt");


#ifdef show_size
            std::cout << "m_susp->get_P_e() = " << m_susp->get_P_e() << std::endl;
            std::cout << "m_susp->get_a_m() = " << m_susp->get_a_m() << std::endl;
            std::cout << "m_susp->get_b_m() = " << m_susp->get_b_m() << std::endl;
            std::cout << "m_susp->get_l_m() = " << m_susp->get_l_m() << std::endl;
            std::cout << "m_susp->get_b_h() = " << m_susp->get_b_h() << std::endl;
            std::cout << "m_susp->get_l_h() = " << m_susp->get_l_h() << std::endl;
            std::cout << "m_susp->get_a_x() = " << m_susp->get_a_x() << std::endl;
            std::cout << "m_susp->get_b_x() = " << m_susp->get_b_x() << std::endl;
            std::cout << "m_susp->get_l_x() = " << m_susp->get_l_x() << std::endl;
            std::cout << "m_susp->get_air_gap() = " << m_susp->get_air_gap() << std::endl;
            std::cout << "m_susp->get_Delta_p() = " << m_susp->get_Delta_p() << std::endl; 
            std::cout << "m_susp->get_Delta_m() = " << m_susp->get_Delta_m() << std::endl; 
            std::cout << "m_susp->get_l_p() = " << m_susp->get_l_p() << std::endl;
            std::cout << "m_susp->get_h_p() = " << m_susp->get_h_p() << std::endl;
#endif
        }

    private:
        float P_v = 0.0f;
        float n = 0.0f;

        using frange = std::pair<float,float>;

        frange range_k_e = {1.0f, 1.5f};
        frange range_k_m = {1.0f, 1.0f};
        frange range_k_mm = {2.0f, 3.0f};
        frange range_k_x = {0.5f, 1.0f};
        frange range_k_h = {1.0f, 1.2f};
        frange range_k_delta = {0.05f, 0.1f};
        frange range_k_p = {1.0f, 1.1f};
        frange range_B_air = {1.0f, 1.2f};
};
#endif //RATIO_MAG_MODEL
