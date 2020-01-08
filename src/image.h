//
// Created by cttc on 10/12/19.
//

#ifndef IMAGE_SEGMENTATION_IMAGE_H
#define IMAGE_SEGMENTATION_IMAGE_H

//#include <cxxabi.h>
#include <stdint.h>


extern const int NUM_CHANNELS;

typedef struct {
    int height;
    int width;
    uint8_t *pixels;
} Image;

uint8_t *getPixel(Image img, int h, int w);
void fillPixel(Image image, int h, int w, int *p);

#endif //IMAGE_SEGMENTATION_IMAGE_H
