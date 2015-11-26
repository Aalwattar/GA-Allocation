/*******************************************************************************
 * Author   : Jennifer Winer
 * 
 * Project  : A DFG Off-Line Task Scheduler for FPGA
 *              - The Genetic Algorithm for determining the ideal implementation
 *                  for each task's operation
 * 
 * Created  : May 16, 2013
 * Modified : July 13, 2013
 ******************************************************************************/

/*******************************************************************************
 * Filename : replacement.h
 * 
 * Purpose  : a header for replacement.c
 ******************************************************************************/


#ifndef REPLACEMENT_H
#define	REPLACEMENT_H

#include "population.h"

/******************************************************************************
 * NAME : replaceAll
 * 
 * PURPOSE : Replaces all of the individuals in a population with their
 *              children
 * ARGUMENTS : Population * = the destination population
 *             Population * = the source population
 * 
 * PRECONDITIONS : Both populations must have already had the fitnesses of
 *                      all of their members evaluated (through evaluateFitness)
 *****************************************************************************/
Population * replaceAll(Population *, Population *);

/******************************************************************************
 * NAME : replaceWorst
 * 
 * PURPOSE : Replaces the worst individuals in one population with 
 *              the best individuals from another population12
 * ARGUMENTS : Population * = the destination population
 *             Population * = the source population
 *             int = the number of individuals that you wish to replace
 * 
 * PRECONDITIONS : Both populations must have already had the fitnesses of
 *                      all of their members evaluated (through evaluateFitness)
 * 
 * NOTE : This algorithm sorts each of the populations according to fitness
 *****************************************************************************/
Population * replaceWorst(Population *, Population *, int);


/******************************************************************************
 * NAME : retainBest
 * 
 * PURPOSE : Replaces the current population with the fittest individuals from
 *              both the children and parent populations.
 * ARGUMENTS : Population * = the destination population
 *             Population * = the source population
 * 
 * PRECONDITIONS : Both populations must have already had the fitnesses of
 *                      all of their members evaluated (through evaluateFitness)
 *****************************************************************************/
Population * retainBest(Population *, Population *);

#endif	/* REPLACEMENT_H */

