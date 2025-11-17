#pragma once
#include <GLFW/glfw3.h>
#include <stdbool.h>

void initGLFW(int majorVersion, int minorVersion);
void initGLAD();
GLFWwindow* createWindow(bool fullscreen);
uint32_t loadTextureFramebuffer(unsigned char* data, uint32_t* writeFramebuffer, int width, int height, int nrChannels);
