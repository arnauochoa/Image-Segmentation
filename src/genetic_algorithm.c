
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

int *obtainClusterGrayValues(Image image, int *population);

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

    // numClusters = K in the paper
    //label (lambda) goes from 0 to K-1
    int imageSize = image.width * image.height;
    //printf("size: %d \n", imageSize);
    clusters.nClusters = designParameters.initialNClusters;
    clusters.clusterIds = (int *) malloc(clusters.nClusters * sizeof(int));

    for (int lambda = 0; lambda < clusters.nClusters; lambda++) {
        clusters.clusterIds[lambda] = (int) ((pow(2, 8) - 1) / clusters.nClusters) * lambda;
//        printf(" -> New label: %d \n",clusters.clusterIds[lambda]);
    }

//    for (int i = 0; i < clusters.nClusters; ++i) {
//        printf("%d, ", clusters.clusterIds[i]);
//    }

    // Randomly assign clusters to pixels --> Create P(0): population of chromosomes: set of chromosomes (alpha) size: n*m (width*height)
    int *population = (int *) malloc(imageSize);
//    int currentChromosome = 0;
    for (int currentChromosome = 0; currentChromosome < imageSize; currentChromosome++) {
//    while(currentChromosome < imageSize){
        for (int currentId = 0; currentId < clusters.nClusters; currentId++) {
            population[currentChromosome] = clusters.clusterIds[getRandomNumber(0, clusters.nClusters - 1)];
//            population[currentChromosome] = clusters.clusterIds[currentId];
//            printf("Population: %d \n", population[currentChromosome]);
//            currentChromosome++;
        }
    }

    clusters.clusterGrayValues = obtainClusterGrayValues(image, population);
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

    // Aux array of new clusters
//    Stack *newClusterIndices = createStack(numMutate);

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
//            // Save indices of pixels of new Clusters in newClusterIndices (stack sorted so that smaller is on top).
//            push(newClusterIndices, mutIndices[i]);
        }
        evolvedPopulation[mutIndices[i]] = newChromosome;
    }
    // selection Process
    int *newPopulation = selectionProcess(image, oldPopulation, evolvedPopulation, designParameters);
    clusters.clusterGrayValues = obtainClusterGrayValues(image, newPopulation);

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
//    printf("\n --->> CLUST ID = %d<<--- \n", clusterId);

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

int *getClusterIntensities(Image image, int *pixelIndices, int size) {
    int *clusterIntensities = malloc(size * sizeof(int));
    for (int i = 0; i < size; ++i) {
        clusterIntensities[i] = image.pixels[pixelIndices[i] * NUM_CHANNELS];
    }
    return clusterIntensities;
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

int isValueInArray(int value, int *array, int size) {
    for (int i = 0; i < size; i++) {
        if (array[i] == value)
            return 1;
    }
    return 0;
}

int findIndex(int value, int *array, int size) {
    for (int i = 0; i < size; i++) {
        if (array[i] == value)
            return i;
    }
    return -1;
}

float computeSimilarityFunction(Image image, int pixel, int *population, DesignParameters designParameters) {
    int pixelGrayValue = image.pixels[pixel * NUM_CHANNELS];
    int *meanGrayValueClusters = obtainClusterGrayValues(image, population);
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

    float rho = (designParameters.a * diffGray) + (designParameters.b * diffGrayNeighbours);


    return rho;

}

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
}

//void updateClusterMeanGrayValue(Clusters cluster, int *population, Image image) {
//
//    int clusterSize;
//    int *pixelsInCluster;
//    int imageSize = image.width * image.height;
//    int grayValuePixelInCluster;
//    for (int currentCluster = 0; currentCluster < cluster.nClusters; currentCluster++) {
//        pixelsInCluster = findPixelsInCluster(population, cluster.clusterIds[currentCluster], imageSize, &clusterSize);
//        for (int currentPixel = 0; currentPixel < clusterSize; currentPixel++) {
//            grayValuePixelInCluster =
//                    grayValuePixelInCluster + image.pixels[pixelsInCluster[currentPixel] * NUM_CHANNELS];
//        }
//        cluster.clusterGrayValues[currentCluster] = grayValuePixelInCluster / clusterSize;
//    }
//}

int *obtainClusterGrayValues(Image image, int *population) {
    int imageSize = image.width * image.height;
    int clusterSize;
    int *clustersGrayValues = malloc(clusters.nClusters * sizeof(int));
    for (int clust = 0; clust < clusters.nClusters; ++clust) {
        int *pixelsInCluster = findPixelsInCluster(population, clusters.clusterIds[clust], imageSize, &clusterSize);
        int *grayscalesInCluster = getClusterIntensities(image, pixelsInCluster, clusterSize);
//        printf("\n ---> Cluster = %d ,(label = %d)", clust, clusters.clusterIds[clust]);
//        for (int j = 0; j < clusterSize; ++j) {
//            printf("   \n pixel %d (pos = %d) ---> %d", j, pixelsInCluster[j], grayscalesInCluster[j]);
//        }
        int sum = 0;
        for (int i = 0; i < clusterSize; i++) {
            sum += grayscalesInCluster[i];
        }
//        printf("\n --->> CLUSTER SIZE = %d<<--- \n", clusterSize);
        clustersGrayValues[clust] = (clusterSize == 0) ? 0 : (sum / clusterSize);
//        printf("\n --->> HELLO 2.5 <<--- \n");

//        printf("  >> sum: %d", sum);
//        printf("  >> clusterSize: %d", clusterSize);
    }

//    printf("\n ====>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
//    for (int k = 0; k < clusters.nClusters; ++k) {
//        printf("%d, ", clusters.clusterGrayValues[k]);
//    }
    return clustersGrayValues;
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

//    for (int i = 0; i < 10; i++) {
//        printf("\n ind %d --> %d", i, getRandomNumber(1, 20));
//    }

//    int *indices = malloc(10 * sizeof(int));
//    getRandomIndices(10, 100, indices);
//    Stack *stack = createStack(15);
//    for (int i = 0; i < 10; i++) {
//        printf("\n push %d --> %d", i, indices[i]);
//        push(stack, indices[i]);
//    }
//    printf("\n==========================================================");
//    printf("\n peek 1 --> %d", peek(stack));
//    printf("\n peek 1 --> %d", peek(stack));
//    printf("\n peek 1 --> %d", peek(stack));
//    printf("\n==========================================================");
//    for (int i = 0; i < 10; i++) {
//        printf("\n pop %d --> %d", i, pop(stack));
//    }


}