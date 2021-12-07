#include <string>

#include "ratio_mag_model.hpp"
#include "simple_mag_model.hpp"
#include "gen_optim.hpp"
#include "utils.hpp"

int main(int argc, char** argv)
{
    if (argc == 1)
    {
      std::printf("Generate and save data to the 'data' file.\n");
      generateAndSaveToFile("data", 100);

      return 0;
    }

    if (argc < 3)
    {
        std::cout << "You need provide iteration count and file name." << std::endl;
        return 0;
    }

    const std::string fileName = argv[2];

    GenOptimizer opt;
    opt.createInitialPopulation(loadFromFile<TSharedDataVec>(fileName));
    opt.runOptimization(std::stoul(argv[1]));

    return 0;
}
