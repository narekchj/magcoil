#ifndef MAG_MODEL
#define MAG_MODEL

#include <memory>
#include "mag_suspension.hpp"
#include "curve.hpp"
#include "data_helper.hpp"

typedef struct
{
	float Fi_m = 0.0f;
	float Fi_s = 0.0f;
	float U_in = 0.0f;
	float U_out = 0.0f;
	float mag_B = 0.0f;
	float mag_H = 0.0f;
	float U_gap = 0.0f;
	float U_base = 0.0f;
	float mag_B_base = 0.0f;
	float mag_H_base = 0.0f;
} circle;

typedef struct
{
    float a_m;
    float b_m;
    float l_m;
    float b_h;
    float l_h;
    float b_x;
    float P_e;
    float air_gap; 
    float B_air;
    float Delta_p; 
    float Delta_m; 
} susp_base_sizes;

class mag_model
{
    protected:
        void init_suspension_base(const susp_base_sizes sz)
        {
            if (!m_susp) return;

            m_susp->set_a_m(sz.a_m);
            m_susp->set_b_m(sz.b_m);
            m_susp->set_l_m(sz.l_m);
            m_susp->set_b_h(sz.b_h);
            m_susp->set_l_h(sz.l_h);
            m_susp->set_b_x(sz.b_x);
            m_susp->set_a_x(sz.a_m);
            m_susp->set_l_x(sz.l_h);
            m_susp->set_Delta_p(sz.Delta_p);
            m_susp->set_Delta_m(sz.Delta_m);
            m_susp->set_B_air(sz.B_air);
            m_susp->set_air_gap(sz.air_gap);
            m_susp->set_P_e(sz.P_e);

            //calculate rest stuff
            const auto h_p = m_susp->get_l_m() - m_susp->get_b_h() - m_susp->get_Delta_m() - m_susp->get_Delta_p();
            m_susp->set_h_p(h_p);

            const auto l_p = m_susp->get_l_h() - 2 * (m_susp->get_b_m() + m_susp->get_Delta_m());
            m_susp->set_l_p(l_p);
        }

    public:
        virtual void init_suspension() = 0;

        virtual bool is_initialized() const 
        {
            if (!m_susp) return false;

            if (!m_curve) return false;

            return true;
        }

        virtual float calculate_direct(const float precision)
        {
            if (!is_initialized()) return false;

            m_data.clear();
            size_t l_contures = 0;

            while(true)
            {
                // creates data for this calculation
                c_data_t c_data = {};
                auto begin_dt = std::make_unique<circle>();

                const auto S_m = m_susp->get_a_m() * m_susp->get_b_m(); 
                begin_dt->Fi_s = m_susp->get_B_air() * S_m;
                const auto la_gap = myu_0 * S_m / m_susp->get_air_gap();
                begin_dt->U_gap = begin_dt->Fi_s / la_gap;
                const auto S_x = m_susp->get_a_x() * m_susp->get_b_x();
                begin_dt->mag_B = begin_dt->Fi_s / S_x;
                begin_dt->mag_H = m_curve->get_H_from_B(begin_dt->mag_B);
                begin_dt->U_in = begin_dt->mag_H * (m_susp->get_l_x() - m_susp->get_b_m());
                begin_dt->U_out = begin_dt->U_in + 2 * begin_dt->U_gap;
                // add to the data container
                c_data.push_back(std::move(begin_dt));

                const auto contures = std::max(l_contures, static_cast<size_t>(1));
                const auto height = m_susp->get_l_m() - m_susp->get_b_h();
                const auto la_m = myu_0 * ((m_susp->get_l_m() - m_susp->get_b_h()) * m_susp->get_a_m() / contures)
                    /(m_susp->get_l_h() - 2 * m_susp->get_b_m());


                for (size_t i = 1; i < l_contures + 1; ++i) // calculating contures stuff
                {
                    // create new contures data
                    auto dt = std::make_unique<circle>();
                    dt->Fi_m = c_data[i-1]->U_out * la_m;
                    dt->Fi_s = c_data[i-1]->Fi_s + c_data[i-1]->Fi_m;
                    dt->mag_B = dt->Fi_s / S_m;
                    dt->mag_H = m_curve->get_H_from_B(dt->mag_B);
                    dt->U_in = dt->mag_H * height / contures;
                    dt->U_out = c_data[i - 1]->U_out + 2 * dt->U_in;
                    // push data to the list
                    c_data.push_back(std::move(dt));
                }

                // finally count base data
                auto end_dt = std::make_unique<circle>();
                end_dt->Fi_s = c_data.back()->Fi_s + c_data.back()->Fi_m;
                end_dt->mag_B = end_dt->Fi_s / S_m;
                end_dt->mag_H = m_curve->get_H_from_B(end_dt->mag_B);
                end_dt->U_in = (l_contures == 0) ? end_dt->mag_H * height / contures : 0;

                // calculate base bulk stuff
                const auto S_h = m_susp->get_a_m() * m_susp->get_b_h();
                end_dt->mag_B_base = end_dt->Fi_s / S_h;
                end_dt->mag_H_base = m_curve->get_value_X(end_dt->mag_B_base);
                end_dt->U_base = end_dt->mag_H_base * (m_susp->get_l_h() - m_susp->get_b_m());

                // magnetic force
                end_dt->U_out = c_data.back()->U_out + end_dt->U_base + 2 * end_dt->U_in;

                // push data to the list
                c_data.push_back(std::move(end_dt));

                // calculate diff of the previous 2 calculations
                const auto prev_U_out = (m_data.size()) ? m_data.back().back()->U_out : c_data.back()->U_out;
                const auto diff = (c_data.back()->U_out - prev_U_out) / prev_U_out * 100.0f;

                m_data.push_back(std::move(c_data));

                if (diff && diff <= precision) // we don't need calculation anymore
                    break;

                ++l_contures;
            }

    //        std::cout << "Contures = " << l_contures << std::endl;

            // return the F_mshu
            return m_data.back().back()->U_out;
        }

        std::pair<float,float> calculate_reverse(const float F_mshu, const float precision)
        {
            auto B_value = 0.01f;
            const auto B_step = 0.01f;
            auto diff_F = 0.0f;
            auto res_F = 0.0f;
            for (; ((diff_F == 0 || diff_F > 1.0f) && B_value < 2.0f);
                    B_value += B_step)
            {
                m_susp->set_B_air(B_value);
                res_F = calculate_direct(precision);
                diff_F = (F_mshu - res_F) / F_mshu * 100;
            }

            return {B_value, res_F};
        }

        float calculate_coil(float F_mshu, float U, float k_fill, float allow_temp)
        {
             const auto dt_c = 1000 * m_susp->get_Delta_m();
             //std::cout <<"isolation = "<< dt_c << std::endl;

             // get sizes
             const auto a_p = 1000 * m_susp->get_a_m() + 2 * dt_c; //mm 
             //std::cout << "a_p = " << a_p << std::endl;

             const auto b_p = 1000 * m_susp->get_b_h() + 2 * dt_c; //mm
             //std::cout << "b_p = " << b_p << std::endl;

             const auto h_p = 1000 * m_susp->get_h_p(); //mm
             //std::cout << "h_p = " << h_p << std::endl;

             const auto l_m = 2 * (a_p + b_p + 2 * h_p) / 1000; //m
             //std::cout << "l_mm = " << l_m << std::endl;

             const auto RO20 = 0.0175f;
             const auto ROt = RO20 * (1.0f + 0.0039f * (allow_temp - 20.0f)); // allow temp is 160
             //std::cout << "R0t = " << ROt << std::endl;

             auto diam = sqrt((4 * F_mshu * ROt * l_m) / (PI * U));
             //std::cout << "diam is = " << diam << std::endl;

             data_helper::db_pair val = data_helper::get_inst().get_wire_diam(diam);
             diam = val.first;
             //std::cout << "diam is = " << diam << std::endl;

             // here we need get near diameter and area
             const auto sgm_isol = data_helper::get_inst().get_isolation(diam);
             //std::cout << "isol = " << sgm_isol << std::endl;

             const auto l_p = 1000 * m_susp->get_l_p(); //mm
             //std::cout << "l_p = " << l_p << std::endl;

             const auto w_coil = (4 * k_fill * l_p * h_p) / (PI * pow((diam + sgm_isol), 2.0));
             //std::cout << "coil turns = " << w_coil << std::endl;

             const auto S_wire = val.second;
             //std::cout << "s_wire = " << S_wire << std::endl;

             const auto r_coil = ROt * (l_m * w_coil) / S_wire;
             //std::cout << "r_coil = " << r_coil << std::endl;

             const auto I = U / r_coil;
             //std::cout << "I = " << I << std::endl;
             const auto power = pow(I, 2) * r_coil;	
             //std::cout << "P = " << power << std::endl;
             
             [[maybe_unused]]const auto j = I / S_wire;
             //std::cout << "j = " << j << std::endl;

             const auto A_p = a_p + 2 * h_p; //mm
             const auto B_p = b_p + 2 * h_p; //mm
             const auto S_cool = 2 * (A_p + B_p) * l_p;
             //std::cout << "S_cool = " << S_cool << std::endl;
             const auto heat_k = data_helper::get_inst().get_heat_cf(160).second;
             //std::cout << "heat_k = " << heat_k << std::endl;
             const auto temp = power / (heat_k * S_cool);

             return temp;
        }

    protected:
        using c_data_t = std::vector<std::unique_ptr<circle>>;
        std::vector<c_data_t> m_data; // TODO: change to the data layers to be passed to the model.
        std::unique_ptr<mag_suspension > m_susp;
        std::unique_ptr<Curve_BH> m_curve;
};

#endif //MAG_MODEL
