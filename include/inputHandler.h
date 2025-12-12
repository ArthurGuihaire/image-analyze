#pragma once
#include <renderer.h>

typedef struct {
    Renderer* renderer;
    float mouseX;
    float mouseY;
} userPointer;

void initInput(Renderer* renderer);
void updateCoords(Renderer* renderer);
