#ifndef FISLATER_HPP
#define FISLATER_HPP

#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

#include <nlohmann/json.hpp>

#include "exceptions.hpp"

using json = nlohmann::json;

namespace fislator {

    // process from JSON file from FISPACT-II
    json from_json(std::string name){
        std::ifstream ifs(name);
        return json::parse(ifs);
    }

    // returns true if valid F-II JSON file, false otherwise
    bool validate(const json& j){
        auto it_fispact = j.find("run_data");
        if (it_fispact == j.end())
            return false;

        it_fispact = j.find("inventory_data");
        if (it_fispact == j.end())
            return false;

        return true;
    }

    // inventory operations
    namespace inventory { 

        template<char delimiter, int width=30>
        void to_sep_file(const json& j, std::string name){

            // check is FISPACT-II valid format
            if(!validate(j))
                throw InvalidJSONFileException("JSON file is not from FISPACT-II");

            std::ofstream ofs;
            ofs.open(name);

            std::vector<std::string> keys = { 
                "irradiation_time", 
                "cooling_time", 
                "flux", 
                "grams", 
                "activity",
                "alpha_heat", 
                "beta_heat", 
                "gamma_heat",
                "half_life",
                "dose",
                "ingestion",
                "inhalation",
                "element", 
                "isotope"
            };

            std::vector<std::string> optionalinventorykeys ={
                "total_mass"
            };
            std::vector<std::string> optionalnuclidekeys ={
                "zai", 
                "atoms", 
                "alpha_activity", 
                "beta_activity", 
                "gamma_activity"
            };

            // original keys
            for(auto it=keys.begin(); it<keys.end()-1;++it)
                ofs << std::setw(width) << *it << delimiter;
            ofs << std::setw(width) << keys.back();

            // TODO: additional (newly added keys for inventory)
            // for(auto it=optionalinventorykeys.begin(); it<optionalinventorykeys.end()-1;++it){
            //     if (it == optionalinventorykeys.begin())
            //         ofs << delimiter;
            //     if (j["inventory_data"] != j_object.end()) 
            //         ofs << std::setw(width) << *it << delimiter;
            // }
            // ofs << std::setw(width) << optionalinventorykeys.back();
            ofs <<"\n";

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
