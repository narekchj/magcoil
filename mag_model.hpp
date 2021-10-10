#ifndef MAG_MODEL
#define MAG_MODEL

#include <memory>
#include "mag_suspension.hpp"
#include "curve.hpp"
#include "data_helper.hpp"
#include "susp_data.hpp"


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

        virtual void calculate_direct(direct_data_t& data, const float precision)
        {
            if (!is_initialized()) return;

            data.clear();
            size_t l_contures = 0;

            while(l_contures < max_contures)
            {
                // creates data for this calculation
                susp_out_data::circle_t c_data = {};
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
                    / (m_susp->get_l_h() - 2 * m_susp->get_b_m());

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
                const auto prev_U_out = (data.size()) ? data.back().back()->U_out : c_data.back()->U_out;
                const auto diff = (c_data.back()->U_out - prev_U_out) / prev_U_out * 100.0f;

                data.push_back(std::move(c_data));

                if (diff && diff <= precision) // we don't need calculation anymore
                    break;

                ++l_contures;
            }
        }

        void calculate_reverse(susp_out_data& data, const float precision)
        {
            // TODO: change this algorithm
            const auto B_step = 0.01f;
            // we here then we have valid value
            const auto F_mshu = get_F(data.m_direct_data).value();
            
            for (auto diff_F = 0.0f, B_value = 0.01f;
                    ((diff_F == 0 || diff_F > 1.0f) && B_value < 2.0f);
                    B_value += B_step)
            {
                data.m_reverse_B = B_value;
                m_susp->set_B_air(B_value);
                calculate_direct(data.m_reverse_data, precision);
               
                const auto F_opt = get_F(data.m_reverse_data);
                const auto res_F = F_opt.has_value() ? F_opt.value() : 0.0f;
                diff_F = (F_mshu - res_F) / F_mshu * 100;
            }
        }

        void calculate_coil(susp_out_data& data)
        {
             const auto dt_c = 1000 * m_susp->get_Delta_m();

             const auto a_p = 1000 * m_susp->get_a_m() + 2 * dt_c; //mm 
             data.coil_out.a_p = a_p;

             const auto b_p = 1000 * m_susp->get_b_h() + 2 * dt_c; //mm
             data.coil_out.b_p = b_p;

             const auto h_p = 1000 * m_susp->get_h_p(); //mm

             const auto l_m = 2 * (a_p + b_p + 2 * h_p) / 1000; //m
             data.coil_out.l_m = l_m;

             const auto Rot = Ro20 * (1.0f + 0.0039f * (data.coil_in.T_allow - 20.0f)); 
             data.coil_out.Rot = Rot;

             const auto F_mshu = get_F(data.m_reverse_data).value();
             auto diam = sqrt((4.0f * F_mshu * Rot * l_m) / (PI * data.coil_in.U));
             data_helper::db_pair val = data_helper::get_inst().get_wire_diam(diam);
             diam = val.first;
             data.coil_out.D_wire = diam;

             // here we need get near diameter and area
             const auto sgm_isol = data_helper::get_inst().get_isolation(diam);
             data.coil_out.isol = sgm_isol;

             const auto l_p = 1000 * m_susp->get_l_p(); //mm

             const auto w_coil = (4 * data.coil_in.k_fill * l_p * h_p) / (PI * pow((diam + sgm_isol), 2.0));
             data.coil_out.W = w_coil;

             const auto S_wire = val.second;
             data.coil_out.S_wire = S_wire;

             const auto r_coil = Rot * (l_m * w_coil) / S_wire;
             data.coil_out.R = r_coil;

             const auto I = data.coil_in.U / r_coil;
             data.coil_out.I = I;

             const auto power = pow(I, 2) * r_coil;	
             data.coil_out.P = power;
             
             const auto j = I / S_wire;
             data.coil_out.j = j;

             const auto A_p = a_p + 2 * h_p; //mm
             data.coil_out.A_p = A_p;

             const auto B_p = b_p + 2 * h_p; //mm
             data.coil_out.B_p = B_p;

             const auto S_cool = 2 * (A_p + B_p) * l_p;
             data.coil_out.S_cool = S_cool;

             const auto heat_k = data_helper::get_inst().get_heat_cf(data.coil_in.T_allow).second;
             const auto temp = power / (heat_k * S_cool);
             data.coil_out.T = temp;

             const auto wire_length = l_p * (std::pow(A_p, 2) - std::pow(a_p, 2))
                 / 4 * std::pow(diam, 2);
             data.coil_out.L_wire = wire_length;
        }

        virtual float calculate_price(const float wire_length) const // TODO
        {
            const auto weight = wire_length / 1000000.0f * 10.0f; //TODO: add the table of the psdkt weight

            const auto usd_per_kg = 10.0f;

            const auto wire_price = usd_per_kg * weight;
            const auto coil_price = wire_price + 0.01f * wire_price; //TODO: 1 percent for isolation

            //calculate bulk price
            const auto bulk_vol = 2 * m_susp->get_a_m() * m_susp->get_b_m() * m_susp->get_l_m()
                + (m_susp->get_l_h() - 2 * m_susp->get_b_m()) * m_susp->get_a_m() * m_susp->get_b_h();

            const auto Ro_steel_10 = 7856.0f;

            const auto bulk_weight = bulk_vol * Ro_steel_10;
//            printf("bulk weight %f kg\n", bulk_weight);

            const auto usd_per_steel_kg = 0.7f;

            const auto bulk_price = bulk_weight * usd_per_steel_kg;

            // price of the suspension
            return bulk_price + coil_price;
        }

    protected:
        std::unique_ptr<mag_suspension > m_susp; // TODO: review this as well
        std::unique_ptr<Curve_BH> m_curve;
};

#endif //MAG_MODEL
