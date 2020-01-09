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

void checkDesignParameters(DesignParameters designParameters){

    if(designParameters.crossoverRate < 0 || designParameters.crossoverRate > 1){
        printf(" << ========================  ERROR  ========================");
    }
    if(designParameters.mutationRate < 0 || designParameters.mutationRate > 1){
        printf(" << ========================  ERROR  ========================");
    }
    if(designParameters.a < 0 || designParameters.b < 0 || (designParameters.a + designParameters.b != 1)){
        printf(" << ========================  ERROR  ========================");
    }

    EXIT_FAILURE;
}

DesignParameters getDesignParameters(){
    DesignParameters designParameters;
    designParameters.initialNClusters = 3;
    designParameters.crossoverRate = 0.9;
    designParameters.mutationRate = 0.02;
    designParameters.a = 0.7;
    designParameters.b = 0.3;

    checkDesignParameters(designParameters);

    return designParameters;
}

int main() {
    srand(time(NULL));

    char *path = "../resources/testBW_small.png";

    Image image = buildImage(path);

    Image grayImage = convertImageToGrayScale(image);

    Image segImage = segmentateImage(grayImage);

    writeImage(segImage);

    return EXIT_SUCCESS;
}

Image segmentateImage(Image image) {
    // get design parameters from console and check values (percentages and a, b)
    DesignParameters designParameters = getDesignParameters();

    int *population = initializePopulation(image, designParameters);

    int hasConverged = 0;
    float newVariance;
    float oldVariance = FLT_MAX;

    while (!hasConverged) {
        population = evolvePopulation(image, population, designParameters);
//        for (int i = 0; i < image.width * image.height; ++i) {
//            printf("\n pix %d --> chromosome: %d", i, population[i]);
//        }
        printf("\n --->> HELLO <<--- \n");
        hasConverged = testConvergence(image, population, oldVariance, &newVariance);
        oldVariance = newVariance;
    }

    printf("has converged: %d", hasConverged);

    return image;
}