//
// Created by cttc on 10/12/19.
//

#include "image.h"
#include <stdlib.h>


const int NUM_CHANNELS = 3;

uint8_t *getPixel(Image img, int h, int w){
    uint8_t *pixel = (uint8_t *) malloc(NUM_CHANNELS * sizeof(int));
    pixel[0] = img.pixels[w * img.width + h];
    pixel[1] = img.pixels[w * img.width + h + 1];
    pixel[2] = img.pixels[w * img.width + h + 2];
    return pixel;
}

void fillPixel(Image image, int h, int w, int *p){
    image.pixels[w*image.width + h] = (uint8_t) p[h * image.width + w * NUM_CHANNELS];
    image.pixels[w*image.width + h + 1] = (uint8_t) p[h * image.width + w * NUM_CHANNELS + 1];
    image.pixels[w*image.width + h + 2] = (uint8_t) p[h * image.width + w * NUM_CHANNELS + 2];
}