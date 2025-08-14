#include "renderer.hpp"
#include "constants.hpp"
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>
#include <cstdlib>
#include <sys/types.h>

Renderer::Renderer(sf::RenderWindow& window, const std::filesystem::path& texturePath)
    : windowReference(window), textureSprite(texture), windowCenter(sf::Vector2i(window.getSize()) / 2), line(sf::PrimitiveType::Lines, 2), lineVisible(false)
{
    screenView = sf::View(sf::Vector2f(windowCenter), sf::Vector2f(startingWidth, startingHeight));
    worldView = sf::View(sf::Vector2f(windowCenter), sf::Vector2f(startingWidth, startingHeight));
    windowReference.setView(worldView);
    const bool imageIsLoaded = texture.loadFromFile(texturePath);
    if (!imageIsLoaded) {
        std::cerr << "Texture failed to load" << std::endl;
        std::exit(1);
    }
    textureSprite.setTexture(texture, true);
    currentZoom = 1.0f;
    windowCenter = sf::Vector2f(windowReference.getSize()) / 2.0f;
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
        std::cout << "Mouse moved" << std::endl;
        if (mouseDown) {
            worldView.move(sf::Vector2f(mousePosition - (mouseMove->position)) * currentZoom);
            mouseDragged = true;
        }
        mousePosition = mouseMove->position;
    }
    else if(const auto mouseScroll = event.getIf<sf::Event::MouseWheelScrolled>()) {
        this->changeZoom(mouseScroll->position, (mouseScroll->delta) * scrollSpeed);
    }
    else if(const auto MouseClicked = event.getIf<sf::Event::MouseButtonPressed>()) {
        std::cout << "Mouse pressed" << std::endl;
        if (MouseClicked->button == sf::Mouse::Button::Left) {
            mouseDown = true;
            mouseDragged = false;
            line[0].position = sf::Vector2f(MouseClicked->position);
        }
    }
    else if(const auto MouseReleased = event.getIf<sf::Event::MouseButtonReleased>()) {
        std::cout << "Mouse released" << std::endl;
        if (MouseReleased->button == sf::Mouse::Button::Left) {
            mouseDown = false;
            if (!mouseDragged) {

            }
        }
    }
    else if(const auto keyPress = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPress->code == sf::Keyboard::Key::LShift ||keyPress->code == sf::Keyboard::Key::RShift)
            shiftPressed = true;
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