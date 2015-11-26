/*******************************************************************************
 * Author   : Jennifer Winer
 * 
 * Project  : A DFG Off-Line Task Scheduler for FPGA
 *              - The Genetic Algorithm for determining the ideal implementation
 *                  for each task's operation
 * 
 * Created  : May 7, 2013
 * Modified : July 13, 2013
 ******************************************************************************/

/*******************************************************************************
 * Filename : population.h
 * 
 * Purpose  : a header for population.c
 ******************************************************************************/



#ifndef POPULATION_H
#define	POPULATION_H

#include "individual.h"



/******************************************************************************
 *****************             GETTERS AND SETTERS            *****************
 *****************************************************************************/

/******************************************************************************
 * NAME : setCrossoverRate
 * 
 * PURPOSE : a setter for the crossover rate
 * ARGUMENTS : double = a real number between 0 and 1 
 *****************************************************************************/
void setCrossoverRate(double);

/******************************************************************************
 * NAME : getCrossoverRate
 * 
 * PURPOSE : getter for the crossover rate 
 * RETURNS : The rate of crossover as a number between 0 and 1
 *****************************************************************************/
double getCrossoverRate(void);


/******************************************************************************
 * NAME : setMutationRate
 * 
 * PURPOSE : a setter for the mutation rate
 * ARGUMENTS : double = a real number between 0 and 1 
 *****************************************************************************/
void setMutationRate(double);

/******************************************************************************
 * NAME : getMutationRate
 * 
 * PURPOSE : getter for the mutation rate 
 * RETURNS : The rate of mutation as a number between 0 and 1
 *****************************************************************************/
double getMutationRate(void);


/*******************************************************************************
 * Population
 * 
 * Contains an array of all the individuals in a population along with the 
 * number of individuals in the population      
 ******************************************************************************/
typedef struct{
    Individual * member;
    int         size;
}Population;



/******************************************************************************
 *****************            CREATION AND DELETION           *****************
 *****************************************************************************/

/******************************************************************************
 * NAME : genRandPopulation
 * 
 * PURPOSE : Creates a population of individuals. Each individual will
 *              contain randomly generated genes.
 *           see initRandIndividual() for more information
 * ARGUMENTS : int = the number of individuals in the population
 * 
 * RETURNS : A population of randomly generated individuals
 * 
 * NOTE : the population that is returned should eventually be freed through
 *              the usage of freePopulation();
 *****************************************************************************/
Population * genRandPopulation(int);

/******************************************************************************
 * NAME : genSeededPopulation
 * 
 * PURPOSE : Creates a population of pre-determined individuals to ensure a 
 *              diverse starting population. PERCENT_POP_RANDOM in config.h
 *              determines what percentage of the population is randomly 
 *              generated, with the remaining individuals being seeded.
 *           see initRandIndividual() and initSeededIndividual() for more 
 *              information
 * ARGUMENTS : int = the number of individuals in the population
 * 
 * RETURNS : A population consisting of some random and some seeded individuals
 * 
 * NOTE : the population that is returned should eventually be freed through
 *              the usage of freePopulation();
 *****************************************************************************/
Population * genSeededPopulation(int);

/******************************************************************************
 * NAME : freePopulation
 * 
 * PURPOSE : Frees all dynamically allocated data from within a Population
 * ARGUMENTS : Population * = the population that you wish to free
 * 
 * PRECONDITIONS : the Population passed in as a parameter must have been
 *      previously created by genRandPopulation();
 *****************************************************************************/
void freePopulation(Population *);



/******************************************************************************
 *****************                MANIPULATION                *****************
 *****************************************************************************/

/******************************************************************************
 * NAME : determineFitness
 * 
 * PURPOSE : Calculate the fitness of all members of a population
 * ARGUMENTS : Population * = the population that you wish evaluate
 *****************************************************************************/
void determineFitness(Population *);

/******************************************************************************
 * NAME : evaluateRanks
 * 
 * PURPOSE : Sorts the individuals of a population in ascending order
 *              based on their fitnesses
 * ARGUMENTS : Population * = the Population that you wish to evaluate
 *****************************************************************************/
void sortByFitness(Population *);

/******************************************************************************
 * NAME : evolvePopulation
 * 
 * PURPOSE : Perform recombination (crossover) and mutation on a Population
 * ARGUMENTS : Population * = the population that you wish to transform
 *             int = the crossover type that was chosen
 *             int = the mutation type that was chosen
 *****************************************************************************/
void evolvePopulation(Population *, int, int);



/******************************************************************************
 *****************               VISUALIZATION                *****************
 *****************************************************************************/

/******************************************************************************
 * NAME : printPopulation
 * 
 * PURPOSE : Print all chromosomes with their fitness values
 * ARGUMENTS : Population * = the population that you wish visualize
 *****************************************************************************/
void printPopulation(Population *);

/******************************************************************************
 * NAME : printSummaryStatistics
 * 
 * PURPOSE : print basic statistical information about a population. This 
 *              includes the minimum fitness, the maximum fitness, the average
 *              fitness and the standard deviation of all fitnesses in the
 *              population
 * ARGUMENTS : Population * = the population that you wish visualize
 *****************************************************************************/
void printSummaryStatistics(Population *);

/******************************************************************************
 * NAME : printPopDiversity
 * 
 * PURPOSE : print a population's diversity information. This includes the sum
 *              of the hamming distances between every pair of individuals, 
 *              the hamming distance of all of the previously mentioned
 *              hamming distance, and the number of unique individuals in a
 *              population.
 *              
 * ARGUMENTS : Population * = the population that you wish to analyze
 *****************************************************************************/
void printPopDiversity(Population *);

/******************************************************************************
 * NAME : printGeneComposition
 * 
 * PURPOSE : Print the occurrence rates for every allele of each gene
 * ARGUMENTS : Population * = the population that you wish to analyze
 *****************************************************************************/
void printGeneComposition(Population *);

#endif	/* POPULATION_H */

