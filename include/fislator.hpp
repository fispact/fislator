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

            // double value keys
            const std::vector<std::string> inventorykeys = { 
                "irradiation_time", 
                "cooling_time", 
                "flux"
            };

            // double value keys
            const std::vector<std::string> nuclidekeys = { 
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

            // string value keys
            const std::string nuclidenamekey = "nuclide";

            const std::vector<std::string> optionalinventorykeys ={
                "total_mass"
            };
            
            const std::vector<std::string> optionalnuclidekeys ={
                "zai", 
                "atoms", 
                "alpha_activity", 
                "beta_activity", 
                "gamma_activity"
            };

            // inventory keys
            for(auto it=inventorykeys.begin(); it<inventorykeys.end();++it)
                ofs << std::setw(width) << *it << delimiter;

            // nuclide keys
            for(auto it=nuclidekeys.begin(); it<nuclidekeys.end();++it)
                ofs << std::setw(width) << *it << delimiter;

            // nuclide name key
            ofs << std::setw(width) << nuclidenamekey;

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
                    for(auto it=inventorykeys.begin(); it<inventorykeys.end();++it){
                        ofs << std::setw(width) << std::scientific << std::setprecision(15) << timestep[*it].get<double>() << delimiter;
                    }

                    for(auto it=nuclidekeys.begin(); it<nuclidekeys.end();++it){
                        ofs << std::setw(width) << std::scientific << std::setprecision(15) << nuclide[*it].get<double>() << delimiter;
                    }

                    // special case - nuclide name
                    ofs << std::setw(width)
                        << nuclide["element"].get<std::string>() 
                        + std::to_string(nuclide["isotope"].get<int>()) 
                        + nuclide["state"].get<std::string>() << "\n";
                }
            }
            ofs.close();
        }

        const auto to_csv = to_sep_file<',', 0>;
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
