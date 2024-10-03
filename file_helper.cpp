#include "file_helper.h"

void fh::copy_directory(fs::path source, fs::path dest)
{
    fs::create_directories(dest);

    for (const auto &entry : fs::directory_iterator(source))
    {
        const auto &path = entry.path();
        auto dest_path = dest / path.filename();

        if (fs::is_directory(path))
        {
            fh::copy_directory(path, dest_path);
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
}

bool fh::flatten(fs::path PWD, fs::path source, fs::path dest, std::regex ignores)
{
    try
    {
        for (auto &entry : fs::directory_iterator(source))
        {
            fs::path path = entry.path();
            fs::path relative = fs::relative(path, PWD);
            if (std::regex_match(relative.string(), ignores))
            {
                continue;
            }

            if (fs::is_directory(path))
            {
                if (!fh::flatten(PWD, path, dest, ignores))
                    return false;
            }
            else if (fs::is_regular_file(path))
            {
                if (fh::has_changed(path))
                {
                    fs::remove(dest / path.filename());
                    std::cout << path.filename() << "\n";
                    fs::copy_file(path, dest / path.filename());
                }
            }
        }
        return true;
    }
    catch (const std::exception &e)
    {
        std::cout << color::red << "Error: " << color::white << e.what() << "\n";
        return false;
    }
}

bool fh::has_changed(fs::path path)
{
    auto temp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(std::filesystem::last_write_time(path) - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now());
    std::time_t last_modified = std::chrono::system_clock::to_time_t(temp);
    return (std::difftime(last_modified, prj::get_configs()["lastOut"]) > 0);
}