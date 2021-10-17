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
            m_susp = std::make_shared<mag_suspension>();
        }
};

#endif //SIMPLE_MAG_MODEL
