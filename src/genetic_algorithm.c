
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
#define new_max(x, y) ((x) >= (y)) ? (x) : (y)
#define min(a, b) (((a) < (b)) ? (a) : (b))

// Global variables
Clusters clusters;

// Function declarations
int mutateChromosome(int chromosome);

int getRandomNumber(int min, int max);

int *getRandomIndices(int numIndices, int numPixels);

int isValueInArray(int value, int *array, int size);

int *selectionProcess(Image image, int *oldPopulation, int *evolvedPopulation, Clusters clusters,
                      DesignParameters designParameters, Stack *newClusterIds);

int *findPixelsInCluster(int *population, int clusterId, int imageSize, int *clusterSize);

float variance(int *array, int size);

int *getPixelIntensity(Image image, int *pixelIndices);

float
computeSimilarityFunction(Image image, int *population, int pixel1, int pixel2, DesignParameters designParameters);

// Public functions

int *initializePopulation(Image image, DesignParameters designParameters) {
    // Define Clusters structure --> Compute S and L

    // numClusters = K in the paper
    //label (lambda) goes from 0 to K-1
    int imageSize = image.width * image.height;
    //printf("size: %d \n", imageSize);
    clusters.nClusters = designParameters.initialNClusters;
    clusters.clusterIds = (int *) malloc(designParameters.initialNClusters);

    for (int lambda = 0; lambda < clusters.nClusters; lambda++) {
        clusters.clusterIds[lambda] = (int) ((pow(2, 8) - 1) / clusters.nClusters) * lambda;
        //printf("Label: %d \n",clusters.clusterIds[lambda]);
    }

    printf("\n>> Clusters ");
    for (int i = 0; i < clusters.nClusters; ++i) {
        printf("%d, ", clusters.clusterIds[i]);
    }

    // Randomly assign clusters to pixels --> Create P(0): population of chromosomes: set of chromosomes (alpha) size: n*m (width*height)
    int *population = (int *) malloc(imageSize);

    for (int currentChromosome = 0; currentChromosome < imageSize; currentChromosome++) {
        population[currentChromosome] = clusters.clusterIds[getRandomNumber(0, clusters.nClusters-1)];
//        printf("\n Population: %d", population[currentChromosome]);
//        if(!isValueInArray(population[currentChromosome], clusters.clusterIds, clusters.nClusters)) printf(" <<--- ERROR ------");
    }
    // Return P(0)
    return population;
}

int *evolvePopulation(Image image, int *population, DesignParameters designParameters) {

    int numPixels = image.height * image.width;

    // Check if num of selected chromosome is even
    int numCrossover = designParameters.crossoverRate * numPixels;

    // Randomly select numCrossover of chromosomes of population
    int *crossIndices = getRandomIndices(numCrossover, numPixels);
    int *firstHalf = crossIndices;
    int *secondHalf = crossIndices + (numCrossover / 2);

    // Cross chromosomes half and half
    int aux;
    for (int i = 0; i < (numCrossover / 2); i++) {
        aux = population[firstHalf[i]];
        population[firstHalf[i]] = population[secondHalf[i]];
        population[secondHalf[i]] = aux;
    }

    // Mutate
    int numMutate = designParameters.mutationRate * numPixels;
    int *mutIndices = getRandomIndices(numMutate, numPixels);

    // Aux array of new clusters
    Clusters auxClusters = {.nClusters = 0, .clusterIds = malloc(numMutate * sizeof(int))};
    Stack *newClusterPixels = createStack(numMutate);

    // Mutate each selected chromosome
    for (int i = 0; i < numMutate; i++) {
        int newChromosome = mutateChromosome(population[mutIndices[i]]);
        population[mutIndices[i]] = newChromosome;

        //If chromosome is new Label add it to aux array
        if (!isValueInArray(newChromosome, auxClusters.clusterIds, auxClusters.nClusters) &&
            !isValueInArray(newChromosome, clusters.clusterIds, clusters.nClusters)) {
            auxClusters.clusterIds[auxClusters.nClusters] = newChromosome;
            auxClusters.nClusters++;

            // Save indices of pixels of new Clusters in newClusterPixels (stack sorted so that smaller is on top).
            push(newClusterPixels, mutIndices[i]);
        }
    }

    // TODO: Call selectionProcess --> newPopulation

    // TODO: Return newPopulation P(t+1)
}

int
testConvergence(Image image, int *population, float oldVariance, float *newVariance) {

    int imageSize = image.height * image.width;
    int hasConverged = 0;

    float varSum = 0;

    for (int i = 0; i < clusters.nClusters; i++) {
        int clusterSize;
        int *clusterPixels = findPixelsInCluster(population, clusters.clusterIds[i], imageSize, &clusterSize);
        int *clusterIntensities = getPixelIntensity(image, clusterPixels);
        varSum += variance(clusterIntensities, clusterSize);
    }

    double phi = sqrt(min(oldVariance, varSum));

    hasConverged = fabs(oldVariance - varSum) <= phi;

    *newVariance = varSum;
    return hasConverged;
}

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

int *getPixelIntensity(Image image, int *pixelIndices) {
    //TODO: compute grayscale of given pixel
    return NULL;
}

// Private functions

int mutateChromosome(int chromosome) {
    int n = getRandomNumber(0, 7);
    int bit = (int) pow(2, n);

    return chromosome ^ bit;
}

int getRandomNumber(int min, int max) {
    return min + rand() % (max + 1 - min);
}

/**
 * The Knuth algorithm --> Complexity O(N), N=numPixels. Sorted result.
 * https://stackoverflow.com/questions/1608181/unique-random-numbers-in-an-integer-array-in-the-c-programming-language
 */
int *getRandomIndices(int numIndices, int numPixels) {
    int indices[numIndices];
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

int isValueInArray(int value, int *array, int size) {
    for (int i = 0; i < size; i++) {
        if (array[i] == value)
            return 1;
    }
    return 0;
}

float
computeSimilarityFunction(Image image, int *population, int pixel1, int pixel2, DesignParameters designParameters) {

    float diffGray =
            abs(image.pixels[pixel1] - image.pixels[pixel2]) / new_max(image.pixels[pixel1], image.pixels[pixel2]);

    int *pixel1Position = getPixelPosition(image, pixel1);
    int nNeighbours = pow(2 * designParameters.r + 1, 2) - 1;
    float *diffGrayNeighbours[nNeighbours];
    int *pixelNeighbours[nNeighbours];

    //for (int currentNeighbour = 0; currentNeighbour < designParameters.r; currentNeighbour++){

    //}

    //i*m+j, i*(m-1)+j, i*(m+1)+j, (i*m)+j-r, (i*m)+j+r, i*(m-1)+j-r, i*(m-1)+j+r, i*(m+1)+j-r, i*(m+1)+j+r,
    //for (int currentNeighbour = 0; currentNeighbour < )
    //float rho = designParameters.a * diffGray + b * ;



}

int *selectionProcess(Image image, int *oldPopulation, int *evolvedPopulation, Clusters clusters,
                      DesignParameters designParameters, Stack *newClusterIds) {
    // Apply fitting function to all of oldPopulation --> f_a
    // Apply fitting function to all of evolvedPopulation --> f_b
    float **oldPopulationFitted;
    float **evolvedPopulationFitted;
    int sizeImage = image.width * image.height;

    for (int currentPixel = 0; currentPixel < sizeImage; currentPixel++) {
        for (int comparisonPixel = 0; comparisonPixel < sizeImage; comparisonPixel++) {
            oldPopulationFitted[currentPixel][comparisonPixel] = computeSimilarityFunction(image, oldPopulation,
                                                                                           currentPixel,
                                                                                           comparisonPixel,
                                                                                           designParameters);
        }


    }




    // malloc newPopulation (sizeof(oldPopulation))

    // foreach pixels --> i
    // If f_b < f_a
    //      newPopulation(i) = evolvedPopulation(i)
    //      if i == peek(newClusterIds)
    //          nClusters ++
    //          append(clustersIds, pop(newClusterIds)
    // else
    //      newPopulation(i) = oldPopulation(i)

    // return newPopulation
}


void test() {
//    for (int i = 0; i < 10; i++) {
//        printf("\n n = %i", getRandomNumber(0, 8));
//    }

//    for (int i = 0; i < 1000; i++) {
//        int a = mutateChromosome(0);
//        printf("\n chromosome 0 -> %d", a);
//        if (a!=0 && a!=1 && a!=2 && a!=4 && a!=8 && a!=16 && a!=32 && a!=64 && a!=128 ) {
//            printf(" << ========================  ERROR  ========================");
//        }
//    }


//    int population[12] = {1, 1, 2, 1, 4, 5, 2, 1, 6, 6, 9, 1};
//    int clusterSize;
//    int *cluster = findPixelsInCluster(population, 2, 12, &clusterSize);
//    printf("Pixels in cluster id = 2 --> ");
//    for (int i = 0; i < clusterSize; i++) {
//        printf("%d, ", cluster[i]);
//    }

//    int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
//    printf("\n  Variance --> %f", variance(arr, 10));

//    float a = 3.3;
//    float b = 3.11;
//    printf("\n min = %f", min(a,b));


}