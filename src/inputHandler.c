#include <inputHandler.h>
#include <GLFW/glfw3.h>
#include <utils.h>
#include <constants.h>
//#include <stdio.h>

static userPointer ptr;

void updateCoords(Renderer* renderer) {
    calculateFramebufferCoords(renderer->fbCoords, renderer->xOffset, renderer->yOffset, renderer->framebufferWidth, renderer->framebufferHeight, renderer->zoomFactor);
    calculateScreenCoords(renderer->screenCoords, renderer->xOffset, renderer->yOffset, renderer->framebufferWidth, renderer->framebufferHeight, renderer->zoomFactor);
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

    renderer->stateChanged = true;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

    windowWidth = width;
    windowHeight = height;

    userPointer* ptr = glfwGetWindowUserPointer(window);
    Renderer* renderer = ptr->renderer;

    renderer->stateChanged = true;
}

void mousePositionCallback(GLFWwindow* window, double xPos, double yPos) {
    int mouseState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    userPointer* ptr = glfwGetWindowUserPointer(window);

    if (mouseState == GLFW_PRESS) {
        Renderer* renderer = ptr->renderer;
        renderer->xOffset += xPos - ptr->mouseX;
        renderer->yOffset -= yPos - ptr->mouseY;

        renderer->stateChanged = true;
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
    glfwSetFramebufferSizeCallback(renderer->window, framebufferSizeCallback);
}

