#ifndef SUSP_DATA
#define SUSP_DATA

//TODO: add comparison operators for floating types.

typedef struct //TODO: review this
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

///
/// Represents whole data related to the mag susp calculation.
///
typedef struct 
{
    ///
    /// Represents single electric circle related data.
    ///
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

    // Data related to the direct calculation.
    using circle_t = std::vector<std::unique_ptr<circle>>;
    using circles_t = std::vector<circle_t>;

    ///
    /// Data to hold stuff for direct calculation in/out.
    ///
    struct 
    {
        float precision = 0.01f;
    } dir_in;

    struct
    {
        circles_t data; 
    } dir_out;

    ///
    /// Data to hold stuff for reverse calculation in/out.
    ///
    struct 
    {
        float precision = 0.01f;
    } rev_in;

    struct
    {
        circles_t data; 
        float B = 0.0f;
    } rev_out;

    ///
    /// Data to hold stuff for coil calculation in/out.
    ///
    struct 
    {
        float U = 0.0f;
        float k_fill = 0.0f;
        float T_allow = 0.0f;
    } coil_in;

    struct
    {
        float a_p = 0.0f; //mm
        float b_p = 0.0f; //mm
        float l_m = 0.0f; //m
        float Rot = 0.0f; 
        float D_wire = 0.0f; //mm
        float isol = 0.0f; //mm
        float W = 0.0f; 
        float S_wire = 0.0f; //mm2
        float R = 0.0f;
        float I = 0.0f;
        float P = 0.0f;
        float j = 0.0f;
        float A_p = 0.0f; //mm
        float B_p = 0.0f; //mm
        float S_cool = 0.0f; //mm2
        float T = 0.0f;
        float L_wire = 0.0f; //mm
    } coil_out;

} susp_data;

using circles_t = susp_data::circles_t;

///
/// Helper to access to the F from the calculation data.
///
inline std::optional<float> get_F(const circles_t& data)
{
    if (data.size() && data.back().back())
        return data.back().back()->U_out;

    return {};
}



#endif // SUSP_DATA
