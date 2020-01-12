
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <memory.h>
#include "genetic_algorithm.h"
#include "image.h"
#include "stack.h"

// Macros
#define max(x, y) (((x) >= (y)) ? (x) : (y))
#define min(a, b) (((a) < (b)) ? (a) : (b))

// Global variables
static Clusters clusters;

// Function declarations

int mutateChromosome(int chromosome);

int getRandomNumber(int min, int max);

int *getRandomIndices(int numIndices, int numPixels);

int isValueInArray(int value, int *array, int size);

int *selectionProcess(Image image, int *oldPopulation, int *evolvedPopulation, DesignParameters designParameters);

int *findPixelsInCluster(int *population, int clusterId, int imageSize, int *clusterSize);

float variance(int *array, int size);

int *getClusterIntensities(Image image, int *pixelIndices, int size);

float computeSimilarityFunction(Image image, int pixel, int *population, DesignParameters designParameters);

void obtainClusterGrayValues(Image image, int *population, int *clusterGrayValues);

// Public functions
Clusters getCluster() {
    return clusters;
}

/**
 * This function returns the initial population of chromosomes P(0).
 * @param image Image structure
 * @param designParameters DesignParameters structure
 * @return Int array with the cluster ID which each pixel of the image belongs to.
 */
int *initializePopulation(Image image, DesignParameters designParameters) {
    // Define Clusters structure --> Compute S and L
    int imageSize = image.width * image.height;
    clusters.nClusters = designParameters.initialNClusters;
    clusters.clusterIds = (int *) malloc(clusters.nClusters * sizeof(int));

    for (int lambda = 0; lambda < clusters.nClusters; lambda++) {
        clusters.clusterIds[lambda] = (int) ((pow(2, 8) - 1) / clusters.nClusters) * lambda;
    }

    // Randomly assign clusters to pixels --> Create P(0): population of chromosomes: set of chromosomes (alpha) size: n*m (width*height)
    int *population = (int *) malloc(imageSize);
    for (int currentChromosome = 0; currentChromosome < imageSize; currentChromosome++) {
        for (int currentId = 0; currentId < clusters.nClusters; currentId++) {
            population[currentChromosome] = clusters.clusterIds[getRandomNumber(0, clusters.nClusters - 1)];
        }
    }

     obtainClusterGrayValues(image, population, clusters.clusterGrayValues);
    printf("\n ============ END INITIALIZE ============ \n");
    // Return P(0)
    return population;
}

/**
 * This function returns the new population of chromosomes P(t+1).
 * @param image Image structure
 * @param oldPopulation Int array of chromosomes
 * @param designParameters DesignParameters structure
 * @return Int array with the cluster ID which each pixel of the image belongs to.
 */
int *evolvePopulation(Image image, int *oldPopulation, DesignParameters designParameters) {
    int numPixels = image.height * image.width;
    int evolvedPopulation[numPixels];

    // Check if num of selected chromosome is even
    int numCrossover = (int) (designParameters.crossoverRate * numPixels);
    numCrossover = (numCrossover % 2 == 0) ? numCrossover : numCrossover - 1;

    // Randomly select numCrossover of chromosomes of population
    int *crossIndices = getRandomIndices(numCrossover, numPixels);

    int *firstHalf = crossIndices;
    int *secondHalf = crossIndices + (numCrossover / 2);

    memcpy(evolvedPopulation, oldPopulation, numPixels * sizeof(int));

    // Cross chromosomes half and half
    int aux;
    for (int i = 0; i < (numCrossover / 2); i++) {
        aux = evolvedPopulation[firstHalf[i]];
        evolvedPopulation[firstHalf[i]] = evolvedPopulation[secondHalf[i]];
        evolvedPopulation[secondHalf[i]] = aux;
    }

    // Mutate
    int numMutate = (int) (designParameters.mutationRate * numPixels);
    int *mutIndices = getRandomIndices(numMutate, numPixels);

    // Mutate each selected chromosome
    for (int i = 0; i < numMutate; i++) {
        int newChromosome = mutateChromosome(evolvedPopulation[mutIndices[i]]);

        //If chromosome is new Label add it to aux array
        if (!isValueInArray(newChromosome, clusters.clusterIds, clusters.nClusters)) {
            int smallerInd = 0;
            int smaller = abs(newChromosome - clusters.clusterIds[0]);
            for (int clust = 1; clust < clusters.nClusters; ++clust) {
                int diff = abs(newChromosome - clusters.clusterIds[clust]);
                if (smaller > diff) {
                    smaller = diff;
                    smallerInd = clust;
                }
            }
            newChromosome = clusters.clusterIds[smallerInd];
        }
        evolvedPopulation[mutIndices[i]] = newChromosome;
    }
    // selection Process
    int *newPopulation = selectionProcess(image, oldPopulation, evolvedPopulation, designParameters);
    obtainClusterGrayValues(image, newPopulation, clusters.clusterGrayValues);

    // Return newPopulation P(t+1)
    return newPopulation;
}

/**
 * This function returns 1 if the GA has converged or 0 otherwise, and updates the old variance
 * @param image Image structure
 * @param population Int array of chromosomes
 * @param oldVariance float value
 * @param newVariance float value
 * @return Int value (1 or 0)
 */
int testConvergence(Image image, int *population, float oldVariance, float *newVariance) {

    int imageSize = image.height * image.width;
    int hasConverged = 0;

    float varSum = 0;

    for (int i = 0; i < clusters.nClusters; i++) {
        int clusterSize;
        int *clusterPixels = findPixelsInCluster(population, clusters.clusterIds[i], imageSize, &clusterSize);
        int *clusterIntensities = getClusterIntensities(image, clusterPixels, clusterSize);
        varSum += variance(clusterIntensities, clusterSize);
    }

    double phi = sqrt(min(oldVariance, varSum));

    hasConverged = fabs(oldVariance - varSum) <= phi;

    *newVariance = varSum;
    return hasConverged;
}

/**
 * This function returns the array of pixels contained in a cluster
 * @param population Int array
 * @param clusterId Int value
 * @param imageSize Int value
 * @param clusterSize Int array
 * @return Int array of the pixels indices wrt the image
 */
int *findPixelsInCluster(int *population, int clusterId, int imageSize, int *clusterSize) {
    int size = 0;

    // Create aux array bigger than needed
    int *auxArray = malloc(imageSize * sizeof(int));
    if (!auxArray) printf("Error allocating memory for auxArray in findPixelsInCluster");
    int auxIndex = 0;

    // Find pixels of current cluster
    for (int pix = 0; pix < imageSize; pix++) {
        if (population[pix] == clusterId) {
            size++;
            auxArray[auxIndex] = pix;
            auxIndex++;
        }
    }

    // Copy cluster array to new array with proper size
    int *pixelsInCluster = malloc(size * sizeof(int));
    if (!pixelsInCluster) printf("Error allocating memory for pixelsInCluster in findPixelsInCluster");
    memcpy(pixelsInCluster, auxArray, size * sizeof(int));
    free(auxArray);
    *clusterSize = size;

    return auxArray;
}

/**
 * This function computes de variance in an array
 * @param array Array of the values to compute the variance from
 * @param size Size of the array
 * @return
 */
float variance(int *array, int size) {
    float average;
    float sum = 0.0;
    float varSum = 0.0;
    // Sum of all elements and average
    for (int i = 0; i < size; i++) {
        sum += array[i];
    }
    average = sum / (float) size;
    // Variance
    for (int i = 0; i < size; i++) {
        varSum += powf((array[i] - average), 2);
    }
    return varSum / (float) size;
}

/**
 * This functions returns an array with the color intensities of the pixels given to the function
 * @param image Image structure of the actual image
 * @param pixelIndices Array with the indexes of the pixels
 * @param size Size of the array of pixels
 * @return Array with the color intensities of the pixels
 */
int *getClusterIntensities(Image image, int *pixelIndices, int size) {
    int *clusterIntensities = malloc(size * sizeof(int));
    for (int i = 0; i < size; ++i) {
        clusterIntensities[i] = image.pixels[pixelIndices[i] * NUM_CHANNELS];
    }
    return clusterIntensities;
}

// Private functions
/**
 * This functions modifies the nth bit of the chromosome of a pixel
 * @param chromosome Chromosome to modify
 * @return Modified chromosome
 */
int mutateChromosome(int chromosome) {
    int n = getRandomNumber(0, 7);
    int bit = (int) pow(2, n);

    return chromosome ^ bit;
}

/**
 * This function returns a random number between two values
 * @param min Int value of the minimum possible random number
 * @param max Int value of the maximum possible random number
 * @return Generated random number
 */
int getRandomNumber(int min, int max) {
    return min + rand() % (max + 1 - min);
}

/**
 * This function generates an array of different random numbers given the interval [0 to numPixels]
 * @param numIndices Length of the resulting array
 * @param numPixels Maximum value
 * @return Resulting array of random numbers
 */
int *getRandomIndices(int numIndices, int numPixels) {
    int *indices = malloc(numIndices * sizeof(int));
    int in, im;

    im = 0;

    for (in = 0; in < numPixels && im < numIndices; ++in) {
        int rn = numPixels - in;
        int rm = numIndices - im;
        if (rand() % rn < rm)
            /* Take it */
            indices[im++] = in;
    }

    assert(im == numIndices);

    return indices;
}

/**
 * Returns 1 if the given value is in the given array
 * @param value Value to find
 * @param array Array where to find the value
 * @param size Size of the array
 * @return 1 if the given value is in the given array, 0 otherwise
 */
int isValueInArray(int value, int *array, int size) {
    for (int i = 0; i < size; i++) {
        if (array[i] == value)
            return 1;
    }
    return 0;
}

/**
 * Find the index of a given value in a given array
 * @param value Value to find
 * @param array Array where to find the value
 * @param size Size of the array
 * @return The index of the value in the array. -1 if not found
 */
int findIndex(int value, int *array, int size) {
    for (int i = 0; i < size; i++) {
        if (array[i] == value)
            return i;
    }
    return -1;
}

/**
 * This functions returns the result of applying the fitting function of a population
 * @param image Image structure
 * @param pixel Reference pixel
 * @param population Set of chromosomes of the pixels
 * @param designParameters design parameters structure
 * @return Result of the fitting function
 */
float computeSimilarityFunction(Image image, int pixel, int *population, DesignParameters designParameters) {
    int pixelGrayValue = image.pixels[pixel * NUM_CHANNELS];
    int *meanGrayValueClusters = malloc(clusters.nClusters * sizeof(int));
    obtainClusterGrayValues(image, population, meanGrayValueClusters);
    int pixelClusterLabel = population[pixel];
    int clusterIndex = findIndex(pixelClusterLabel, clusters.clusterIds, clusters.nClusters);
    if (clusterIndex == -1) {
        printf("\n There was an error indexing the cluster label\n");
        printf("pixelClusterLabel --> %d \n", pixelClusterLabel);
        exit(EXIT_FAILURE);
    }

    float diffGray = (float) abs(pixelGrayValue - meanGrayValueClusters[clusterIndex]) /
                     (float) max(pixelGrayValue, meanGrayValueClusters[clusterIndex]);

    int *pixelPosition = getPixelPosition(image, pixel);
    int nNeighbours = (int) pow(2 * designParameters.r + 1, 2) - 1;

    uint8_t *neighboursGrayValues = malloc(nNeighbours * sizeof(int));

    int n = 0;
    int startRow = (pixelPosition[0] - designParameters.r);
    int endRow = startRow + (2 * designParameters.r + 1);
    int startColumn = (pixelPosition[1] - designParameters.r);
    int endColumn = startColumn + (2 * designParameters.r + 1);
    for (int row = startRow; row < endRow; row++) {
        for (int col = startColumn; col < endColumn; col++) {
            if (row != pixelPosition[0] || col != pixelPosition[1]) { // If not is "our" pixel
                uint8_t *currentPixel = getPixel(image, row, col);
                if (currentPixel != NULL) { // TODO: not sure
                    neighboursGrayValues[n] = currentPixel[0];
                    n++;
                }
            }
        }
    }

    float diffGrayNeighbours = 0;
    for (int actualNeighbours = 0; actualNeighbours < n; actualNeighbours++) {
        diffGrayNeighbours +=
                (float) abs(neighboursGrayValues[actualNeighbours] - meanGrayValueClusters[clusterIndex]) /
                (float) max(neighboursGrayValues[actualNeighbours], meanGrayValueClusters[clusterIndex]);
    }

    free(meanGrayValueClusters);
    free(neighboursGrayValues);

    return (designParameters.a * diffGray) + (designParameters.b * diffGrayNeighbours);
}

/**
 * This function returns the new population after comparing the fitting results
 * @param image Image structure
 * @param oldPopulation Set of old chromosomes
 * @param evolvedPopulation Set of evolved chromosomes (after crossover and mutation)
 * @param designParameters design parameters structure
 * @return Array of int corresponding to the new chromosomes (population)
 */
int *selectionProcess(Image image, int *oldPopulation, int *evolvedPopulation, DesignParameters designParameters) {
    // Apply fitting function to all of oldPopulation --> f_a
    // Apply fitting function to all of evolvedPopulation --> f_b
    int sizeImage = image.width * image.height;

    float oldPopulationFitted[sizeImage];       // Results of fitting function on old population
    float evolvedPopulationFitted[sizeImage];   // Results of fitting function on evolved population

    int *newPopulation = malloc(sizeImage * sizeof(int));
    for (int currentPixel = 0; currentPixel < sizeImage; currentPixel++) {

        int pixelCluster = findIndex(oldPopulation[currentPixel], clusters.clusterIds, clusters.nClusters);
        if (pixelCluster == -1) {
            printf("\n Error in function selectionProcess when indexing cluster");
            printf("\ncurrentPixel = %i", currentPixel);
            printf("\noldPopulation[currentPixel] = %i", oldPopulation[currentPixel]);
            printf("\n Clusters labels : ");
            for (int i = 0; i < clusters.nClusters; ++i) {
                printf("%d, ", clusters.clusterIds[i]);
            }

            exit(EXIT_FAILURE);
        }
        oldPopulationFitted[currentPixel] = computeSimilarityFunction(image, currentPixel, oldPopulation,
                                                                      designParameters);

        evolvedPopulationFitted[currentPixel] = computeSimilarityFunction(image,
                                                                          currentPixel,
                                                                          evolvedPopulation,
                                                                          designParameters);
        if (evolvedPopulationFitted[currentPixel] < oldPopulationFitted[currentPixel]) {
            newPopulation[currentPixel] = evolvedPopulation[currentPixel];
        } else {
            newPopulation[currentPixel] = oldPopulation[currentPixel];
        }
    }
    return newPopulation;
}/**
 * This functions returns an array of the gray values of the pixels that belong to a same cluster
 * @param image Image structure
 * @param population Chromosomes of the cluster
 * @return Array of the gray values
 */
void obtainClusterGrayValues(Image image, int *population, int *clusterGrayValues) {
    int imageSize = image.width * image.height;
    int clusterSize;
    //int *clustersGrayValues = malloc(clusters.nClusters * sizeof(int));
    for (int clust = 0; clust < clusters.nClusters; ++clust) {
        int *pixelsInCluster = findPixelsInCluster(population, clusters.clusterIds[clust], imageSize, &clusterSize);
        int *grayscalesInCluster = getClusterIntensities(image, pixelsInCluster, clusterSize);

        int sum = 0;
        for (int i = 0; i < clusterSize; i++) {
            sum += grayscalesInCluster[i];
        }
        clusterGrayValues[clust] = (clusterSize == 0) ? 0 : (sum / clusterSize);
    }
}