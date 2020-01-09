#ifndef IMAGE_SEGMENTATION_IMAGE_H
#define IMAGE_SEGMENTATION_IMAGE_H

#include <stdint.h>


extern const unsigned int NUM_CHANNELS;

typedef struct {
    int height;
    int width;
    uint8_t *pixels;
} Image;

uint8_t *getPixel(Image image, int h, int w);

void fillPixelFromArray(Image image, int h, int w, uint8_t *arr);

Image fillPixel(Image image, int h, int w, uint8_t *pix);

Image convertImageToGrayScale(Image image);

int *getPixelPosition(Image image, int i);

#endif //IMAGE_SEGMENTATION_IMAGE_H
