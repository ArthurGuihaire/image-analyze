#include <filesystem>
std::filesystem::path findLatestScreenshot(const std::filesystem::path& directory);

#ifdef APP_INSTALLED_DATA_DIR
const inline std::filesystem::path fontPath(std::filesystem::path(APP_INSTALLED_DATA_DIR) / "roboto.ttf");
#else
#ifdef APP_BUILD_DATA_DIR
const inline std::filesystem::path fontPath(std::filesystem::path(APP_BUILD_DATA_DIR));
#else
const inline std::filesystem::path fontPath(std::string(getenv("HOME")) + "/projects/image-analyze/build/roboto.ttf");
#endif
#endif