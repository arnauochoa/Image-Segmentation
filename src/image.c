//
// Created by cttc on 10/12/19.
//

#include "image.h"
#include <stdlib.h>


const int NUM_CHANNELS = 3;

int *getPixel(Image img, int h, int w){
    int *pixel = (int*) malloc(NUM_CHANNELS * sizeof(int));
    pixel[0] = img.pixels[w * img.width + h];
    pixel[1] = img.pixels[w * img.width + h + 1];
    pixel[2] = img.pixels[w * img.width + h + 2];
    return pixel;
}

void fillPixel(Image image, int h, int w, int pixel[3]){
    image.pixels[w*image.width + h] = pixel[0];
    image.pixels[w*image.width + h + 1] = pixel[1];
    image.pixels[w*image.width + h + 2] = pixel[2];
}