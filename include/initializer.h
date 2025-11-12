#pragma once
#include <GLFW/glfw3.h>
#include <stdbool.h>

void setWindowSize(int width, int height);
void initGLFW(int majorVersion, int minorVersion);
void initGLAD();
GLFWwindow* createWindow(bool fullscreen);

