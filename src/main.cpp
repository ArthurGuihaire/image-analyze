#include "constants.hpp"
#include "fileManager.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include "renderer.hpp"

int main(int argc, char* argv[]) {
    // Init
    std::string filepath;
    if (argc == 1) {
        std::cout << "File path of image to load, or empty to load latest screenshot: " << std::flush;
        std::getline(std::cin, filepath);
        std::cout << "\n";
    }
    else {
        filepath = argv[1];
    }

    namespace fs = std::filesystem;
    fs::path imagePath;

    // If no filename specified, find the latest screenshot
    if (filepath.empty()) {
        std::cout << "File not specified, finding latest screenshot" << std::endl;
        imagePath = findLatestScreenshot(std::string(getenv("HOME")) + "/Pictures/Screenshots");
    }
    else {
        imagePath = fs::path(filepath);
    }

    std::cout << "Loading font " << std::string(fontPath) << std::endl;
    
    //Create render window
    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode(sf::Vector2u(startingWidth, startingHeight)), "Image Analyzer", sf::Style::Resize | sf::Style::Close);

    window.setVerticalSyncEnabled(true); // Enable vsync

    Renderer renderer(window, imagePath, fontPath);

    while (window.isOpen()) {
        renderer.renderFrame();
    }
}