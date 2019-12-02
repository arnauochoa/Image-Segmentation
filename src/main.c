#include <stdio.h>
#include <stdlib.h>
#include "build_image_matrix.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include "stb-master/stb_image_write.h"
#include "stb-master/stb_image.h"



int main() {
    char *path = "resources/testBW.png";
    int x;
    int y;
    int n;
    helloWorld();
    unsigned char *data = stbi_load(path, &x, &y, &n, 0);
    return EXIT_SUCCESS;
}