#include <glad/gl.h>
#include <renderer.h>
#include <initializer.h>
#include <constants.h>
#include <stb_image.h>
#include <utils.h>
#include <pthread.h>
#include <time.h>

typedef struct {
    const char* path;
    int* width;
    int* height;
    int* nrChannels;
} imageLoadArgs;

void* loadImageThread(void* argStruct) {
    clock_t start = clock();
    imageLoadArgs* args = argStruct;
    stbi_set_flip_vertically_on_load(true);
    printf("Path: %s\n", args->path);
    unsigned char* data = stbi_load(args->path, args->width, args->height, args->nrChannels, 0);
    if (!data) {
        fprintf(stderr, "Error: failed to load texture\n");
        return NULL;
    }
    clock_t end = clock();
    double ms = (double)(end-start) * 1000 / CLOCKS_PER_SEC;
    printf("Loading the image (parallel with context creation) took %f ms\n", ms);
    return data;
}

Renderer initRenderer(const char* path) {
    //Start decoding image in parallel with opengl init stuff
    pthread_t imgLoadThread;
    int fbWidth, fbHeight, nrChannels;
    imageLoadArgs args = {path, &fbWidth, &fbHeight, &nrChannels};
    pthread_create(&imgLoadThread, NULL, loadImageThread, &args);
 
    clock_t start = clock();
    initGLFW(3, 1);
    clock_t end = clock();
    double ms = (double)(end-start) * 1000 / CLOCKS_PER_SEC;
    printf("InitGLFW took %f ms\n", ms);

    start = clock();
    int imageWidth, imageHeight;
    {
        int channels;
        stbi_info(path, &imageWidth, &imageHeight, &channels);
        printf("Loading %s\n", path);
        printf("Detected image dimensions imageWidth=%d imageHeight=%d\n", imageWidth, imageHeight);
        windowWidth = imageWidth;
        windowHeight = imageHeight;
    }
    end = clock();
    ms = (double)(end-start) * 1000 / CLOCKS_PER_SEC;
    printf("Image info took %f ms\n", ms);

    start = clock();
    GLFWwindow* window = createWindow(false);
    end = clock();
    ms = (double)(end-start) * 1000 / CLOCKS_PER_SEC;
    printf("Context and Window creation took %f ms\n", ms);
    start = clock();
    initGLAD(); 
    end = clock();
    ms = (double)(end-start) * 1000 / CLOCKS_PER_SEC;
    printf("Loading GLAD took %f ms\n", ms);

    start = clock();
    Renderer renderer = {
        window, 0, 0, 1.0f, 0.0f, 0.0f, imageWidth, imageHeight, {0, 0, windowWidth, windowHeight}, {0, 0, windowWidth, windowHeight}
    };

    unsigned char* data;
    pthread_join(imgLoadThread, (void**)&data);

    renderer.textureId = loadTextureFramebuffer(data, &(renderer.framebufferId), fbWidth, fbHeight, nrChannels);
    if (!(renderer.textureId)) {
        fprintf(stderr, "Error: Loading the OpenGL texture failed!\n");
        exit(3);
    }

    glBindFramebuffer(GL_READ_FRAMEBUFFER, renderer.framebufferId);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    end = clock();
    ms = (double)(end-start) * 1000 / CLOCKS_PER_SEC;
    printf("End took %f ms\n", ms);

    return renderer;
}

void render(Renderer* renderer) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBlitFramebuffer(renderer->fbCoords[0], renderer->fbCoords[1], renderer->fbCoords[2], renderer->fbCoords[3], renderer->screenCoords[0], renderer->screenCoords[1], renderer->screenCoords[2], renderer->screenCoords[3], GL_COLOR_BUFFER_BIT, GL_NEAREST);

    glfwSwapBuffers(renderer->window);
    glfwPollEvents();
}

