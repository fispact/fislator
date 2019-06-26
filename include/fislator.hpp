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

    namespace io {
        
        json from_json(std::string name){
            std::ifstream ifs(name);
            return json::parse(ifs);
        }

        void to_csv(json& j, std::string name){
            std::ofstream ofs;
            ofs.open(name);

            std::vector<std::string> keys = { 
                "irradiation_time", 
                "cooling_time", 
                "flux", 
                "element", 
                "isotope",
                "state",
                "grams", 
                "activity",
                "alpha_heat", 
                "beta_heat", 
                "gamma_heat",
                "half_life",
                "dose",
                "ingestion",
                "inhalation"
            };

            std::copy(keys.begin(), keys.end()-1, std::ostream_iterator<std::string>(ofs, ","));
            ofs << keys.back() <<"\n";

            for (auto& timestep: j["inventory_data"]){
                for (auto& nuclide: timestep["nuclides"]){
                    std::string delimiter = "";
                    ofs << delimiter << std::scientific << std::setprecision(15) << timestep["irradiation_time"].get<double>() << ","
                        << delimiter << std::scientific << std::setprecision(15) << timestep["cooling_time"].get<double>() << ","
                        << delimiter << std::scientific << std::setprecision(15) << timestep["flux"].get<double>() << ","
                        << delimiter << nuclide["element"].get<std::string>() << ","
                        << delimiter << nuclide["isotope"].get<int>() << ","
                        << delimiter << nuclide["state"].get<std::string>() << ","
                        << delimiter << std::scientific << std::setprecision(15) << nuclide["grams"].get<double>() << ","
                        << delimiter << std::scientific << std::setprecision(15) << nuclide["activity"].get<double>() << ","
                        << delimiter << std::scientific << std::setprecision(15) << nuclide["alpha_heat"].get<double>() << ","
                        << delimiter << std::scientific << std::setprecision(15) << nuclide["beta_heat"].get<double>() << ","
                        << delimiter << std::scientific << std::setprecision(15) << nuclide["gamma_heat"].get<double>() << ","
                        << delimiter << std::scientific << std::setprecision(15) << nuclide["half_life"].get<double>() << ","
                        << delimiter << std::scientific << std::setprecision(15) << nuclide["dose"].get<double>() << ","
                        << delimiter << std::scientific << std::setprecision(15) << nuclide["ingestion"].get<double>() << ","
                        << delimiter << std::scientific << std::setprecision(15) << nuclide["inhalation"].get<double>() << "\n";
                }
            }
            ofs.close();
        }
    } // namespace io

    template <typename T>
    std::vector<T> inventoryvalues(const json& j, std::string key){
        std::vector<T> values;
        for (auto& element: j["inventory_data"])
            values.emplace_back(element[key].get<T>());
        return values;
    }

} // namespace fispact

#endif //FISLATER_HPP
