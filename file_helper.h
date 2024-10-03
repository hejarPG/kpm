#include <filesystem>

namespace fs = std::filesystem;

namespace fh
{
    void copy_directory(fs::path source, fs::path destination);
    bool flatten(fs::path source, fs::path dest, time_t since);
    bool has_changed(fs::path path, time_t since);
}