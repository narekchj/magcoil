#ifndef UTILS_HPP
#define UTILS_HPP

#ifdef DEBUG
#define DEBUGP(...) printf("line %d file %s\n", __LINE__, __FILE__)
#define DEBUGM(...) printf(__VA_ARGS__)
#else
#define DEBUGP(...) 
#define DEBUGM(...) 
#endif

#include <sstream>
#include "ratio_mag_model.hpp"

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

using TUniqueDataVec = std::vector<std::unique_ptr<ratio_susp_data>>;
using TSharedDataVec = std::vector<std::shared_ptr<ratio_susp_data>>;

using ratio_model_t = ratio_model<ratio_susp_data>;

namespace 
{
    auto generateRandomData(const size_t inCount = 100)
    {
        TSharedDataVec initPop;
        std::mutex inMutex;

        const auto pass = [](const auto& dt) 
        { return dt.coil_out.T >= 130 && dt.coil_out.T <= 160; };

        const auto populate = [&inCount, &initPop, &inMutex, &pass]() {
            ratio_model_t rm(200000, 20);
            while (true)
            {
                auto sp_data = std::make_unique<ratio_susp_data>();
                rm.init_suspension(*sp_data);
                rm.calculate_direct(*sp_data);
                rm.calculate_reverse(*sp_data);
                rm.calculate_coil(*sp_data);

                if (!pass(*sp_data)) continue;

                {
                    std::lock_guard<std::mutex> guard(inMutex);
                    if (initPop.size() >= inCount) return;
                    // the individum is fine add to population
                    initPop.push_back(std::move(sp_data));
                }
            }
        };

        const auto hardNum = std::thread::hardware_concurrency();
        {
            std::vector<std::future<void>> f {hardNum};
            for(size_t i = 0; i < hardNum; ++i)
              f[i] = std::async(std::launch::async, populate);
        }

        return initPop;
    }

    std::ostream& operator<<(std::ostream& inStream, const ratio_susp_data& sp_data)
    {
        inStream << sp_data.cpack.k_e.value() << " "
            << sp_data.cpack.B_air.value() << " "
            << sp_data.cpack.k_m.value() << " "
            << sp_data.cpack.k_mm.value() << " "
            << sp_data.cpack.k_x.value() << " "
            << sp_data.cpack.k_h.value() << " "
            << sp_data.cpack.k_delta.value() << " "
            << sp_data.cpack.k_p.value();

        return inStream;
    }

    void generateAndSaveToFile(const std::string_view inPath,
            const size_t inCount)
    {
        if (inCount == 0) return;

        const auto& randData = generateRandomData(inCount);
        std::ofstream fileToWrite(inPath.data());

        std::for_each(std::begin(randData), std::end(randData) - 1, 
                [&fileToWrite](const auto& data)
                { fileToWrite << *data << std::endl;});

        // adding the last element without new line
        fileToWrite << *randData.back();
    }


    std::stringstream& operator>>(std::stringstream& inStream, ratio_susp_data& sp_data)
    {
        float k_e, B_air, k_m, k_mm, k_x, k_h, k_delta, k_p;

        inStream >> k_e >> B_air >> k_m >> k_mm
            >> k_x >> k_h >> k_delta >> k_p;

        sp_data.cpack.k_e     = k_e;
        sp_data.cpack.B_air   = B_air;
        sp_data.cpack.k_m     = k_m;
        sp_data.cpack.k_mm    = k_mm;
        sp_data.cpack.k_x     = k_x;
        sp_data.cpack.k_h     = k_h;
        sp_data.cpack.k_delta = k_delta;
        sp_data.cpack.k_p     = k_p;

        return inStream;
    }

    template<typename T>
    T loadFromFile(const std::string_view inPath = "")
    {
        // try to read from given file
        std::ifstream fileToRead;
        fileToRead.open(inPath.data());

        if (!fileToRead.is_open())
        {
            generateAndSaveToFile(inPath, 100);
            const std::string str = std::string("File ")
            + std::string(inPath)
            + std::string(" is not opened, it seems missing.\n")
            + std::string("New one with random data is created. Try one more time\n");

            throw std::runtime_error(str);
        }

        T data;
        // get data from the file
        const size_t ssize = 256;
        char c[ssize];
        while (fileToRead.getline(c, ssize))
        {
            auto sp_data = std::make_unique<ratio_susp_data>();
            std::stringstream str(c); str >> *sp_data;
            data.push_back(std::move(sp_data));
        }

        DEBUGM("loaded size %zu\n", data.size());
        return data;
    }
}

#endif //UTILS_HPP
