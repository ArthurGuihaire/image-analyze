#include <utils.h>
#include <stdint.h>
#include <constants.h>
#include <stdio.h>

float min(float a, float b) {
    return (a<b) ? a : b;
}

float max(float a, float b) {
    return (a>b) ? a : b;
}


void calculateScreenCoords(uint32_t writeScreenCoords[4], float xOffset, float yOffset, int32_t framebufferWidth, int32_t framebufferHeight, float zoomFactor) {
    writeScreenCoords[0] = max(0, xOffset);
    writeScreenCoords[1] = max(0, yOffset);
    writeScreenCoords[2] = min((float)windowWidth, xOffset + framebufferWidth * zoomFactor);
    writeScreenCoords[3] = min((float)windowHeight, yOffset + framebufferHeight * zoomFactor);
}

void calculateFramebufferCoords(uint32_t writeFramebufferCoords[4], float xOffset, float yOffset, int32_t framebufferWidth, int32_t framebufferHeight, float zoomFactor) {
    writeFramebufferCoords[0] = max(0.0f, -xOffset / zoomFactor);
    writeFramebufferCoords[1] = max(0.0f, -yOffset / zoomFactor);
    writeFramebufferCoords[2] = min((float)framebufferWidth, (windowWidth - xOffset) / zoomFactor);
    writeFramebufferCoords[3] = min((float)framebufferHeight, (windowHeight - yOffset) / zoomFactor);
}

