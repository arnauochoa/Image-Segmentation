//
// Created by cttc on 10/12/19.
//

#ifndef IMAGE_SEGMENTATION_IMAGE_H
#define IMAGE_SEGMENTATION_IMAGE_H

extern const int NUM_CHANNELS;

typedef struct {
    int height;
    int width;
    int *pixels;
} Image;

int *getPixel(Image img, int h, int w);

#endif //IMAGE_SEGMENTATION_IMAGE_H
