#include "ImageAnalyzePCH.hpp"
class Renderer {
    private:
        sf::View screenView;
        sf::View worldView;
        sf::Texture texture;
        sf::RenderWindow& windowReference;
        sf::Sprite textureSprite;
        sf::VertexArray line;
        sf::Text infoText;
        sf::Font infoTextFont;
        sf::Vector2f windowCenter;
        sf::Vector2i mousePosition;
        float currentZoom;
        bool mouseDown;
        bool shiftPressed;
        bool mouseDragged;
        char numSetPoints;
    public:
        Renderer(sf::RenderWindow& window, const std::filesystem::path& texturePath, const std::filesystem::path& fontPath);
        void renderFrame();
        void changeZoom(const sf::Vector2i& mousePosition, const float zoomMultiplier);
        void handleInput(const sf::Event& event);
};