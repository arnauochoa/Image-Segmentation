#include <stdio.h>
#include <stdlib.h>

#include "build_image_matrix.h"
#include "write_image_matrix.h"
#include "genetic_algorithm.h"
#include <stdint.h>
#include <sys/time.h>
#include <float.h>
#include <time.h>

Image segmentateImage(Image image);

int main() {
    srand(time(NULL));
    char *path = "../resources/testBW_small.png";

    // get design parameters from console and check values (percentages and a, b)

//    Image image = buildImage(path);
//
//    Image segImage = segmentateImage(image);
//
//    writeImage(segImage);

    test();

//    Image grayImage = convertImageToGrayScale(image);
//
//    DesignParameters designParameters;
//    designParameters.initialNClusters = 5;
//    designParameters.crossoverRate = 0.9;
//    designParameters.mutationRate = 0.02;
//    designParameters.a = 0.7;
//    designParameters.b = 0.3;
//
//    int *population = initializePopulation(grayImage, designParameters);

    //Image segImage = segmentateImage(image);


//    Image segImage = segmentateImage(grayImage);

//    writeImage(grayImage);

    return EXIT_SUCCESS;
}

//Image segmentateImage(Image image) {
//    DesignParameters designParameters; // TODO: initialize
//
//    int *population = initializePopulation(image, designParameters);
//
//    int hasConverged = 0;
//    int newVariance;
//    int oldVariance = INT32_MAX;
//
//    while (!hasConverged) {
//        population = evolvePopulation(population, designParameters);
//        hasConverged = testConvergence(image, population, designParameters, oldVariance, &newVariance);
//        oldVariance = newVariance;
//    }
//
//
//    return image;
//
//
//}