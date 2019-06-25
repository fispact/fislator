#include <iostream>
#include <vector>

#include "fislater.hpp"

int main(int argv, char** argc){
    auto j = fispact::from_json("test31.json");

    for(auto v: fispact::inventoryvalues<double>(j, "total_heat"))
        std::cout << v << "\n";

    fispact::to_csv(j, "test31.csv");
    return 0;
}
