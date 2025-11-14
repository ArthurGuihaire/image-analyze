#include <glad/gl.h>
#include <renderer.h>
#include <inputHandler.h>
#include <initializer.h>
#include <constants.h>
#include <stb_image.h>
#include <utils.h>

Renderer initRenderer(const char* path) {
    initGLFW(3, 1);

    {
        int width, height, channels;
        stbi_info(path, &width, &height, &channels);
        printf("Loading %s\n", path);
        printf("Detected image dimensions width=%d height=%d\n", width, height);
        windowWidth = width;
        windowHeight = height;
    }   

    GLFWwindow* window = createWindow(false);
    initGLAD();

    Renderer renderer = {
        window, 0, 0, 1.0f, 0.0f, 0.0f, windowWidth, windowHeight, {0, 0, windowWidth, windowHeight}, {0, 0, windowWidth, windowHeight}
    };

    renderer.framebufferWidth = windowWidth;
    renderer.framebufferHeight = windowHeight;
    renderer.textureId = loadTextureFramebuffer(path, &(renderer.framebufferId));
    if (!(renderer.textureId)) {
        exit(3);
    }

    glBindFramebuffer(GL_READ_FRAMEBUFFER, renderer.framebufferId);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    return renderer;
}

void render(Renderer* renderer) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //calculateScreenCoords(renderer->screenCoords, renderer->xOffset, renderer->yOffset, renderer->framebufferWidth, renderer->framebufferHeight, renderer->zoomFactor);
    //calculateFramebufferCoords(renderer->fbCoords, renderer->xOffset, renderer->yOffset, renderer->framebufferWidth, renderer->framebufferHeight, renderer->zoomFactor);
    
    //printf("Width: %d, height: %d, xOffset: %d, yOffset: %d\n", windowWidth, windowHeight, renderer->xOffset, renderer->yOffset);

    glBlitFramebuffer(renderer->fbCoords[0], renderer->fbCoords[1], renderer->fbCoords[2], renderer->fbCoords[3], renderer->screenCoords[0], renderer->screenCoords[1], renderer->screenCoords[2], renderer->screenCoords[3], GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glfwSwapBuffers(renderer->window);
    glfwPollEvents();
}

