
#include "image.h"

#ifndef IMAGE_SEGMENTATION_GENETIC_ALGORITHM_H
#define IMAGE_SEGMENTATION_GENETIC_ALGORITHM_H

#endif //IMAGE_SEGMENTATION_GENETIC_ALGORITHM_H

typedef struct {
    int initialNClusters;
    int crossoverRate;
    int mutationRate;
    int a; // fitness parameter, weight of pixel intensity
    int b; // fitness parameter, weight of spatial position
    int r; // interaction radius
} DesignParameters;

typedef struct {
    int nClusters;
    int *clusterIds;
} Clusters;

int *initializePopulation(Image image, DesignParameters designParameters);
int *evolvePopulation(int *population, DesignParameters designParameters);
int testConvergence(Image image, int *population, DesignParameters designParameters, int oldVariance, int *newVariance);