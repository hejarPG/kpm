#include <iostream>
#include <string>
#include <filesystem>
#include <windows.h>
#include <fstream>
#include <nlohmann/json.hpp>
#include <iomanip>
#include <fstream>
#include <chrono>
#include <ctime>

namespace fs = std::filesystem;
using json = nlohmann::json;

bool init(std::string name);
bool out();
bool integrate_files();
bool compile();
bool assemble();
bool link();
fs::path get_kpm_path();
bool copy_directory(const fs::path &source, const fs::path &destination);
bool flatten_project(const fs::path &source);
bool has_changed(fs::path path);
bool load_configs();
void update_configs();

fs::path PWD, KPM;

json configs;

int main(int argc, char *argv[])
{

    if (argc <= 1)
        return 0;

    PWD = std::filesystem::current_path();
    KPM = get_kpm_path();

    std::string command = argv[1];
    if (command == "init")
    {
        if (init((argc >= 3 ? argv[2] : ".")))
            std::cout << "---- Project started! \n";
    }
    else if (command == "out")
    {
        if (load_configs())
        {
            out();
        }
    }
    return 0;
}

fs::path get_kpm_path()
{
    WCHAR path[MAX_PATH];
    GetModuleFileNameW(NULL, path, MAX_PATH);

    std::wstring ws(path);
    std::string kpm_path(ws.begin(), ws.end());

    std::string redundant = "\\kpm.exe";

    kpm_path = kpm_path.substr(0, kpm_path.length() - redundant.length());

    return kpm_path;
}

bool init(std::string name)
{
    try
    {
        PWD = (name == "." ? PWD : PWD / name);
        if (!copy_directory(KPM / "init", PWD))
            return false;
        if (!fs::exists(PWD / "configs.js"))
        {
            json configs = {
                {"name", PWD.filename()},
                {"lastOut", time(NULL) - 1}};

            std::ofstream configs_file(PWD / "configs.json");
            configs_file << configs;
            configs_file.close();
        }
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
}

bool out()
{
    if (!integrate_files())
        return false;
    if (!compile())
        return false;
    if (!assemble())
        return false;
    if (!link())
        return false;

    std::string run = "cd out & blitz -g " + std::string(configs["name"]);
    system(run.c_str());
    return true;
}

bool integrate_files()
{
    try
    {
        std::cout << "---- Integrating All Files\n";
        fs::create_directories(PWD / "out");
        if (!fs::exists(PWD / "out" / "DISK"))
        {
            fs::copy_file(KPM / "DISK", PWD / "out" / "DISK");
        }

        std::cout << "Changes:\n";
        if (!flatten_project(PWD))
            return false;

        configs["lastOut"] = time(NULL);
        update_configs();
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
}

bool compile()
{
    try
    {
        std::cout << "---- Compiling KPL Files\n";
        for (const auto &entry : fs::directory_iterator(PWD / "out"))
        {
            const auto &path = entry.path();
            if (path.extension() == ".c")
            {
                const std::string cmd = "cd out & kpl " + path.filename().replace_extension("").string();
                int status = system(cmd.c_str());
                if (status != 0)
                    return false;
            }
        }
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
}

bool assemble()
{
    try
    {
        std::cout << "---- Assembling All .s Files \n";
        for (const auto &entry : fs::directory_iterator(PWD / "out"))
        {
            const auto &path = entry.path();
            if (path.extension() == ".s")
            {
                const std::string cmd = "cd out & asm " + path.filename().string();
                int status = system(cmd.c_str());
                if (status != 0)
                    return false;
            }
        }
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
}

bool link()
{
    try
    {
        std::cout << "---- Linking All .o Files \n";
        std::string objs = "";
        for (const auto &entry : fs::directory_iterator(PWD / "out"))
        {
            const auto &path = entry.path();
            if (path.extension() == ".o")
            {
                objs += (" " + path.filename().string());
            }
        }

        std::string cmd = "cd out & lddd " + objs + " -o " + std::string(configs["name"]);

        int status = system(cmd.c_str());
        if (status != 0)
            return false;
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
}

bool copy_directory(const fs::path &source, const fs::path &destination)
{
    try
    {
        fs::create_directories(destination);

        for (const auto &entry : fs::directory_iterator(source))
        {
            const auto &path = entry.path();
            auto dest_path = destination / path.filename();

            if (fs::is_directory(path))
            {
                copy_directory(path, dest_path);
            }
            else if (fs::is_regular_file(path))
            {
                if (fs::exists(dest_path))
                {
                    fs::remove(dest_path);
                }
                fs::copy_file(path, dest_path, fs::copy_options::update_existing);
            }
        }
        return true;
    }
    catch (const fs::filesystem_error &e)
    {
        std::cout << "Error: " << e.what() << "\n";
        return false;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return false;
    }
}

bool flatten_project(const fs::path &source)
{
    try
    {
        for (const auto &entry : fs::directory_iterator(source))
        {
            const auto &path = entry.path();

            if (fs::is_directory(path))
            {
                if (path.filename() != "out")
                    flatten_project(path);
            }
            else if (fs::is_regular_file(path))
            {
                if (path.filename() == "configs.json")
                    continue;

                if (has_changed(path))
                {
                    fs::remove(PWD / "out" / path.filename());
                    std::cout << "\t" << path.filename() << "\n";
                    fs::copy_file(path, PWD / "out" / path.filename());
                }
            }
        }
        return true;
    }
    catch (const fs::filesystem_error &e)
    {
        std::cout << "Error: " << e.what() << "\n";
        return false;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return false;
    }
}

bool has_changed(fs::path path)
{
    auto temp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(std::filesystem::last_write_time(path) - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now());
    std::time_t last_modified = std::chrono::system_clock::to_time_t(temp);
    return (std::difftime(last_modified, configs["lastOut"]) > 0);
}

bool load_configs()
{
    try
    {
        std::ifstream configs_file(PWD / "configs.json");
        configs = json::parse(configs_file);
        configs_file.close();
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error reading configs.json file: " << e.what() << '\n';
        return false;
    }
}

void update_configs()
{
    std::ofstream configs_file(PWD / "configs.json");
    configs_file << configs;
    configs_file.close();
}
