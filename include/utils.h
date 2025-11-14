#pragma once
#include <stdint.h>
inline float min(float a, float b);
inline float max(float a, float b);
void calculateFramebufferCoords(uint32_t writeFramebufferCoords[4], float offsetX, float offsetY, int32_t framebufferWidth, int32_t framebufferHeight, float zoomFactor);
void calculateScreenCoords(uint32_t writeScreenCoords[4], float offsetX, float offsetY, int32_t framebufferWidth, int32_t framebufferHeight, float zoomFactor);


