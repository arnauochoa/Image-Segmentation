#include "image.h"
#include <stdlib.h>
#include <stdio.h>


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

uint8_t *convertPixelsToGray(Image image){

    // Convert pixels of input image to gray
    size_t imageSize = image.width * image.height * NUM_CHANNELS;
    int grayChannels = 1;
    size_t grayImageSize = image.width * image.height * grayChannels;

    uint8_t *grayPixels = (uint8_t *) malloc(grayImageSize);

    if(grayPixels == NULL) {
        printf("Unable to allocate memory for the gray image.\n");
        exit(1);
    }

    for(uint8_t *p = image.pixels, *pg = grayPixels; p != image.pixels + imageSize; p += NUM_CHANNELS, pg += grayChannels) {
        *pg = (uint8_t)((*p + *(p + 1) + *(p + 2))/3.0);

    }

return grayPixels;
}

int *getPixelPosition(Image image, int i){

    int *pixelPosition[2];
    *pixelPosition[0] = i/image.width;
    *pixelPosition[1] = i%image.width;

    return *pixelPosition;
}