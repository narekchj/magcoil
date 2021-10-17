#ifndef STEEL_HPP
#define STEEL_HPP

#include <map>
#include "curve.hpp"

inline const std::string_view curve_path = "./steel_bh/";

///
/// Class to represent the steel of the suspension bulk.
///
class steel
{
    public:
        steel(std::string_view n) : name(n)
        {
            const auto f_path = curve_path.data() + name + ".txt";
            BtoH = std::make_unique<Curve_BH>(f_path);
        }

        float get_H(float B) const
        {
            return BtoH->get_value_X(B);
        }

    private:
        // the type/name of the steel
        std::string name;

        // B to H curve of the steel
        std::unique_ptr<Curve> BtoH;
};

///
/// Container for all available steels. //TODO: get rid of the name duplications.
///
inline std::map<std::string_view, std::shared_ptr<steel>> steel_repo =
{
    {"steel_3", std::make_shared<steel>("Steel_3")}, // TODO: doesn't work with this steel
    {"steel_1211_1212_1311", std::make_shared<steel>("steel_1211_1212_1311")}, 
    {"steel_1411_1412_1413", std::make_shared<steel>("steel_1411_1412_1413")}, 
    {"steel_1511_1512_1513", std::make_shared<steel>("steel_1511_1512_1513")}, 
    {"steel_2013", std::make_shared<steel>("steel_2013")}, 
    {"steel_2211_2312", std::make_shared<steel>("steel_2211_2312")}, 
    {"steel_2411", std::make_shared<steel>("steel_2411")}, 
    {"steel_3413", std::make_shared<steel>("steel_3413")}, 
    {"hiperco_50", std::make_shared<steel>("hiperco_50")}, 
    {"pure_iron", std::make_shared<steel>("pure_iron")}, 
    {"steel_10", std::make_shared<steel>("steel_10")}, 
    {"steel_20", std::make_shared<steel>("steel_20")}, 
    {"steel_22", std::make_shared<steel>("steel_22")}, 
    {"steel_3409", std::make_shared<steel>("steel_3409")}, 
    {"steel_m19", std::make_shared<steel>("steel_m19")}
};

#endif //STEEL_HPP
