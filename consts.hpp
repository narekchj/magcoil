#ifndef CONSTS
#define CONSTS
#include <optional>

inline constexpr std::size_t operator "" _sz(unsigned long long val) { return val; }

inline const auto PI = 3.14f;
inline const auto myu_0 = 4 * PI * 1E-7;
inline const auto Ro20 = 0.0175f;
inline const auto max_contures = 1000_sz;

typedef struct tsizes
{
    std::optional<float> a = {};
    std::optional<float> b = {};
    std::optional<float> h = {};
    std::optional<float> l = {};
    std::optional<float> c = {};
    std::optional<float> b_x = {};
    std::optional<float> gap = {};
    std::optional<float> h_p = {};
} tsizes;

template<typename T>
tsizes getSizes(const T& rm)
{
    const auto& susp = rm.get_susp();

    float a   = susp.get_a_m() * 10.0f;
    float b   = susp.get_a_m() * 10.0f;
    float h   = susp.get_l_m() * 10.0f;
    float l   = susp.get_l_h() * 10.0f;
    float c   = susp.get_b_h() * 10.0f;
    float b_x = susp.get_b_x() * 10.0f;
    float h_p = susp.get_h_p() * 10.0f;
    float gap = susp.get_air_gap() * 10.0f;

    return {a, b, h, l, c, b_x, gap, h_p};
}

#endif //CONSTS
