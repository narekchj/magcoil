#ifndef _MAG_SUSPENSION_
#define _MAG_SUSPENSION_

#include "consts.hpp"
#include "steel.hpp"

#define mag_attribute(x) \
    private: \
    float m_##x = 0.0f; \
    public: \
    inline void set_##x(float val) { m_##x = val; } \
    inline float get_##x() const { return m_##x; } \

//TODO: Add steel class for mag_suspension and move B(H) curve there.

/**
  * This class represents magnetic suspension.
  * The param names similar to the construction within the image.
  */
class mag_suspension
{
    public:
        mag_suspension() = default;
        ~mag_suspension() = default;

        // Set of the main params which values need to be set.
        mag_attribute(a_m)
        mag_attribute(b_m)
        mag_attribute(l_m)
        mag_attribute(b_h)
        mag_attribute(l_h)
        mag_attribute(a_x)
        mag_attribute(b_x)
        mag_attribute(l_x)
        mag_attribute(air_gap) // gap represents δ
        mag_attribute(Delta_p) 
        mag_attribute(Delta_m) 
        mag_attribute(l_p)
        mag_attribute(h_p)

        const steel& get_steel() const 
        {
            if (!s_steel) throw std::runtime_error("No steel set!");

            return *s_steel; 
        }

        void set_steel(const std::string_view st_name)
        {
            auto it = steel_repo.find(st_name);
            if (it == steel_repo.end()) throw ("Unknown steel!");

            s_steel = (*it).second;
        }

    private:
        std::shared_ptr<steel> s_steel = steel_repo["steel_10"];
};

#endif //_MAG_SUSPENSION_
