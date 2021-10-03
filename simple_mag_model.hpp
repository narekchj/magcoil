#ifndef SIMPLE_MAG_MODEL
#define SIMPLE_MAG_MODEL

#include "mag_model.hpp"

class simple_model : public mag_model
{
    public:
        simple_model(const susp_base_sizes& sz)
        {
            init_suspension();
            init_suspension_base(sz);
        }

        virtual void init_suspension() override
        {
            m_susp = std::make_unique<mag_suspension>();
            m_curve = std::make_unique<Curve_BH>("./steel_bh/steel_10.txt");
        }
};

#endif //SIMPLE_MAG_MODEL
