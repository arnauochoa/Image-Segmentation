//
// Created by Arnau Ochoa Bañuelos on 2019-11-28.
//

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
    int *img = (int*) stbi_load(path, &image.width, &image.height, &channels, NUM_CHANNELS); //file path, width and height of the image, the number of color channels
                                                                                  //if R,G,B -> channels = 3 (ignoring transparency)
                                                                                  // if R,G,B,alpha -> 4 channels
                                                                                  //last parameter = 0, then we are loading the image as it is (with 4 channels)
    if (img == NULL) {
        printf("Error in loading the image\n");
        exit(1);
    }
    printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", image.width, image.height, channels);
    if (channels < NUM_CHANNELS){
        printf("Error: Image has less than %d channels", NUM_CHANNELS);
        exit(1);
    }

    image.pixels = (int*) malloc(image.width * image.height * 4 * sizeof(int));

    int *p = image.pixels;
    for(int i=0;i<image.height;i++) {
        for (int j = 0; j < image.width; j++) {
            int *pixel = getPixel(image, i, j);
            printf("(%d %d %d)\t", pixel[0], pixel[1], pixel[2]);
            p++;
        }
        printf("\n");
    }

    stbi_image_free(img);
    return image;
}


//unsigned char *convertToGrayscale(unsigned char *img, int width, int height, int channels){
//    // Allocate memory for greyscale image
//    size_t img_size = (size_t) width * height * channels;
////    int gray_channels = channels == 4 ? 2 : 1;
//    int gray_channels = 1;
//    size_t gray_img_size = (size_t) width * height * gray_channels;
//    unsigned char *gray_img = malloc(gray_img_size);
//    if (gray_img == NULL) {
//        printf("Unable to allocate memory for the gray image.\n");
//        exit(1);
//    }
//
//    // Convert image to grayscale
//    for (unsigned char *p = img, *pg = gray_img; p != img + img_size; p += channels, pg += gray_channels) {
//        *pg = (uint8_t) ((*p + *(p + 1) + *(p + 2)) / 3.0);
//
////        Copy alpha value for grayscale + alpha
////        if (channels == 4) {
////            *(pg + 1) = *(p + 3);
////        }
//    }
//
//    return gray_img;
//}

//int *convertToGrayscale_v2(int width, int height, int channels, int ImgMatrix[][width][channels]){

//    channels_grey = 1;
//    for(int i=0;i<height;i++) {
//        for (int j = 0; j < width; j++) {
//            for (int k = 0; k < channels; k++) {
//                GreyImgMatrix [i][j] = ImgMatrix [i][j][k];
//                p++;
//                //printf("Current matrix position: x: %d, y: %d, z: %d and its corresponding value: %d\n",i,j,k,img[(i+j*width)*channels+k]);
//                printf("%d\t",ImgMatrix[i][j][k]);
//            }
//            printf("\n");
//        }
//        printf("\n");
//    }

//}