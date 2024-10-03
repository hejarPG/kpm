#include "linker.h"

#include "console_color.h"
#include "includes.h"
#include "file_helper.h"
#include "prj.h"

bool linker::link(fs::path dir)
{
    try
    {
        std::cout << color::green << "* Linking .s files\n"
                  << color::white;
        std::string objs = "";
        for (const auto &entry : fs::directory_iterator(dir))
        {
            const auto &path = entry.path();
            if (path.extension() == ".o")
            {
                objs += (" " + path.filename().string());
            }
        }

        std::string cmd = "cd out & lddd " + objs + " -o " + std::string(prj::get_configs()["name"]);

        int status = system(cmd.c_str());
        if (status != 0)
            return false;
        return true;
    }
    catch (const std::exception &e)
    {
        std::cout << color::red << "Error: " << color::white << e.what() << '\n';
        return false;
    }
}
