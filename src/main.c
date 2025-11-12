#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    char* filepath;
    char buffer[256];
    if (argc == 2) {
        filepath = argv[1]; //just copy the poitner
    }
    else if (argc == 1) {
        fgets(buffer, 256, stdin);
        filepath = buffer;
    }
    else {
        fprintf(stderr, "Error: too many arguments\n");
        return 1;
    }

    FILE* fp;
    if ((fp = fopen(filepath, "r")) == NULL) {
        fprintf(stderr, "Error: file %s cannot be opened\n", filepath);
        return 2;
    }

    
}
