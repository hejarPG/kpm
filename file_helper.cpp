#include "file_helper.h"
#include <filesystem>

void fs::copy_directory(const std::filesystem::path &source, const std::filesystem::path &destination)
{
    std::filesystem::create_directories(destination);

    for (const auto &entry : std::filesystem::directory_iterator(source))
    {
        const auto &path = entry.path();
        auto dest_path = destination / path.filename();

        if (std::filesystem::is_directory(path))
        {
            copy_directory(path, dest_path);
        }
        else if (std::filesystem::is_regular_file(path))
        {
            if (std::filesystem::exists(dest_path))
            {
                std::filesystem::remove(dest_path);
            }
            std::filesystem::copy_file(path, dest_path, std::filesystem::copy_options::update_existing);
        }
    }
}

bool fs::exists(std::filesystem::path path)
{
    return std::filesystem::exists(path);
}