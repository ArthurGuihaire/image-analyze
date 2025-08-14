#include "ImageAnalyzePCH.hpp"
#include "fileManager.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include "renderer.hpp"

int main() {
    // Init
    std::cout << "File path of image to load, or empty to load latest screenshot: ";
    std::string filepath;
    std::getline(std::cin, filepath);
    std::cout << "\n";

    std::filesystem::path imagePath;

    // If no filename specified, find the latest screenshot
    if (filepath.empty()) {
        std::cout << "File not specified, finding latest screenshot" << std::endl;
        imagePath = findLatestScreenshot(std::string(getenv("HOME")) + "/Pictures/Screenshots");
    }
    else {
        imagePath = std::filesystem::path(filepath);
    }
    
    //Create render window
    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode(sf::Vector2u(800, 600)), "Image Analyzer", sf::Style::Resize | sf::Style::Close);

    std::cout << "created window? " << std::endl;

    window.setVerticalSyncEnabled(true); // Enable vsync

    Renderer renderer(window, imagePath);

    while (window.isOpen()) {
        renderer.renderFrame();
    }
}