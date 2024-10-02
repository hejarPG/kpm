#include <filesystem>

namespace fs
{
    using path = std::filesystem::path;

    void copy_directory(const fs::path &source, const fs::path &destination);
    bool exists(fs::path path);
}