#include "calc_thread.h"

void CalcThread::run()
{

    using ratio_model_t = ratio_model<ratio_susp_data>;
    ratio_model_t rm(200000, 20);

    ratio_susp_data sp_data;
    for (size_t i = 0; i < 1000; ++i)
    {
        rm.init_suspension(sp_data);

        rm.calculate_direct(sp_data);
        rm.calculate_reverse(sp_data);
        rm.calculate_coil(sp_data);
        const auto& susp = rm.get_susp();

        if (sp_data.coil_out.T < 100 || sp_data.coil_out.T > 160) continue;

        float a   = susp.get_a_m() * 10.0f;
        float b   = susp.get_a_m() * 10.0f;
        float h   = susp.get_l_m() * 10.0f;
        float l   = susp.get_l_h() * 10.0f;
        float c   = susp.get_b_h() * 10.0f;
        float b_x = susp.get_b_x() * 10.0f;
        float h_p = susp.get_h_p() * 10.0f;
        float gap = susp.get_air_gap() * 10.0f;

        QThread::msleep(80);
        std::cout << a << " " 
            << b << " " 
            << h << " "
            << l << " "
            << c << " "
            << b_x << " "
            << h_p << " "
            << gap << " " << std::endl;

        const tsizes sz = {a, b, h, l, c, b_x, gap, h_p};
        emit resultReady(sz);
    }
}
