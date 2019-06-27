#include <iostream>
#include <vector>

#include <CLI/CLI.hpp>

#include "fislator.hpp"


int main(int argc, char** argv){
    CLI::App app{"FISPACT-II JSON translator"};

    std::string filename = "default";
    bool usecsv = false;
    app.add_option("-f,--file", filename, "The FISPACT-II JSON file")
        ->required()
        ->check(CLI::ExistingFile);
    app.add_flag("--csv", usecsv, "Option to write csv");

    CLI11_PARSE(app, argc, argv);

    // read from JSON file
    auto j = fispact::io::from_json(filename);
    
    // strip of extension
    size_t lastindex = filename.find_last_of("."); 
    std::string rawname = filename.substr(0, lastindex); 

    // to csv
    if(usecsv)
        fispact::io::to_csv(j, rawname + ".csv");

    return 0;
}
