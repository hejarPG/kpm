#include "includes.h"

#include "prj.h"

fs::path get_kpm_path();

fs::path PWD, KPM;

int main(int argc, char *argv[])
{

    if (argc <= 1)
        return 0;

    PWD = std::filesystem::current_path();
    KPM = get_kpm_path();

    std::string command = argv[1];
    if (command == "init")
    {
        if (prj::init(KPM, argc >= 3 ? PWD / argv[2] : PWD))
        {
            std::cout << color::green << "* Project started!\n"
                      << color::white;
        }
        else
        {
            std::cout << color::red << "* Failed \n"
                      << color::white;
        }
    }
    else if (command == "out")
    {
        prj::out(KPM, PWD);
    }
    else if (command == "run")
    {
        prj::run(PWD);
    }
    return 0;
}

fs::path get_kpm_path()
{
    WCHAR path[MAX_PATH];
    GetModuleFileNameW(NULL, path, MAX_PATH);

    std::wstring ws(path);
    std::string kpm_path(ws.begin(), ws.end());

    std::string redundant = "\\outDebug.exe";
    kpm_path = kpm_path.substr(0, kpm_path.length() - redundant.length());

    return kpm_path;
}