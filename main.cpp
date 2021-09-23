
#include "ratio_mag_model.hpp"

int main()
{
    ratio_model rm(200000, 20);

    rm.init_suspension();
    const auto F = rm.calculate_direct();
    std::cout << "Direct calculation is " << F << std::endl;
    const auto& [B, FF] = rm.calculate_reverse(F);
    std::cout << "B = " << B << " F = " << FF << std::endl;

    const auto temp = rm.calculate_coil(FF, 220.0f, 0.95f, 160.0f);
    std::cout << "Temp = " << temp << std::endl;
}
