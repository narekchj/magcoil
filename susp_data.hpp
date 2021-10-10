#ifndef SUSP_DATA
#define SUSP_DATA

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

///
/// Represents all data about the suspension coil.
///
typedef struct
{
    float a_p = 0.0f; //mm
    float b_p = 0.0f; //mm
    float l_m = 0.0f; //m
    float Rot = 0.0f; 
    float D_wire = 0.0f; //mm
    float isol = 0.0f; //mm
    float w_coil = 0.0f; 
    float S_wire = 0.0f; //mm2
    float R_coil = 0.0f;
    float I_coil = 0.0f;
    float P = 0.0f;
    float j = 0.0f;
    float A_p = 0.0f; //mm
    float B_p = 0.0f; //mm
    float S_cool = 0.0f; //mm2
    float Temp = 0.0f;
    float Len_wire = 0.0f; //mm
} coil;

///
/// Represents whole data related to the mag susp calculation.
///
typedef struct 
{
    // Data related to the direct calculation.
    using circle_t = std::vector<std::unique_ptr<circle>>;
    using direct_data_t = std::vector<circle_t>;
    direct_data_t m_direct_data; 


    // Data related to the reverse calculation.
    std::vector<circle_t> m_reverse_data; 
    float m_reverse_B = 0.0f;

    // Data related to the coil calculation.
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

} susp_out_data;

using direct_data_t = susp_out_data::direct_data_t;

///
/// Helper to access to the F from the calculation data.
///
inline std::optional<float> get_F(const direct_data_t& data)
{
    if (data.size() && data.back().back())
        return data.back().back()->U_out;

    return {};
}



#endif // SUSP_DATA
