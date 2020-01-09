#include "image.h"
#include <stdlib.h>
#include <stdio.h>


const unsigned int NUM_CHANNELS = 3;

/**
 * This function returns the pixel values of the given pixel coordinates. Returns Null if invalid coordinates.
 * @param image Image structure
 * @param h Vertical coordinate
 * @param w Horizontal coordinate
 * @return  Array with values RGB or NULL if coordinates are not valid
 */
uint8_t *getPixel(Image image, int h, int w) {
    if (h >= 0 && h < image.height && w >= 0 && w < image.width) {
        uint8_t *pixel = (uint8_t *) malloc(NUM_CHANNELS * sizeof(int));
        int pixelPos = (h * image.width * NUM_CHANNELS) + (w * NUM_CHANNELS);
        pixel[0] = image.pixels[pixelPos];
        pixel[1] = image.pixels[pixelPos + 1];
        pixel[2] = image.pixels[pixelPos + 2];
        return pixel;
    } else return NULL;
}

/**
 * This function fills the specified pixel on an image at the given coordinates.
 * @param image Image structure to fill
 * @param h Vertical coordinate
 * @param w Horizontal coordinate
 * @param arr RGB values of the new pixel
 */
void fillPixelFromArray(Image image, int h, int w, uint8_t *arr) {
    int pixelPos = (h * image.width * NUM_CHANNELS) + (w * NUM_CHANNELS);
    image.pixels[pixelPos] = arr[pixelPos];
    image.pixels[pixelPos + 1] = arr[pixelPos + 1];
    image.pixels[pixelPos + 2] = arr[pixelPos + 2];
}


Image fillPixel(Image image, int h, int w, uint8_t *pix) {
    int pixelPos = (h * image.width * NUM_CHANNELS) + (w * NUM_CHANNELS);
    image.pixels[pixelPos] = pix[0];
    image.pixels[pixelPos + 1] = pix[1];
    image.pixels[pixelPos + 2] = pix[2];
    return image;
}


/**
 * This function generates a new Image structure where all three RGB of a pixel values correspond to the gray scale
 * value of the same pixel
 * @param image Image structure to convert
 * @return Gray scale image
 */
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

/**
 * This function transforms the coordinate of a pixel from array format to vertical and horizontal coordinates
 * @param image Image structure of the pixel
 * @param i Vector coordinate
 * @return
 */
int *getPixelPosition(Image image, int i) {
    int *pixelPosition = malloc(2 * sizeof(int));
    pixelPosition[0] = i / image.width;
    pixelPosition[1] = i % image.width;

    return pixelPosition;
}