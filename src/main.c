#include <stdio.h>
#include <stdlib.h>

#include "build_image_matrix.h"



int main() {
    char *path = "../resources/testBW.png";

    Image image = buildImage(path);


    return EXIT_SUCCESS;
}