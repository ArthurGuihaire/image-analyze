#include <utils.h>
#include <stdint.h>
#include <constants.h>
//#include <stdio.h>

float min(float a, float b) {
    return (a<b) ? a : b;
}

float max(float a, float b) {
    return (a>b) ? a : b;
}

void calculateScreenCoords(uint32_t writeScreenCoords[4], float offsetX, float offsetY, int32_t framebufferWidth, int32_t framebufferHeight, float zoomFactor) {
    writeScreenCoords[0] = max(0, offsetX);
    writeScreenCoords[1] = max(0, offsetY);
    //writeScreenCoords[2] = minF((float)windowWidth, offsetX + windowWidth * zoomFactor);
    //writeScreenCoords[3] = minF((float)windowHeight, offsetY + windowHeight * zoomFactor);
    writeScreenCoords[2] = min((float)windowWidth, offsetX + framebufferWidth * zoomFactor);
    writeScreenCoords[3] = min((float)windowHeight, offsetY + framebufferHeight * zoomFactor);
}

void calculateFramebufferCoords(uint32_t writeFramebufferCoords[4], float offsetX, float offsetY, int32_t framebufferWidth, int32_t framebufferHeight, float zoomFactor) {
    //printf("Recieved parameters: offset (%d, %d), framebuffer (%d, %d), zoomFactor %f, window (%d, %d)\n", offsetX, offsetY, framebufferWidth, framebufferHeight, zoomFactor, windowWidth, windowHeight);
    writeFramebufferCoords[0] = max(0.0f, -offsetX / zoomFactor);
    writeFramebufferCoords[1] = max(0.0f, -offsetY / zoomFactor);
    writeFramebufferCoords[2] = min((float)windowWidth, (framebufferWidth - offsetX) / zoomFactor);
    writeFramebufferCoords[3] = min((float)windowHeight, (framebufferHeight - offsetY) / zoomFactor);
    //printf("Wrote framebuffer coords (%d, %d) to (%d, %d)\n", writeFramebufferCoords[0], writeFramebufferCoords[1], writeFramebufferCoords[2], writeFramebufferCoords[3]);
}

