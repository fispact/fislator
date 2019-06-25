#ifndef FISLATER_HPP
#define FISLATER_HPP

#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace fispact {

    json from_json(std::string name){
        std::ifstream ifs(name);
        return json::parse(ifs);
    }

    void to_csv(json& j, std::string name){
        std::ofstream ofs;
        ofs.open(name);

        std::vector<std::string> keys = { 
            "irradiation_time", "cooling_time", "flux", "alpha_heat", "beta_heat", "gamma_heat"
        };

        std::copy(keys.begin(), keys.end()-1, std::ostream_iterator<std::string>(ofs, ","));
        ofs << keys.back() <<"\n";

        for (auto& element: j["inventory_data"]){
            std::string delimiter = "";
            for (auto& key: keys) {
                ofs << delimiter << std::scientific << std::setprecision(15) << element[key].get<double>();
                delimiter = ",";
            }
            ofs <<"\n";
        }
        ofs.close();
    }

    template <typename T>
    std::vector<T> inventoryvalues(const json& j, std::string key){
        std::vector<T> values;
        for (auto& element: j["inventory_data"])
            values.emplace_back(element[key].get<T>());
        return values;
    }
}

#endif //FISLATER_HPP
