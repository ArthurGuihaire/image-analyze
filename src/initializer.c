#include <glad/gl.h>
#include <initializer.h>
#include <constants.h>
#include <stb_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint32_t windowWidth;
uint32_t windowHeight;

void initGLFW(int majorVersion, int minorVersion) {
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);
    //glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
    glfwInitHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVersion);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
}

GLFWwindow* createWindow(bool fullscreen) {
    GLFWmonitor* mon = glfwGetPrimaryMonitor();
    const GLFWvidmode* vidMode = glfwGetVideoMode(mon);
    GLFWwindow* window;
    if (fullscreen) {
        window = glfwCreateWindow(vidMode->width, vidMode->height, "OpenGL", mon, NULL);
        windowWidth = vidMode->width;
        windowHeight = vidMode->height;
    }
    else {
        windowWidth = (windowWidth > vidMode->width) ? vidMode->width : windowWidth;
        windowHeight = (windowHeight > vidMode->height) ? vidMode->height : windowHeight;
        window = glfwCreateWindow(windowWidth, windowHeight, "OpenGL", NULL, NULL);
    }
    if (window == NULL)
    {
        fprintf(stderr, "Failed to create GLFW window\n");
        printf("Width: %d, Height: %d\n", windowWidth, windowHeight);
        glfwTerminate();
        exit (2);
    }
    //Create an OpenGL contex for the window
    glfwSwapInterval(1); //Limit framerate
    glfwMakeContextCurrent(window);
    return window;
}

void initGLAD() {
    //if (!gladLoaderLoadGL(glfwGetProcAddress))
    //{
    //    fprintf(stderr, "Failed to initialize GLAD\n");
    //    return;
    //}
    gladSetGLOnDemandLoader((GLADloadfunc)glfwGetProcAddress);
    if (glGetString == NULL) {
        fprintf(stderr, "Failed to load GL on-demand functions!\n");
        exit(1);
    }
    glViewport(0, 0, windowWidth, windowHeight);
}

uint32_t loadTextureFramebuffer(unsigned char* data, uint32_t* writeFramebuffer, int width, int height, int nrChannels) {
    GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;

    uint32_t textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Fix alignment for non-4-byte row size
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    glGenFramebuffers(1, writeFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, *writeFramebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    stbi_image_free(data);
    return textureID;
}
