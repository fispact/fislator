#include <iostream>
#include <vector>

#include <CLI/CLI.hpp>

#include "fislator.hpp"


struct CLIOption{
    std::string name;
    bool use;
    std::function<void(const json&, std::string)> writefunc;
};

int main(int argc, char** argv){
    CLI::App app{"FISPACT-II JSON translator"};

    std::string filename = "default";
    std::vector<CLIOption> options = {
        { "csv", false, fislator::inventory::to_csv },
        { "tab", false, fislator::inventory::to_tab }
    };
    app.add_option("-f,--file", filename, "The FISPACT-II JSON file")
        ->required()
        ->check(CLI::ExistingFile);
        
    for (auto& option : options)
        app.add_flag("--" + option.name, option.use, "Option to write " + option.name + " file");

    CLI11_PARSE(app, argc, argv);

    // read from JSON file
    auto j = fislator::from_json(filename);
    
    // strip of extension
    size_t lastindex = filename.find_last_of("."); 
    std::string rawname = filename.substr(0, lastindex); 

    // file options
    for (auto const& option : options) {
        if(option.use)
            option.writefunc(j, rawname + "." + option.name);
    }

    return 0;
}
