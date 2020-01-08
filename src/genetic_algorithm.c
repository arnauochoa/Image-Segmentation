
#include "genetic_algorithm.h"
#include "image.h"

int mutateChromosome(int chromosome);

int *selectionProcess(int *oldPopulation, int *evolvedPopulation, Clusters clusters, DesignParameters designParameters,
                      int *newClusterIds);

Clusters clusters;

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


int testConvergence(Image image, int *population, DesignParameters designParameters, int oldVariance, int *newVariance) {

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

int mutateChromosome(int chromosome) {

    //https://www.includehelp.com/c-programs/find-binary-addition-subtraction.aspx

    //Select n between 0 and 8

    // if L & 2^n == 1
    //      binary substraction 2^n
    // else
    //      binary addition 2^n
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
