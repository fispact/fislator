#ifndef FISLATER_HPP
#define FISLATER_HPP

#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace fislator {

    // process from JSON file from FISPACT-II
    json from_json(std::string name){
        std::ifstream ifs(name);
        return json::parse(ifs);
    }

    // inventory operations
    namespace inventory { 

        template<char delimiter, int width=30>
        void to_sep_file(const json& j, std::string name){
            std::ofstream ofs;
            ofs.open(name);

            std::vector<std::string> keys = { 
                "irradiation_time", 
                "cooling_time", 
                "flux", 
                "element", 
                "isotope",
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

            // std::copy(keys.begin(), keys.end()-1, std::ostream_iterator<std::string>(ofs, delimiter));
            for(auto it=keys.begin(); it<keys.end()-1;++it)
                ofs << std::setw(width) << *it << delimiter;
            ofs << std::setw(width) << keys.back() <<"\n";

            for (auto& timestep: j["inventory_data"]){
                for (auto& nuclide: timestep["nuclides"]){
                    ofs << std::setw(width) << std::scientific << std::setprecision(15) << timestep["irradiation_time"].get<double>() << delimiter
                        << std::setw(width) << std::scientific << std::setprecision(15) << timestep["cooling_time"].get<double>() << delimiter
                        << std::setw(width) << std::scientific << std::setprecision(15) << timestep["flux"].get<double>() << delimiter
                        << std::setw(width) << nuclide["element"].get<std::string>() << delimiter
                        << std::setw(width) << nuclide["isotope"].get<int>() << nuclide["state"].get<std::string>() << delimiter
                        << std::setw(width) << std::scientific << std::setprecision(15) << nuclide["grams"].get<double>() << delimiter
                        << std::setw(width) << std::scientific << std::setprecision(15) << nuclide["activity"].get<double>() << delimiter
                        << std::setw(width) << std::scientific << std::setprecision(15) << nuclide["alpha_heat"].get<double>() << delimiter
                        << std::setw(width) << std::scientific << std::setprecision(15) << nuclide["beta_heat"].get<double>() << delimiter
                        << std::setw(width) << std::scientific << std::setprecision(15) << nuclide["gamma_heat"].get<double>() << delimiter
                        << std::setw(width) << std::scientific << std::setprecision(15) << nuclide["half_life"].get<double>() << delimiter
                        << std::setw(width) << std::scientific << std::setprecision(15) << nuclide["dose"].get<double>() << delimiter
                        << std::setw(width) << std::scientific << std::setprecision(15) << nuclide["ingestion"].get<double>() << delimiter
                        << std::setw(width) << std::scientific << std::setprecision(15) << nuclide["inhalation"].get<double>() << "\n";
                }
            }
            ofs.close();
        }

        const auto to_csv = to_sep_file<','>;
        const auto to_tab = to_sep_file<' '>;

        template <typename T>
        std::vector<T> values(const json& j, std::string key){
            std::vector<T> values;
            for (auto& element: j["inventory_data"])
                values.emplace_back(element[key].get<T>());
            return values;
        }

    }// namespace inventory

} // namespace fislator

#endif //FISLATER_HPP
