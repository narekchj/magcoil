#include <string>

#include "ratio_mag_model.hpp"
#include "simple_mag_model.hpp"
#include "gen_optim.hpp"

int main()
{
    gen_optimizer opt;
    opt.runOptimization();

    return 0;
}
