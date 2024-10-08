#include "prj.h"

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

        fh::copy_directory(KPM / "init", PWD);

        configs = {
            {"name", PWD.filename()},
            {"lastOut", 0},
            {"ignore", {".vscode", "out", "configs.json"}}};

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

bool prj::out(fs::path KPM, fs::path PWD)
{
    if (!fs::exists(PWD / "configs.json"))
        std::cout << color::red << "Error: " << color::white << "configs.json not found. initialize the project\n";

    load_configs(PWD);

    if (!integrate(KPM, PWD))
        return false;

    if (!compiler::compile(PWD / "out"))
        return false;

    if (!assembler::assemble(PWD / "out"))
        return false;

    if (!linker::link(PWD / "out"))
        return false;

    configs["lastOut"] = time(NULL);
    update_configs(PWD);

    run(PWD);

    return true;
}

void prj::run(fs::path PWD)
{
    load_configs(PWD);
    std::string command = "cd out & blitz -g " + std::string(configs["name"]);
    system(command.c_str());
}

bool prj::integrate(fs::path KPM, fs::path PWD)
{
    try
    {
        json ignores_json = configs["ignore"];
        std::string pattern_str = "";
        for (std::string pattern : ignores_json)
        {
            pattern_str += "|" + pattern;
        }

        std::regex ignores(pattern_str);

        std::cout << color::green << "* Integrating All Files\n"
                  << color::white;
        fs::create_directories(PWD / "out");
        if (!fs::exists(PWD / "out" / "DISK"))
        {
            fs::copy_file(KPM / "DISK", PWD / "out" / "DISK");
        }

        std::cout << color::blue << "Changes:\n"
                  << color::white;

        if (!fh::flatten(PWD, PWD, PWD / "out", ignores))
            return false;

        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
}

json prj::get_configs()
{
    return configs;
}