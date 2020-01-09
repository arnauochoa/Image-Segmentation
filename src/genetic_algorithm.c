
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <memory.h>
#include "genetic_algorithm.h"
#include "image.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "stack.h"

// Global variables
Clusters clusters;

// Function declarations
int mutateChromosome(int chromosome);

int getRandomNumber(int min, int max);

int *getRandomIndices(int numIndices, int numPixels);

int isValueInArray(int value, int *array);

int *selectionProcess(int *oldPopulation, int *evolvedPopulation, Clusters clusters, DesignParameters designParameters,
                      Stack *newClusterIds);


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
        clusters.clusterIds[lambda] = (int)(pow(2,8) - 1)/clusters.nClusters * lambda;
        //printf("Label: %d \n",clusters.clusterIds[lambda]);
    }

    // Randomly assign clusters to pixels --> Create P(0): population of chromosomes: set of chromosomes (alpha) size: n*m (width*height)
    int *population = (int *) malloc(imageSize);
    int currentChromosome = 0;
    int currentCluster = 0;
    while (currentChromosome < imageSize && currentCluster < clusters.nClusters) {
        population[currentChromosome] = clusters.clusterIds[currentCluster];
        //printf("Population: %d \n", population[currentChromosome]);
        //printf("contador: %d \n", currentChromosome);
        currentCluster++;
        currentChromosome++;

        if(currentCluster == clusters.nClusters){
            currentCluster = 0;
        }
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
        if (!isValueInArray(newChromosome, auxClusters.clusterIds) &&
            !isValueInArray(newChromosome, clusters.clusterIds)) {
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
testConvergence(Image image, int *population, DesignParameters designParameters, int oldVariance, int *newVariance) {

    int hasConverged = 0;

    // int variances[clusters.nClusters]:

    // foreach clusters.custerIds --> k
    //      int *cluster = getAllPixels(population, clusters.clusterIds[k])
    //      int variances[k] = variance(cluster);   <----- sigma_t(k)
    // end

    // *newVariance = sum(variances);   k=0...K-1    <------- Var_t

    // phi = sqrt(min(oldVariance, newVariance))

    // hasConverged = abs(oldVariance - newVariance) <= phi

    return hasConverged;
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

int isValueInArray(int value, int *array) {
    for (int i = 0; i < sizeof(array) / sizeof(array[0]); i++) {
        if (array[i] == value)
            return 1;
    }
    return 0;
}

int *selectionProcess(int *oldPopulation, int *evolvedPopulation, Clusters clusters, DesignParameters designParameters,
                      Stack *newClusterIds) {
    // Apply fitting function to all of oldPopulation --> f_a
    // Apply fitting function to all of evolvedPopulation --> f_b

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

    int *indices = getRandomIndices(10, 255);
    printf("\n Indices = [");
    for (int i = 0; i < 10; i++) {
        printf("%d, ", indices[i]);
    }
    printf("]");
}