/*******************************************************************************
 * Author   : Jennifer Winer
 * 
 * Project  : A DFG Off-Line Task Scheduler for FPGA
 *              - The Genetic Algorithm for determining the ideal implementation
 *                  for each task's operation
 * 
 * Created  : June 17, 2013
 * Modified : July 14, 2013
 ******************************************************************************/

/*******************************************************************************
 * Filename : config.h
 * 
 * Purpose  : a configuration file (to contain all unchanging information)
 ******************************************************************************/


#ifndef CONFIG_H
#define	CONFIG_H


// locations of input files
#define DEFAULT_ARCH_FILENAME "input/arch.conf"
#define DEFAULT_DFG_FILENAME  "input/B1_10_5.conf"
#define DEFAULT_PRR_FILENAME  "input/prr.conf"


// Fitness weights
#define DEFAULT_RUNTIME_WEIGHT 0.875

// Genetic operator rates
#define DEFAULT_CROSSOVER_RATE 0.85
#define DEFAULT_MUTATION_RATE  0.005

// Population Diversity
#define PERCENT_POP_RANDOM 0.75
#define MAX_PHENOTYPE_DEVIATION 0

// misc
#define BUFF_SIZE 500
#define DEFAULT_SETUP_INDEX 2 // FIX - remove this if possible ....

#endif	/* CONFIG_H */

