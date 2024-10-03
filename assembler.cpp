#include "assembler.h"

bool assembler::assemble(fs::path dir)
{
    try
    {
        std::cout << color::green << "* Assembling .s files\n"
                  << color::white;
        for (auto &entry : fs::directory_iterator(dir))
        {
            const auto &path = entry.path();
            if ((path.extension() == ".s") && fh::has_changed(path))
            {
                std::cout << path.filename() << "\n";
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
        std::cout << color::red << "Error: " << color::white << e.what() << '\n';
        return false;
    }
}
