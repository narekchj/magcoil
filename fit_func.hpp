#ifndef FIT_FUNC_HPP
#define FIT_FUNC_HPP

class PowerFit
{
    public:
        PowerFit() = default;
        ~PowerFit() = default;

        bool operator()(const auto& ld, const auto& rd)
        {
            return (*ld).coil_out.P < (*rd).coil_out.P;
        }
};

class PriceFit
{
    public:
        PriceFit() = default;
        ~PriceFit() = default;

        bool operator()(const auto& ld, const auto& rd)
        {
            return (*ld).other.price < (*rd).other.price;
        }
};

class PowerPriceFit
{
    public:
        PowerPriceFit() = default;
        ~PowerPriceFit() = default;

        bool operator()(const auto& ld, const auto& rd)
        {
            return mid(*ld) < mid(*rd);
        }

        static auto mid(const auto& data)
        {
            const auto mid = (data.other.price + data.coil_out.P)/2;
            const auto midq = (std::pow((data.other.price - mid), 2) + 
                std::pow((data.coil_out.P - mid), 2)) / 2;

            return std::sqrt(midq);
        }
};

#endif // FIT_FUNC_HPP
