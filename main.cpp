
#include "ratio_mag_model.hpp"
#include "simple_mag_model.hpp"

#include <string>

int main()
{
#if 0
    ratio_model rm(200000, 20);

    for (size_t i = 0; i < 1000; ++i)
    {
        rm.init_suspension();
        const auto precision = 0.01f;
        const auto F = rm.calculate_direct(precision);
        const auto& [B, FF] = rm.calculate_reverse(F, precision);
        //std::cout << "B = " << B << " F = " << FF << std::endl;
        const auto& [temp, len] = rm.calculate_coil(FF, 220.0f, 0.95f, 160.0f);

        const auto price = rm.calculate_price(len);


        if (temp < 130 || temp > 160) continue;

        std::cout<< std::string(30, '-') << std::endl;
        cof_pack cpack = rm.get_cofs();

        std::cout << "Input" << std::endl;
        print_range(ratio_model::range_k_e, cpack.k_e, "Kէ");
        print_range(ratio_model::range_B_air, cpack.B_air, "Bδ");
        print_range(ratio_model::range_k_m, cpack.k_m, "Kմ");
        print_range(ratio_model::range_k_mm, cpack.k_mm, "Kմմ");
        print_range(ratio_model::range_k_x, cpack.k_x, "Kխ");
        print_range(ratio_model::range_k_h, cpack.k_h, "Kհ");
        print_range(ratio_model::range_k_delta, cpack.k_delta, "KΔ");
        print_range(ratio_model::range_k_p, cpack.k_p, "Kփ");

        std::cout << "\nOutput" << std::endl;
        std::cout << "F = " << F << "Ա" << std::endl;
        std::cout << "θփ = " << temp << "°C" << std::endl;
        std::cout << "Գին = " << price << "$" << std::endl;
        std::cout << "P = " << rm.m_Power << "Վտ" << std::endl;
    }

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
    susp_out_data susp_data;
    sm.calculate_direct(susp_data.m_direct_data, precision);

    const auto& F_opt = get_F(susp_data.m_direct_data);
    const auto& F_val = F_opt.has_value() ? F_opt.value(): 0.0f;

    std::cout << "F = " << F_val << std::endl;
    std::cout << "Contures = " << susp_data.m_direct_data.size() << std::endl;

    sm.calculate_reverse(susp_data, precision);
    const auto B = susp_data.m_reverse_B;
    const auto& F_opt_rev = get_F(susp_data.m_reverse_data);
    const auto F_val_rev = F_opt_rev.has_value() ? F_opt_rev.value() : 0.0f;
    std::cout.precision(3);
    std::cout << std::fixed << "B = " << B << " F = " << F_val_rev << std::endl;

    susp_data.coil_in.U = 220.0f;
    susp_data.coil_in.k_fill = 0.95f;
    susp_data.coil_in.T_allow = 160.0f;
    sm.calculate_coil(susp_data);

    std::cout << "T = " << susp_data.coil_out.T << std::endl;
    std::cout << "L = " << susp_data.coil_out.L_wire << std::endl;
    std::cout << "P = " << susp_data.coil_out.P << std::endl;

    std::cout << "Price = " << sm.calculate_price(susp_data.coil_out.L_wire) << std::endl;

#endif
}
