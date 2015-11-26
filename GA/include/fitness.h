/*******************************************************************************
 * Author   : Jennifer Winer
 * 
 * Project  : A DFG Off-Line Task Scheduler for FPGA
 *              - The Genetic Algorithm for determining the ideal implementation
 *                  for each task's operation
 * 
 * Created  : May 16, 2013
 * Modified : July 12, 2013
 ******************************************************************************/

/*******************************************************************************
 * Filename : fitness.h
 * 
 * Purpose  : a header for fitness.c
 ******************************************************************************/


#ifndef FITNESS_H
#define	FITNESS_H

// FIX - try to eliminate the dependency on a "higher level" class
#include "individual.h"


/******************************************************************************
 *****************             GETTERS AND SETTERS            *****************
 *****************************************************************************/

/******************************************************************************
 * NAME : setSetupIndex
 * 
 * PURPOSE : To choose with setup within the array of hardwares that 
 * ARGUMENTS : int = the index of the hardware setup that you wish to test
 * 
 * PRECONDITIONS : The index must be a value between 0 and 
 *                      (the # of setups in prr.conf) - 1
 * RETURNS : nothing (an invalid index will result in an exit(1);
 * 
 * NOTE : INDICES START AT 0 NOT 1 !!!
 *****************************************************************************/
void setSetupIndex(int);

/******************************************************************************
 * NAME : getNumNodes
 * 
 * PURPOSE : getter for the number of nodes in the DFG that we are trying to 
 *              schedule (AKA the number of genes in a chromosome)
 * 
 * PRECONDITIONS : This function should ONLY be called after initScheduler();
 * 
 * RETURNS : the number of genes in a chromosome
 *****************************************************************************/
int getNumNodes(void);

/******************************************************************************
 * NAME : getTaskType
 * 
 * PURPOSE : getter for the task type that a node in the DFG corresponds to
 * ARGUMENTS : int = the position of the node (task) in the DFG
 * 
 * PRECONDITIONS : This function should ONLY be called after initScheduler();
 * 
 * RETURNS : the task type of the input node
 *****************************************************************************/
int getTaskType(int);

/******************************************************************************
 * NAME : getNumArch
 * 
 * PURPOSE : return the number of architectures that exist for that task type
 * ARGUMENTS : int = the task type
 * 
 * PRECONDITIONS : This function should ONLY be called after initScheduler();
 * RETURNS : The number of hardware implementations available for a specific
 *              task type
 *****************************************************************************/
int getNumArch(int);  



/******************************************************************************
 * NAME : setRuntimeWeight
 * 
 * PURPOSE : The fitness function is derived of (X * schedule runtime), and 
 *              ( (1 - X) * power). This function sets the value of X.
 * ARGUMENTS : double = The weight of the runtime when calculating a
 *                        chromosome's fitness
 * 
 * PRECONDITIONS: The argument must be a number between 0 and 1 inclusive
 *****************************************************************************/
void setRuntimeWeight(double);

/******************************************************************************
 * NAME : getRuntimeWeight
 * 
 * PURPOSE : return the weight of runtime from the schedule that the chromosome
 *              produces when calculating a chromosome's fitness
 * 
 * RETURNS : runtime's weight
 *****************************************************************************/
double getRuntimeWeight(void);


/******************************************************************************
 * NAME : setFitnessFunction
 * 
 * PURPOSE : To set whether the user wishes to use the online scheduler
 *              (rcsSimulator) or the offlineScheduler (Napoleon)
 * ARGUMENTS : char * = a string (command line argument) that specifies the
 *                      scheduler that should be used as a fitness function
 * 
 * NOTE : If the argument does not match any existing scheduler, this function
 *              will cause the program to terminate via an exit(1);
 *****************************************************************************/
void setFitnessFunction(char *);

/******************************************************************************
 * NAME : getFitnessFunction
 * 
 * PURPOSE : return the name of the scheduler that the GA is using as its
 *              fitness function 
 * 
 * RETURNS : A string that contains the name of the scheduler used as the
 *              fitness function
 *****************************************************************************/
char * getFitnessFunction(void);

/******************************************************************************
 *****************              FITNESS FUNCTION              *****************
 *****************************************************************************/

/******************************************************************************
 * NAME : initScheduler
 * 
 * PURPOSE : Create and initialize all of the fitness function's static data
 *              structures
 * ARGUMENTS : char * = the name + relative path to the architecture information
 *                      file (arch.conf is the default)
 *             char * = the name + relative path to the DFG you are trying to
 *                      schedule (B1_10_5.conf is the default)
 *             char * = the name + relative path to the hardware information
 *                      file (prr.conf is the default)
 * 
 * RETURNS : EXIT_SUCCESS if the fitness function was successfully initialized
 *           EXIT_FAILURE if anything has gone wrong (with an error message 
 *              printed to stderr)
 *****************************************************************************/
int initScheduler(char *, char *, char *);


/******************************************************************************
 * NAME : evaluateFitness
 * 
 * PURPOSE : Evaluate the fitness of one possible solution (AKA a chromosome, 
 *              AKA a series of architectures or implementations)
 * ARGUMENTS : Individual * = the possible solution to be evaluated
 *****************************************************************************/
void evaluateFitness(Individual *);

/******************************************************************************
 * NAME : freeNapoleon
 * 
 * PURPOSE : free all of the fitness function's static data structures
 * PRECONDITION : This function should only be called if initScheduler() 
 *                  returned true.
 *****************************************************************************/
void freeScheduler(void);

#endif	/* FITNESS_H */

