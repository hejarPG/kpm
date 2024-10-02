#include "prj.h"
#include "file_helper.h"
#include "console_color.h"
#include <string>
#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

json configs = {};

bool prj::init(fs::path KPM, fs::path PWD)
{
    try
    {
        char answer = 'y';
        if (fs::exists(PWD / "configs.json"))
        {
            std::cout << "configs.json already exists. this probably means your project has already been initialized. continue? (y/n): ";
            std::cin >> answer;
        }

        if (answer == 'n' || answer != 'y')
            return true;

        if (!fs::exists(KPM / "init"))
        {
            std::cout << color::red << "Error: " << color::white << "init folder (alongside kpm) does not exists\n";
            return false;
        }

        fs::copy_directory(KPM / "init", PWD);

        configs = {
            {"name", PWD.filename()},
            {"lastOut", time(NULL) - 1},
            {"extenals", {}}};

        update_configs(PWD);

        return true;
    }
    catch (const std::exception &e)
    {
        std::cout << color::red << "Error: " << color::white << e.what() << '\n';
        return false;
    }

    return true;
}

void prj::load_configs(fs::path PWD)
{
    std::ifstream configs_file(PWD / "configs.json");
    configs = json::parse(configs_file);
    configs_file.close();
}

void prj::update_configs(fs::path PWD)
{
    std::ofstream configs_file(PWD / "configs.json");
    configs_file << configs;
    configs_file.close();
}
