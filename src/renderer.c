#include <renderer.h>
#include <initializer.h>
#include <stb_image.h>

Renderer initRenderer() {
    
}

void loadImage(const char* imageName) {
    int width, height, channels;
    unsigned char* img = stbi_load(imageName, &width, &height, &channels, 0);
}
