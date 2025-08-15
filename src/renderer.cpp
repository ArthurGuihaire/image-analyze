#include "renderer.hpp"
#include "constants.hpp"
#include <cmath>

Renderer::Renderer(sf::RenderWindow& window, const std::filesystem::path& texturePath)
    : windowReference(window), textureSprite(texture), line(sf::PrimitiveType::Lines, 2), windowCenter(sf::Vector2i(window.getSize()) / 2), mouseDown(false), shiftPressed(false), numSetPoints(0)
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
    line[0].color = sf::Color::Red;
    line[1].color = sf::Color::Red;
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
            line[1].position = windowReference.mapPixelToCoords(mouseMove->position, worldView);
            numSetPoints = 2;
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
                    std::cout << "Mouse clicked at pixel position " << truncf(worldPosition.x) << ", " << truncf(worldPosition.y) << std::endl;
                }
            }
            else if (numSetPoints == 2) {
                line[1].position = windowReference.mapPixelToCoords(MouseReleased->position);
                numSetPoints = 3;
                std::cout << "Line goes from " << truncf(line[0].position.x) << ", " << truncf(line[0].position.y) << " to " << truncf(line[1].position.x) << ", " << truncf(line[1].position.y) << " so dimensions are " << std::abs((truncf(line[1].position.x)) - truncf(line[0].position.x)) << ", " << std::abs(truncf(line[1].position.y) - truncf(line[0].position.y)) << std::endl;
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
        screenView.setSize(sf::Vector2f(Resized->size));
        worldView.zoom(currentZoom);
        windowCenter = sf::Vector2f(windowReference.getSize()) / 2.0f;
    }
    else if (event.is<sf::Event::Closed>())
        windowReference.close();
}