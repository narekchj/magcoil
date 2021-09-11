#include <iostream>
#include <utility>
#include <cmath>
#include "mag_suspension.hpp"

#define show_size
inline const float PI = 3.14f;
inline const float myu_0 = 4 * PI * 1E-7;

class mag_model
{
    public:
        virtual void init_suspension() = 0;
        virtual bool calculate() = 0;
};

// Function to return random value in the given range.
float random_from(std::pair<float,float> in_range)
{
    //TODO:
    return (in_range.first + in_range.second)/2;
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
    float B_air = 0.0f;
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

            mag_suspension susp;
            susp.set_P_e(P_v*cpack.k_e/n);

            cpack.B_air = random_from(range_B_air);
            auto S_m = myu_0*susp.get_P_e()/(cpack.B_air*cpack.B_air); // of a single polus, so divided by 2
            
            cpack.k_m = random_from(range_k_m);
            susp.set_a_m(sqrt(S_m/cpack.k_m));
            susp.set_b_m(S_m/susp.get_a_m());

            cpack.k_mm = random_from(range_k_mm);
            susp.set_l_m(cpack.k_mm*susp.get_a_m());

            cpack.k_x = random_from(range_k_x);
            auto S_x = cpack.k_x*S_m;
            susp.set_a_x(susp.get_a_m());
            susp.set_b_x(S_x/susp.get_a_x());

            cpack.k_h = random_from(range_k_h);
            auto S_h = cpack.k_h*S_m;
            susp.set_b_h(S_h/susp.get_b_m());

            cpack.k_delta = random_from(range_k_delta);
            susp.set_Delta_p(cpack.k_delta*susp.get_l_m());
            susp.set_Delta_m(0.005f);
            auto h_p = susp.get_l_m()-susp.get_b_h()-susp.get_Delta_m()-susp.get_Delta_p();
            susp.set_h_p(h_p);
            cpack.k_p = random_from(range_k_p);
            susp.set_l_p(susp.get_h_p()*cpack.k_p);
            susp.set_l_h(susp.get_l_p()+2*(susp.get_b_m()+susp.get_Delta_m()));
            susp.set_l_x(susp.get_l_h());

            susp.set_air_gap(0.01f);


#ifdef show_size
            std::cout << "susp.get_P_e() = " << susp.get_P_e() << std::endl;
            std::cout << "susp.get_a_m() = " << susp.get_a_m() << std::endl;
            std::cout << "susp.get_b_m() = " << susp.get_b_m() << std::endl;
            std::cout << "susp.get_l_m() = " << susp.get_l_m() << std::endl;
            std::cout << "susp.get_b_h() = " << susp.get_b_h() << std::endl;
            std::cout << "susp.get_l_h() = " << susp.get_l_h() << std::endl;
            std::cout << "susp.get_a_x() = " << susp.get_a_x() << std::endl;
            std::cout << "susp.get_b_x() = " << susp.get_b_x() << std::endl;
            std::cout << "susp.get_l_x() = " << susp.get_l_x() << std::endl;
            std::cout << "susp.get_air_gap() = " << susp.get_air_gap() << std::endl;
            std::cout << "susp.get_Delta_p() = " << susp.get_Delta_p() << std::endl; 
            std::cout << "susp.get_Delta_m() = " << susp.get_Delta_m() << std::endl; 
            std::cout << "susp.get_l_p() = " << susp.get_l_p() << std::endl;
            std::cout << "susp.get_h_p() = " << susp.get_h_p() << std::endl;
#endif
        }

        virtual bool calculate() override
        {

            return false;
        }

    private:
        float P_v = 0.0f;
        float n = 0.0f;

        using frange = std::pair<float,float>;

        frange range_k_e = {1.2f, 1.5f};
        frange range_k_m = {1.0f, 2.0f};
        frange range_k_mm = {2.0f, 3.0f};
        frange range_k_x = {0.6f, 1.0f};
        frange range_k_h = {1.0f, 1.2f};
        frange range_k_delta = {0.05f, 0.1f};
        frange range_k_p = {2.0f, 3.0f};
        frange range_B_air = {0.6f, 1.0f};
};

