#ifndef MAG_MODEL
#define MAG_MODEL

#include <memory>
#include "mag_suspension.hpp"
#include "curve.hpp"



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

class mag_model
{
    public:
        virtual void init_suspension() = 0;

        virtual bool is_initialized() const 
        {
            if (!m_susp) return false;

            if (!m_curve) return false;

            return true;
        }

        virtual bool calculate()
        {
            if (!is_initialized()) return false;

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
            m_data.push_back(std::move(begin_dt));

            const auto contures = std::max(m_contures, static_cast<size_t>(1));
            const auto height = m_susp->get_l_m() - m_susp->get_b_h();
            const auto la_m = myu_0 * ((m_susp->get_l_m() - m_susp->get_b_h()) * m_susp->get_a_m() / contures)
                /(m_susp->get_l_h() - 2 * m_susp->get_b_m());


            for (size_t i = 1; i < m_contures + 1; ++i) // calculating contures stuff
            {
                // create new contures data
                auto dt = std::make_unique<circle>();
                dt->Fi_m = m_data[i-1]->U_out * la_m;
                dt->Fi_s = m_data[i-1]->Fi_s + m_data[i-1]->Fi_m;
                dt->mag_B = dt->Fi_s / S_m;
                dt->mag_H = m_curve->get_H_from_B(dt->mag_B);
                dt->U_in = dt->mag_H * height / contures;
                dt->U_out = m_data[i - 1]->U_out + 2 * dt->U_in;
                // push data to the list
                m_data.push_back(std::move(dt));
            }

            // finally count base data
            auto end_dt = std::make_unique<circle>();
            end_dt->Fi_s = m_data.back()->Fi_s + m_data.back()->Fi_m;
            end_dt->mag_B = end_dt->Fi_s / S_m;
            end_dt->mag_H = m_curve->get_H_from_B(end_dt->mag_B);

            if (m_contures == 0)
            {
                end_dt->U_in = end_dt->mag_H * height / contures;
            }
            else
                end_dt->U_in = 0;

            // calculate base bulk stuff
            const auto S_h = m_susp->get_a_m() * m_susp->get_b_h();
            end_dt->mag_B_base = end_dt->Fi_s / S_h;
            end_dt->mag_H_base = m_curve->get_value_X(end_dt->mag_B_base);
            end_dt->U_base = end_dt->mag_H_base * (m_susp->get_l_h() - m_susp->get_b_m());

            // magnetic force
            end_dt->U_out = m_data.back()->U_out + end_dt->U_base + 2 * end_dt->U_in;

            // push data to the list
            m_data.push_back(std::move(end_dt));
            return false;
        }

    private:
        size_t m_contures = 50;

    protected:
        std::vector<std::unique_ptr<circle>> m_data;
        std::unique_ptr<mag_suspension > m_susp;
        std::unique_ptr<Curve_BH> m_curve;
};

#endif //MAG_MODEL
