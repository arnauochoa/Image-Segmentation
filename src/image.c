#include "image.h"
#include <stdlib.h>


const unsigned int NUM_CHANNELS = 3;

uint8_t *getPixel(Image image, int h, int w){
    uint8_t *pixel = (uint8_t *) malloc(NUM_CHANNELS * sizeof(int));
    int pixelPos = (h * image.width * NUM_CHANNELS) + (w * NUM_CHANNELS);
    pixel[0] = image.pixels[pixelPos];
    pixel[1] = image.pixels[pixelPos + 1];
    pixel[2] = image.pixels[pixelPos + 2];
    return pixel;
}

void fillPixel(Image image, int h, int w, uint8_t *p){
    int pixelPos = (h * image.width * NUM_CHANNELS) + (w * NUM_CHANNELS);
    image.pixels[pixelPos] = p[pixelPos];
    image.pixels[pixelPos + 1] = p[pixelPos + 1];
    image.pixels[pixelPos + 2] = p[pixelPos + 2];
}