//
// Created by cttc on 18/12/19.
//

#include <sys/stat.h>
#include <string.h>
#include "write_image_matrix.h"
#include "image.h"
#include "stb-master/stb_image_write.h"
#include "stb-master/stb_image.h"

int writeImage(Image img) {
    int size = img.width * NUM_CHANNELS;
    char *saveDirectory = "../results/";

    // Check if directory exists. If not, create it.
    struct stat st = {0};
    if (stat(saveDirectory, &st) == -1) {
        mkdir(saveDirectory, 0700);
    }

    stbi_write_png("../results/output_image.png", img.width, img.height, NUM_CHANNELS, img.pixels, size);

    return 0;
}