#ifndef SIMPLE_MAG_MODEL
#define SIMPLE_MAG_MODEL

#include "mag_model.hpp"

class simple_model : public mag_model<susp_data>
{
    public:
        simple_model(susp_data& data, const susp_base_sizes& sz)
        {
            init_suspension(data);
            init_suspension_base(data, sz);
        }

        virtual void init_suspension(susp_data& data) override
        {
            data.susp = std::make_unique<mag_suspension>();
            data.curve = std::make_unique<Curve_BH>("./steel_bh/steel_10.txt");
        }
};

#endif //SIMPLE_MAG_MODEL
