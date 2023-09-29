#ifndef CONSTS
#define CONSTS
#include <optional>

inline float operator ""_m(long double in) { return 10.0f * in; }

inline const auto PI = 3.14f;
inline const auto myu_0 = 4 * PI * 1E-7;
inline const auto Ro20 = 0.0175f;
inline const auto max_contures = 1000;

typedef struct tsizes
{
    std::optional<float> a = {};
    std::optional<float> b = {};
    std::optional<float> h = {};
    std::optional<float> l = {};
    std::optional<float> c = {};
    std::optional<float> b_x = {};
    std::optional<float> gap = {};
    std::optional<float> delm = {};
    std::optional<float> h_p = {};

    std::optional<float> P = {}; //TODO: get rid of those
    std::optional<float> T = {};

} tsizes;

template<typename T>
tsizes getSizes(const T& rm)
{
    const auto& susp = rm.get_susp();

    float a   = susp.get_a_m() * 10.0f;
    float b   = susp.get_b_m() * 10.0f;
    float h   = susp.get_l_m() * 10.0f;
    float l   = susp.get_l_h() * 10.0f;
    float c   = susp.get_b_h() * 10.0f;
    float b_x = susp.get_b_x() * 10.0f;
    float h_p = susp.get_h_p() * 10.0f;
    float gap = susp.get_air_gap() * 10.0f;

    return {a, b, h, l, c, b_x, gap, h_p};
}

#endif //CONSTS
