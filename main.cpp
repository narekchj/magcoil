
#include "ratio_mag_model.hpp"
#include "simple_mag_model.hpp"

#include <string>

int main()
{
#if 1
    ratio_model rm(200000, 20);

    for (size_t i = 0; i < 1000; ++i)
    {
        susp_data sp_data;
        rm.init_suspension(sp_data);

        rm.calculate_direct(sp_data);
        rm.calculate_reverse(sp_data);
        rm.calculate_coil(sp_data);

        if (sp_data.coil_out.T < 130 || sp_data.coil_out.T > 160) continue;

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
        std::cout << "F = " << get_F(sp_data.dir_out.data).value() << "Ա" << std::endl;
        std::cout << "θփ = " << sp_data.coil_out.T << "°C" << std::endl;
        std::cout << "Գին = " << calculate_price(sp_data) << "$" << std::endl;
        std::cout << "P = " << sp_data.coil_out.P << "Վտ" << std::endl;
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

    susp_data sp_data;
    sp_data.dir_in.precision = 0.01f;
    sp_data.rev_in.precision = 0.01f;

    // Make direct calculation.
    sm.calculate_direct(sp_data);

    const auto& F_opt = get_F(sp_data.dir_out.data);
    const auto& F_val = F_opt.has_value() ? F_opt.value(): 0.0f;

    std::cout << "F = " << F_val << std::endl;
    std::cout << "Contures = " << sp_data.dir_out.data.size() << std::endl;

    sm.calculate_reverse(sp_data);
    const auto B = sp_data.rev_out.B;
    const auto& F_opt_rev = get_F(sp_data.rev_out.data);
    const auto F_val_rev = F_opt_rev.has_value() ? F_opt_rev.value() : 0.0f;
    std::cout.precision(3);
    std::cout << std::fixed << "B = " << B << " F = " << F_val_rev << std::endl;

    sp_data.coil_in.U = 220.0f;
    sp_data.coil_in.k_fill = 0.95f;
    sp_data.coil_in.T_allow = 160.0f;
    sm.calculate_coil(sp_data);

    std::cout << "T = " << sp_data.coil_out.T << std::endl;
    std::cout << "L = " << sp_data.coil_out.L_wire << std::endl;
    std::cout << "P = " << sp_data.coil_out.P << std::endl;

    std::cout << "Price = " << sm.calculate_price(sp_data.coil_out.L_wire) << std::endl;

#endif
}
