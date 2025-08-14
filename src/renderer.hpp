#include "ImageAnalyzePCH.hpp"
class Renderer {
    private:
        sf::View screenView;
        sf::View worldView;
        sf::Texture texture;
        sf::RenderWindow& windowReference;
        sf::Sprite textureSprite;
        sf::VertexArray line;
        sf::Vector2f windowCenter;
        sf::Vector2i mousePosition;
        float currentZoom;
        bool mouseDown;
        bool shiftPressed;
        bool mouseDragged;
        bool lineVisible;
    public:
        Renderer(sf::RenderWindow& window, const std::filesystem::path& texturePath);
        void renderFrame();
        void changeZoom(const sf::Vector2i& mousePosition, const float zoomMultiplier);
        void handleInput(const sf::Event& event);
};