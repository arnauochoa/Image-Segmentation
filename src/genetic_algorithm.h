
#include "image.h"

#ifndef IMAGE_SEGMENTATION_GENETIC_ALGORITHM_H
#define IMAGE_SEGMENTATION_GENETIC_ALGORITHM_H

#endif //IMAGE_SEGMENTATION_GENETIC_ALGORITHM_H

typedef struct {
    int initialNClusters;
    float crossoverRate; // 0 < cr < 1
    float mutationRate;  // 0 < mr < 1
    float a; // fitness parameter, weight of pixel intensity
    float b; // fitness parameter, weight of spatial position
    int r; // interaction radius
} DesignParameters;

typedef struct {
    int nClusters;
    int *clusterIds;
    int *clusterGrayValues;
} Clusters;

Clusters getCluster();
int *initializePopulation(Image image, DesignParameters designParameters);
int *evolvePopulation(Image image, int *oldPopulation, DesignParameters designParameters);
int testConvergence(Image image, int *population, float oldVariance, float *newVariance);
int findIndex(int value, int *array, int size);

void test();