#include <stdio.h>
#include <stdlib.h>

#include "build_image_matrix.h"
#include "write_image_matrix.h"


int main() {
    char *path = "../resources/testBW.png";

    Image image = buildImage(path);
    writeImage(image);

    return EXIT_SUCCESS;
}