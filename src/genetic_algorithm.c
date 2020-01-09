
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "genetic_algorithm.h"
#include "image.h"

// Global variables
Clusters clusters;

// Function declarations
int mutateChromosome(int chromosome);

int getRandomNumber(int min, int max);

int *selectionProcess(int *oldPopulation, int *evolvedPopulation, Clusters clusters, DesignParameters designParameters,
                      int *newClusterIds);


// Public functions

int *initializePopulation(Image image, DesignParameters designParameters) {
    // Define Clusters structure --> Compute S and L

    // Randomly assign clusters to pixels --> Create P(0)

    // Return P(0)
}

int *evolvePopulation(int *population, DesignParameters designParameters) {
    // Check crossoverRate & mutationRate are between 0 and 100

    // Check if num of selected chromosome is even
    // Randomly select #crossoverRate of chromosomes of population
    // Cross chromosomes half and half

    // Randomly select #mutationRate of chromosomes of population: For each one: mutateChromosome

    // Save indices of new Clusters --> newClusterIds (stack sorted so that smaller is top)

    // Call selectionProcess --> newPopulation

    // Return newPopulation P(t+1)
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

int *selectionProcess(int *oldPopulation, int *evolvedPopulation, Clusters clusters, DesignParameters designParameters,
                      int *newClusterIds) {
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
}