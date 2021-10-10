#ifndef _MAG_SUSPENSION_
#define _MAG_SUSPENSION_

inline const auto PI = 3.14f;
inline const auto myu_0 = 4 * PI * 1E-7;
inline const auto Ro20 = 0.0175f;
inline const auto max_contures = (size_t)1000; //TODO

#define mag_attribute(x) \
    private: \
    float m_##x = 0.0f; \
    public: \
    inline void set_##x(float val) { m_##x = val; } \
    inline float get_##x() const { return m_##x; } \

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
        mag_attribute(P_e)
        mag_attribute(a_m)
        mag_attribute(b_m)
        mag_attribute(l_m)
        mag_attribute(b_h)
        mag_attribute(l_h)
        mag_attribute(a_x)
        mag_attribute(b_x)
        mag_attribute(l_x)
        mag_attribute(air_gap) // gap represents δ
        mag_attribute(B_air) // gap induction
        mag_attribute(Delta_p) 
        mag_attribute(Delta_m) 
        mag_attribute(l_p)
        mag_attribute(h_p)
};

#endif //_MAG_SUSPENSION_
