#include <inputHandler.h>
#include <GLFW/glfw3.h>
#include <utils.h>
#include <constants.h>
//#include <stdio.h>

static userPointer ptr;

static void updateCoords(Renderer* renderer) {
    calculateFramebufferCoords(renderer->fbCoords, renderer->xOffset, renderer->yOffset, renderer->framebufferWidth, renderer->framebufferHeight, renderer->zoomFactor);
    calculateScreenCoords(renderer->screenCoords, renderer->xOffset, renderer->yOffset, renderer->framebufferWidth, renderer->framebufferHeight, renderer->zoomFactor);

    //printf("Offset: (%d, %d)\n", renderer->xOffset, renderer->yOffset);
    //printf("Screen coords: From (%d, %d) to (%d, %d)\nFramebuffer coords: From (%d, %d) to (%d, %d)\n", renderer->screenCoords[0], renderer->screenCoords[1], renderer->screenCoords[2], renderer->screenCoords[3], renderer->fbCoords[0], renderer->fbCoords[1], renderer->fbCoords[2], renderer->fbCoords[3]);
}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
    userPointer* ptr = glfwGetWindowUserPointer(window);
    Renderer* renderer = ptr->renderer;

    double mousePosX, mousePosY;
    glfwGetCursorPos(window, &mousePosX, &mousePosY);
    mousePosY = windowHeight - mousePosY;
    float mouseOffsetX = mousePosX - renderer->xOffset;
    float mouseOffsetY = mousePosY - renderer->yOffset; 

    float zoomOffsetDiffX = mouseOffsetX * yOffset * 0.05;
    float zoomOffsetDiffY = mouseOffsetY * yOffset * 0.05;

    renderer->zoomFactor += yOffset * 0.05 * renderer->zoomFactor;

    renderer->xOffset -= zoomOffsetDiffX;
    renderer->yOffset -= zoomOffsetDiffY;

    updateCoords(renderer);
    //printf("Offset: (%d, %d)\n", renderer->xOffset, renderer->yOffset);
    //printf("Bottom left: (%d, %d), Zoom offset: (%f, %f), Mouse offset: (%d, %d), diff: (%d, %d)\n", bottomLeftX, bottomLeftY, mousePosX, mousePosY, mouseOffsetX, mouseOffsetY, zoomOffsetDiffX, zoomOffsetDiffY);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    windowWidth = width;
    windowHeight = height;

    userPointer* ptr = glfwGetWindowUserPointer(window);
    Renderer* renderer = ptr->renderer;

    updateCoords(renderer);
}

void mousePositionCallback(GLFWwindow* window, double xPos, double yPos) {
    int mouseState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    userPointer* ptr = glfwGetWindowUserPointer(window);

    if (mouseState == GLFW_PRESS) {
        Renderer* renderer = ptr->renderer;
        renderer->xOffset += xPos - ptr->mouseX;
        renderer->yOffset -= yPos - ptr->mouseY;

        updateCoords(renderer);
    }
    ptr->mouseX = xPos;
    ptr->mouseY = yPos;
}

void initInput(Renderer* renderer) {
    glfwSetWindowUserPointer(renderer->window, &ptr);
    ptr.renderer = renderer;
    double mouseX, mouseY;
    glfwGetCursorPos(renderer->window, &mouseX, &mouseY);
    ptr.mouseX = mouseX;
    ptr.mouseY = mouseY;
    glfwSetCursorPosCallback(renderer->window, mousePositionCallback);
    glfwSetScrollCallback(renderer->window, scrollCallback);
}

