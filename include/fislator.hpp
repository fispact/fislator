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
json from_json(std::string name) {
  std::ifstream ifs(name);
  return json::parse(ifs);
}

// returns true if valid F-II JSON file, false otherwise
bool validate(const json &j) {
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

template <char delimiter, int width = 30>
void to_sep_file(const json &j, std::string name) {

  // check is FISPACT-II valid format
  if (!validate(j))
    throw InvalidJSONFileException("JSON file is not from FISPACT-II");

  std::ofstream ofs;
  ofs.open(name);

  // double value keys
  const std::vector<std::string> inventorykeys = {
      "irradiation_time", "cooling_time", "flux", "total_mass"};

  // double value keys
  const std::vector<std::string> nuclidekeys = {
      "zai",           "atoms",          "alpha_activity",
      "beta_activity", "gamma_activity", "grams",
      "activity",      "alpha_heat",     "beta_heat",
      "gamma_heat",    "half_life",      "dose",
      "ingestion",     "inhalation",
  };

  // string value keys
  const std::string nuclidenamekey = "nuclide";

  // nuclide first
  ofs << std::setw(width) << nuclidenamekey << delimiter << "time_step";

  // inventory keys
  for (auto it = inventorykeys.begin(); it < inventorykeys.end(); ++it)
    ofs << std::setw(width) << delimiter << *it;

  // nuclide keys
  for (auto it = nuclidekeys.begin(); it < nuclidekeys.end(); ++it)
    ofs << std::setw(width) << delimiter << *it;

  ofs << "\n";

  double value = -1.0;
  for (size_t i = 0; i < j["inventory_data"].size(); ++i) {
    const auto timestep = j["inventory_data"][i];
    for (auto &nuclide : timestep["nuclides"]) {
      // nuclide name
      ofs << std::setw(width)
          << nuclide["element"].get<std::string>() +
                 std::to_string(nuclide["isotope"].get<int>()) +
                 nuclide["state"].get<std::string>();

      // timestep
      ofs << std::setw(width) << delimiter << i;

      for (auto it = inventorykeys.begin(); it < inventorykeys.end(); ++it) {
        value = -1.0;
        if (timestep.find(*it) != timestep.end())
          value = timestep[*it].get<double>();
        ofs << std::setw(width) << std::scientific << std::setprecision(15)
            << delimiter << value;
      }

      for (auto it = nuclidekeys.begin(); it < nuclidekeys.end(); ++it) {
        value = -1.0;
        if (nuclide.find(*it) != nuclide.end())
          value = nuclide[*it].get<double>();
        ofs << std::setw(width) << std::scientific << std::setprecision(15)
            << delimiter << value;
      }
      ofs << "\n";
    }
  }
  ofs.close();
}

const auto to_csv = to_sep_file<',', 0>;
const auto to_tab = to_sep_file<' '>;

template <typename T> std::vector<T> values(const json &j, std::string key) {
  std::vector<T> values;
  for (auto &element : j["inventory_data"])
    values.emplace_back(element[key].get<T>());
  return values;
}

} // namespace inventory

} // namespace fislator

#endif // FISLATER_HPP
