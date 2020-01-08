#include <stdio.h>
#include <stdlib.h>

#include "build_image_matrix.h"
#include "write_image_matrix.h"
#include <stdint.h>


int main() {
    char *path = "../resources/testBW_small.png";

    Image image = buildImage(path);

    writeImage(image);

    return EXIT_SUCCESS;
}