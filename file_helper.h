#include "includes.h"
#include "prj.h"

namespace fh
{
    void copy_directory(fs::path source, fs::path destination);
    bool flatten(fs::path PWD, fs::path source, fs::path dest, std::regex ignores);
    bool has_changed(fs::path path);
}