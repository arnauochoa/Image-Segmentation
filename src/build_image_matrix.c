//
// Created by Arnau Ochoa Bañuelos on 2019-11-28.
//

#include <printf.h>
#include <stdbool.h>
#include "build_image_matrix.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include "stb-master/stb_image_write.h"
#include "stb-master/stb_image.h"

int **buildImageMatrix(char *path) {
    int width, height, channels;
//    stbi_set_flip_vertically_on_load(true);

    unsigned char *img = stbi_load(path, &width, &height, &channels, 0);

    if (img == NULL) {
        printf("Error in loading the image\n");
        exit(1);
    }
    printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channels);


    // TODO: Build matrix

    stbi_image_free(img);
    return 0;
}


unsigned char *convertToGrayscale(unsigned char *img, int width, int height, int channels){
    // Allocate memory for greyscale image
    size_t img_size = (size_t) width * height * channels;
//    int gray_channels = channels == 4 ? 2 : 1;
    int gray_channels = 1;
    size_t gray_img_size = (size_t) width * height * gray_channels;
    unsigned char *gray_img = malloc(gray_img_size);
    if (gray_img == NULL) {
        printf("Unable to allocate memory for the gray image.\n");
        exit(1);
    }

    // Convert image to grayscale
    for (unsigned char *p = img, *pg = gray_img; p != img + img_size; p += channels, pg += gray_channels) {
        *pg = (uint8_t) ((*p + *(p + 1) + *(p + 2)) / 3.0);

//        Copy alpha value for grayscale + alpha
//        if (channels == 4) {
//            *(pg + 1) = *(p + 3);
//        }
    }

    return gray_img;
}