#ifndef _MAG_SUSPENSION_
#define _MAG_SUSPENSION_

#define mag_attribute(x) \
    private: \
    float m_##x = 0.0f; \
    public: \
    void set_##x(float val) { m_##x = val; } \
    float get_##x() const { return m_##x; } \

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
        mag_attribute(am);
        mag_attribute(bm);
        mag_attribute(lm);
        mag_attribute(bh);
        mag_attribute(lh);
        mag_attribute(ax);
        mag_attribute(bx);
        mag_attribute(lx);
        mag_attribute(air_gap); // gap represents δ
        mag_attribute(Dp); 
        mag_attribute(Dm); 
        mag_attribute(lp);
        mag_attribute(hp);
};

#endif //_MAG_SUSPENSION_
