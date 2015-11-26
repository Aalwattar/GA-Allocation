/*******************************************************************************
 * Author   : Jennifer Winer
 * 
 * Project  : A DFG Off-Line Task Scheduler for FPGA
 *              - The Genetic Algorithm for determining the ideal implementation
 *                  for each task's operation
 * 
 * Created  : May 7, 2013
 * Modified : July 12, 2013
 ******************************************************************************/

/*******************************************************************************
 * Filename : individual.h
 * 
 * Purpose  : a header for individual.c
 ******************************************************************************/


#ifndef INDIVIDUAL_H
#define	INDIVIDUAL_H

#include "util.h"

/*******************************************************************************
 * Individual
 * 
 * Encodes one individual in the population (a series of architectures with its
 * corresponding fitness information)
 ******************************************************************************/
typedef struct{
    int * encoding;     // their genotype (corresponds to an architecture's index)
    
    int fitness;
    int exec_time;
    int energy;
    
    int num_reuse;
    int prefetch;
} Individual;

/******************************************************************************
 *****************            CREATION AND DELETION           *****************
 *****************************************************************************/

/******************************************************************************
 * NAME : initRandIndividual
 * 
 * PURPOSE : Initializes an individual with randomly generated genes (while 
 *              ensuring that each allele corresponds to a valid architecture)
 * ARGUMENTS : Individual * = the individual to initialize
 * 
 * PRECONDITIONS : the Individual struct must already exist in memory
 *****************************************************************************/
void initRandIndividual(Individual *);

/******************************************************************************
 * NAME : initSeededIndividual
 * 
 * PURPOSE : Initializes an individual with a predetermined set of genes to
 *              ensure the diversity of the initial population. This function
 *              will produce a different individual every time it is called. 
 *              (while ensuring that each allele corresponds to a valid 
 *              architecture / implementation)
 * ARGUMENTS : Individual * = the individual to initialize
 * 
 * PRECONDITIONS : the Individual struct must already exist in memory
 *****************************************************************************/
void initSeededIndividual(Individual *);


/******************************************************************************
 * NAME : duplicateIndividual
 * 
 * PURPOSE : Creates a deep copy of the individual passed in as an argument 
 * ARGUMENTS : Individual * = the destination for the copy
 *             Individual * = the individual that you wish to copy
 * 
 * PRECONDITIONS : the Individuals passed in as a second parameter must have
 *      been previously created either through initRandIndividual() or 
 *      duplicateIndividual().
 *****************************************************************************/
void duplicateIndividual(Individual *, Individual *);

/******************************************************************************
 * NAME : freeIndividual
 * 
 * PURPOSE : Frees all dynamically allocated data from within an Individual
 * ARGUMENTS : Individual * = the individual that you wish to free
 * 
 * PRECONDITIONS : the Individual passed in as a parameter must have been
 *      previously created either through initRandIndividual() or 
 *      duplicateIndividual().
 *****************************************************************************/
void freeIndividual(Individual *);



/******************************************************************************
 *****************                MANIPULATION                *****************
 *****************************************************************************/

/******************************************************************************
 * NAME : mutateRandomly
 * 
 * PURPOSE : Perform mutation on an Individual. The probability of mutating one
 *      individual gene (MUTATION_RATE) is pre-determined by the user. The new
 *      allele will be randomly chosen (but cannot be the initial allele)
 * ARGUMENTS : Individual * = the Individual that you wish to mutate
 * 
 * PRECONDITIONS : the Individual passed in as a parameter must have been
 *      previously created either through initRandIndividual() or 
 *      duplicateIndividual().
 *****************************************************************************/
void mutateRandomly(Individual *);

/******************************************************************************
 * NAME : mutateRotationally
 * 
 * PURPOSE : Perform mutation on an Individual. The probability of mutating one
 *      individual gene (MUTATION_RATE) is pre-determined by the user. The 
 *      mutated allele will become an architecture of larger area than the 
 *      original. If there exist no architectures with larger area, then the 
 *      mutated gene becomes the smallest architecture available.
 * ARGUMENTS : Individual * = the Individual that you wish to mutate
 * 
 * PRECONDITIONS : the Individual passed in as a parameter must have been
 *      previously created either through initRandIndividual() or 
 *      duplicateIndividual().
 *****************************************************************************/
void mutateRotationally(Individual *);



/******************************************************************************
 * NAME : onePointCrossover
 * 
 * PURPOSE : Perform crossover on the two individuals provided. The probability
 *      of mutating one individual gene (CROSSOVER_RATE) is pre-determined by
 *      the user. onePointCrossover chooses one division point in the 
 *      chromosome with each child inheriting the first half of one parent's
 *      chromosome, and the second half of the other parent
 * ARGUMENTS : Individual * = the first individual that will undergo crossover
 *             Individual * = the second individual that will undergo crossover
 * 
 * PRECONDITIONS : the Individuals passed in as a parameter must have been
 *      previously created either through initRandIndividual() or 
 *      duplicateIndividual().
 *****************************************************************************/
void onePointCrossover(Individual *, Individual *);

/******************************************************************************
 * NAME : twoPointCrossover
 * 
 * PURPOSE : Perform crossover on the two individuals provided. The probability
 *      of mutating one individual gene (CROSSOVER_RATE) is pre-determined by
 *      the user. twoPointCrossover chooses two division points in the 
 *      chromosome. Children are generated by swapping the alleles between the
 *      two division points on each parent.
 * ARGUMENTS : Individual * = the first individual that will undergo crossover
 *             Individual * = the second individual that will undergo crossover
 * 
 * PRECONDITIONS : the Individuals passed in as a parameter must have been
 *      previously created either through initRandIndividual() or 
 *      duplicateIndividual().
 *****************************************************************************/
void twoPointCrossover(Individual *, Individual *);



/******************************************************************************
 * NAME : printIndividual
 * 
 * PURPOSE : Print an Individual's genotype and fitness information to stdout.
 * ARGUMENTS : Individual * = The individual that you wish to visualize
 *****************************************************************************/
void printIndividual(Individual *);



#endif	/* INDIVIDUAL_H */

