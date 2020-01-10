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
    designParameters.initialNClusters = 10;
    designParameters.crossoverRate = 0.9;
    designParameters.mutationRate = 0.05;
    designParameters.a = 0.5;
    designParameters.b = 0.5;
    designParameters.r = 3;

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

    int iter = 0;
//    while (!hasConverged) {
    while (iter<20) {
        printf("\n ===========>> START ITER %d <<=========== \n", iter);
        population = evolvePopulation(image, population, designParameters);
        hasConverged = testConvergence(image, population, oldVariance, &newVariance);
        oldVariance = newVariance;
//        printf("\n ===========>> END ITER %d <<=========== \n", iter);
        iter++;
    }

    Image newImage;
    newImage.width = image.width;
    newImage.height = image.height;

    printf("\n");
    uint8_t pixel[3];
    uint8_t grayValue;
    int clusterIndex;
    Clusters clusters = getCluster();
    for (int i = 0; i < image.height; ++i) {
        for (int j = 0; j < image.width; ++j) {
            clusterIndex = findIndex(population[i*image.width+j], clusters.clusterIds, clusters.nClusters);
            if(clusterIndex == -1) {
                printf("\n Error while searching cluster index\n");
                exit(EXIT_FAILURE);
            }
            grayValue = (uint8_t) clusters.clusterGrayValues[clusterIndex];
            pixel[0] = grayValue;
            pixel[1] = grayValue;
            pixel[2] = grayValue;
            newImage = fillPixel(newImage, i, j, pixel);
        }
    }

    printf("Has converged: %d. After %d iterations", hasConverged, iter);

    return newImage;
}