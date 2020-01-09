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
    uint8_t *img = (uint8_t *) stbi_load(path, &image.width, &image.height, &channels, NUM_CHANNELS);

    if (img == NULL) {
        printf("Error in loading the image\n");
        exit(1);
    }
    printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n\n", image.width, image.height,
           channels);
    if (channels < NUM_CHANNELS) {
        printf("Error: Image has less than %d channels", NUM_CHANNELS);
        exit(1);
    }

    size_t imageSize = image.width * image.height * NUM_CHANNELS;

    image.pixels = (uint8_t *) malloc(imageSize);

    uint8_t *p = img;
    for (int i = 0; i < image.height; i++) {
        for (int j = 0; j < image.width; j++) {
            fillPixelFromArray(image, i, j, p);
//            uint8_t *pix = getPixel(image, i, j);

            //printf("(%d %d %d)\t\t", pix[0], pix[1], pix[2]); // Print for checking
        }
        //printf("\n");
    }

    return image;
}
