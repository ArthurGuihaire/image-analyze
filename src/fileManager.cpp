#include "fileManager.hpp"
std::filesystem::path findLatestScreenshot(const std::filesystem::path& directory)
{
    std::cout << "looking for latest file..." << std::endl;
    namespace fs = std::filesystem;
    fs::path latestFile;
    fs::file_time_type latestTime = fs::file_time_type::min();

    for (const auto& entry : fs::directory_iterator(directory))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".png")
        {
            auto modTime = fs::last_write_time(entry);
            if (modTime > latestTime)
            {
                latestTime = modTime;
                latestFile = entry.path();
            }
        }
    }

    if (latestFile.empty()) { return {}; }
    return latestFile.string();
}