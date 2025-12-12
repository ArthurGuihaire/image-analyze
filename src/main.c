#include <stdio.h>
#include <renderer.h>
#include <inputHandler.h>
#include <filemanager.h>

int main(int argc, char* argv[]) {
    char* filepath;
    char buffer[256];
    if (argc == 2) {
        filepath = argv[1]; //just copy the poitner
    }
    else if (argc == 1) {
        fgets(buffer, 256, stdin);
        if (buffer[0] == '\n') {
            char expanded[256];
            expandTilde("~/Pictures/Screenshots", expanded, 256);
            int success = findLatestScreenshot(buffer, expanded);
            if (success != 0) {
                fprintf(stderr, "Failed to load %s\n", expanded);
                return 2;
            }
        }
        filepath = buffer;
    }
    else {
        fprintf(stderr, "Error: too many arguments\n");
        return 1;
    }

    Renderer renderer = initRenderer(filepath);
    initInput(&renderer);

    render(&renderer);
    while (!glfwWindowShouldClose(renderer.window)) {
        glfwWaitEvents();
        if (renderer.stateChanged) {
            updateCoords(&renderer);
            render(&renderer);
        }
    }
}
