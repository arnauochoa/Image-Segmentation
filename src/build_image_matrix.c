//
// Created by Arnau Ochoa Bañuelos on 2019-11-28.
//

#include <printf.h>
#include <stdbool.h>
#include "build_image_matrix.h"
#include "image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include "stb-master/stb_image_write.h"
#include "stb-master/stb_image.h"

Image buildImage(char *path) {
    Image image;
    int channels;
    int *img = (int *) stbi_load(path, &image.width, &image.height, &channels, NUM_CHANNELS);
                                //file path, width and height of the image, the number of color channels
                                // if R,G,B -> channels = 3 (ignoring transparency)
                                // if R,G,B,alpha -> 4 channels
                                // last parameter = 0, then we are loading the image as it is (with 4 channels)

    if (img == NULL) {
        printf("Error in loading the image\n");
        exit(1);
    }
    printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", image.width, image.height,
           channels);
    if (channels < NUM_CHANNELS) {
        printf("Error: Image has less than %d channels", NUM_CHANNELS);
        exit(1);
    }

    size_t imageSize = (size_t) (image.width * image.height * NUM_CHANNELS);
    image.pixels = (uint8_t *) malloc(imageSize);

    int *p = img;
    for (int i = 0; i < image.height; i++) {
        for (int j = 0; j < image.width; j++) {
            fillPixel(image, i, j, p);
            uint8_t *pix = getPixel(image, i, j);

            printf("(%d %d %d)\t\t", pix[0], pix[1], pix[2]); // Print for checking
            p++;
        }
        printf("\n");
    }

//    stbi_image_free(img);
    return image;
}
