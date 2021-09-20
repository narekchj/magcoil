
#include "ratio_mag_model.hpp"

int main()
{
    ratio_model rm(200000, 20);

    rm.init_suspension();
    rm.calculate();
}
