#include "image.h"
#include <stdlib.h>
#include <stdio.h>


const unsigned int NUM_CHANNELS = 3;

uint8_t *getPixel(Image image, int h, int w) {
    uint8_t *pixel = (uint8_t *) malloc(NUM_CHANNELS * sizeof(int));
    int pixelPos = (h * image.width * NUM_CHANNELS) + (w * NUM_CHANNELS);
    pixel[0] = image.pixels[pixelPos];
    pixel[1] = image.pixels[pixelPos + 1];
    pixel[2] = image.pixels[pixelPos + 2];
    return pixel;
}

void fillPixel(Image image, int h, int w, uint8_t *p) {
    int pixelPos = (h * image.width * NUM_CHANNELS) + (w * NUM_CHANNELS);
    image.pixels[pixelPos] = p[pixelPos];
    image.pixels[pixelPos + 1] = p[pixelPos + 1];
    image.pixels[pixelPos + 2] = p[pixelPos + 2];
}

Image convertImageToGrayScale(Image image) {

    Image grayImage;
    grayImage.width = image.width;
    grayImage.height = image.height;

    // Convert pixels of input image to gray
    size_t imageSize = image.width * image.height * NUM_CHANNELS;
    int grayChannels = 3;
    size_t grayImageSize = image.width * image.height * grayChannels;

    grayImage.pixels = (uint8_t *) malloc(grayImageSize);

    if (grayImage.pixels == NULL) {
        printf("Unable to allocate memory for the gray image.\n");
        exit(1);
    }

    uint8_t grayValue;
    for (uint8_t *p = image.pixels, *pg = grayImage.pixels;
         p != image.pixels + imageSize; p += NUM_CHANNELS, pg += grayChannels) {
        grayValue = (uint8_t) ((*p + *(p + 1) + *(p + 2)) / 3.0);
        pg[0] = grayValue;
        pg[1] = grayValue;
        pg[2] = grayValue;
    }

    return grayImage;
}

int *getPixelPosition(Image image, int i) {

    int *pixelPosition[2];
    *pixelPosition[0] = i / image.width;
    *pixelPosition[1] = i % image.width;

    return *pixelPosition;
}