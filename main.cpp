
#include "ratio_mag_model.hpp"
#include "simple_mag_model.hpp"

int main()
{
#if 1
    ratio_model rm(200000, 20);

    for (size_t i = 0; i < 1; ++i)
    {
        rm.init_suspension();
        const auto precision = 0.01f;
        const auto F = rm.calculate_direct(precision);
        std::cout << "Direct calculation is " << F << std::endl;
        const auto& [B, FF] = rm.calculate_reverse(F, precision);
        std::cout << "B = " << B << " F = " << FF << std::endl;
        const auto temp = rm.calculate_coil(FF, 220.0f, 0.95f, 160.0f);
        std::cout << "Temp = " << temp << std::endl;
    }

    std::cin.get();

#else
    susp_base_sizes sz;
    //sizes
    sz.a_m = 0.1f;
    sz.b_m = 0.1f;
    sz.l_m = 0.2f;
    sz.l_h = 0.3f;
    sz.b_h = 0.1f;
    sz.b_x = 0.05f;
    sz.Delta_p = 0.02f;
    sz.Delta_m = 0.005f;
    // forces
    sz.P_e = 8500;
    sz.air_gap = 0.01f;
    const auto S_m = sz.a_m * sz.b_m;
    sz.B_air = std::sqrt(myu_0 * sz.P_e / S_m);

    simple_model sm(sz);

    const auto precision = 0.01f;
    const auto F = sm.calculate_direct(precision);
    std::cout << "Direct calculation is " << F << std::endl;

    const auto& [B, FF] = sm.calculate_reverse(F, precision);
    std::cout << std::fixed << "B = " << B << " F = " << FF << std::endl;

    const auto temp = sm.calculate_coil(FF, 220.0f, 0.95f, 160.0f);
    std::cout << "Temp = " << temp << std::endl;
#endif
}
