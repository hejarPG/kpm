#include "includes.h"

namespace fh
{
    void copy_directory(fs::path source, fs::path destination);
    bool flatten(fs::path source, fs::path dest);
    bool has_changed(fs::path path);
}