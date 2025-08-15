#include "renderer.hpp"
#include "constants.hpp"
#include <SFML/Window/WindowEnums.hpp>
#include <cmath>
#include <string>

Renderer::Renderer(sf::RenderWindow& window, const std::filesystem::path& texturePath, const std::filesystem::path& fontPath)
    : windowReference(window), textureSprite(texture), line(sf::PrimitiveType::LineStrip, 5), infoText(infoTextFont), infoTextFont(fontPath), windowCenter(sf::Vector2i(window.getSize()) / 2), mouseDown(false), shiftPressed(false), numSetPoints(0)
{
    screenView = sf::View(sf::Vector2f(windowCenter), sf::Vector2f(startingWidth, startingHeight));
    worldView = sf::View(sf::Vector2f(windowCenter), sf::Vector2f(startingWidth, startingHeight));
    windowReference.setView(worldView);
    const bool imageIsLoaded = texture.loadFromFile(texturePath);
    if (!imageIsLoaded) {
        std::cerr << "Texture failed to load" << std::endl;
        std::exit(1);
    }
    windowCenter = sf::Vector2f(windowReference.getSize()) / 2.0f;
    textureSprite.setTexture(texture, true);
    currentZoom = 1.0f;
    for (unsigned char i = 0; i < 5; i++) {
        line[i].color = sf::Color::Red;
    }
}

void Renderer::renderFrame() {
    while (const auto eventPointer = windowReference.pollEvent()) {
        handleInput(*eventPointer);
    }
    windowReference.clear();

    windowReference.setView(worldView);
    windowReference.draw(textureSprite);
    if (numSetPoints == 2 || numSetPoints == 3)
        windowReference.draw(line);

    windowReference.setView(screenView);
    windowReference.draw(infoText);

    windowReference.display();
}

void Renderer::changeZoom(const sf::Vector2i& currentMousePosition, const float zoomIncrease) {
    const float zoomMultiplier = zoomIncrease + 1.0f;
    const sf::Vector2f offsetPixels = windowCenter - sf::Vector2f(currentMousePosition);
    const sf::Vector2f moveOffset = offsetPixels * zoomIncrease * currentZoom;
    worldView.move(moveOffset);
    worldView.zoom(zoomMultiplier);
    currentZoom *= zoomMultiplier;
}

void Renderer::handleInput(const sf::Event& event) {
    if (const auto mouseMove = event.getIf<sf::Event::MouseMoved>()) {
        if (shiftPressed && (numSetPoints == 1 || numSetPoints == 2)) {
            line[2].position = windowReference.mapPixelToCoords(mouseMove->position, worldView);
            numSetPoints = 2;
            line[1].position = sf::Vector2f(line[2].position.x, line[0].position.y);
            line[3].position = sf::Vector2f(line[0].position.x, line[2].position.y);
        }
        else if (mouseDown) {
            worldView.move(sf::Vector2f(mousePosition - (mouseMove->position)) * currentZoom);
            mouseDragged = true;
        }
        mousePosition = mouseMove->position;
    }
    else if(const auto mouseScroll = event.getIf<sf::Event::MouseWheelScrolled>()) {
        this->changeZoom(mouseScroll->position, (mouseScroll->delta) * scrollSpeed);
    }
    else if(const auto mouseClicked = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseClicked->button == sf::Mouse::Button::Left) {
            mouseDown = true;
            mouseDragged = false;
            if (shiftPressed) {
                if (numSetPoints == 0 || numSetPoints == 3) {
                    line[0].position = windowReference.mapPixelToCoords(mouseClicked->position, worldView);
                    line[4].position = line[0].position;
                    numSetPoints = 1;
                }
            }
        }
    }
    else if(const auto MouseReleased = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (MouseReleased->button == sf::Mouse::Button::Left) {
            mouseDown = false;
            if (!shiftPressed) {
                if (!mouseDragged) {
                    sf::Vector2f worldPosition = windowReference.mapPixelToCoords(MouseReleased->position, worldView);
                    std::string infoTextString = std::string("Mouse clicked at pixel position ") + std::to_string(static_cast<int>(worldPosition.x)) + ", " + std::to_string(static_cast<int>(worldPosition.y));
                    infoText.setString(infoTextString);
                    std::cout << infoTextString << std::endl;
                }
            }
            else if (numSetPoints == 2) {
                line[2].position = windowReference.mapPixelToCoords(MouseReleased->position, worldView);
                numSetPoints = 3;
                line[1].position = sf::Vector2f(line[2].position.x, line[0].position.y);
                line[3].position = sf::Vector2f(line[0].position.x, line[2].position.y);
                std::string infoTextString = std::string("Line goes from ") + std::to_string(static_cast<int>(line[0].position.x)) + ", " + std::to_string(static_cast<int>(line[0].position.y)) + " to " + std::to_string(static_cast<int>(line[2].position.x)) + ", " + std::to_string(static_cast<int>(line[2].position.y)) + " so dimensions are " + std::to_string(std::abs((static_cast<int>(line[2].position.x)) - static_cast<int>(line[0].position.x))) + ", " + std::to_string(std::abs(static_cast<int>(line[2].position.y) - static_cast<int>(line[0].position.y)));
                infoText.setString(infoTextString);
                std::cout << infoTextString << std::endl;
            }
        }
    }
    else if(const auto keyPress = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPress->code == sf::Keyboard::Key::LShift ||keyPress->code == sf::Keyboard::Key::RShift) {
            shiftPressed = true;
        }
    }
    else if(const auto keyReleased = event.getIf<sf::Event::KeyReleased>()) {
        if (keyReleased->code == sf::Keyboard::Key::LShift ||keyReleased->code == sf::Keyboard::Key::RShift)
            shiftPressed = false;
    }
    else if(const auto Resized = event.getIf<sf::Event::Resized>()) {
        worldView.setSize(sf::Vector2f(Resized->size));
        worldView.zoom(currentZoom);
        std::cout << "changed default view" << std::endl;
        windowCenter = sf::Vector2f(windowReference.getSize()) / 2.0f;
        screenView.setSize(sf::Vector2f(windowReference.getSize()));
        screenView.setCenter(windowCenter);
    }
    else if (event.is<sf::Event::Closed>())
        windowReference.close();
}