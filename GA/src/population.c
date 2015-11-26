/*******************************************************************************
 * Author   : Jennifer Winer
 * 
 * Project  : A DFG Off-Line Task Scheduler for FPGA
 *              - The Genetic Algorithm for determining the ideal implementation
 *                  for each task's operation
 * 
 * Created  : May 7, 2013
 * Modified : July 14, 2013
 ******************************************************************************/

/*******************************************************************************
 * Filename : population.c
 * 
 * Purpose  : A library for the creation and manipulation of a population
 *              of individuals
 ******************************************************************************/

#include "config.h"
#include "population.h"
#include "fitness.h"
#include "selection.h"
// FIX - Make this file use replacement.h instead of from main

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits.h>



/******************************************************************************
 *****************             GETTERS AND SETTERS            *****************
 *****************************************************************************/

// FIX - remove global variables somehow? eg. by making into arguments to function
static double CROSSOVER_RATE = DEFAULT_CROSSOVER_RATE;
static double MUTATION_RATE  = DEFAULT_MUTATION_RATE;

void setCrossoverRate(double rate){
    if(0 <= rate && rate <= 1){
        CROSSOVER_RATE = rate;
        return;
    }
    
    fprintf(stderr, "Invalid crossover rate %.3lf.\n", rate);
    fprintf(stderr, "The crossover rate must be a decimal number between 0 and 1\n");
    
    exit(1);
}

double getCrossoverRate(void){
    return CROSSOVER_RATE;
}


void setMutationRate(double rate){
    if(0 <= rate && rate <= 1){
        MUTATION_RATE = rate;
        return;
    }
    
    fprintf(stderr, "Invalid mutation rate %.3lf.\n", rate);
    fprintf(stderr, "The mutation rate must be a decimal number between 0 and 1\n");
    
    exit(1);
}

double getMutationRate(void){
    return MUTATION_RATE;
}



/******************************************************************************
 *****************            CREATION AND DELETION           *****************
 *****************************************************************************/

Population * genRandPopulation(int pop_size){
    Population * pop;
    int i;
    
    pop = malloc(sizeof(Population));
    pop->member = malloc(sizeof(Individual) * pop_size);
    pop->size = pop_size;

    for(i=0; i < pop_size; i++)
        initRandIndividual(&(pop->member[i]));

    fprintf(stdout, "\tAll solutions generated randomly"); 
    return pop;
}


Population * genSeededPopulation(int pop_size){
    Population * pop;
    int num_random;
    int i;
        
    pop = malloc(sizeof(Population));
    pop->member = malloc(sizeof(Individual) * pop_size);
    pop->size = pop_size;

    num_random = 0;
    for(i=0; i < pop_size; i++){
    	if(randomNumber() <= PERCENT_POP_RANDOM){
            initRandIndividual(&(pop->member[i]));
            num_random++;
    	}else{
    		initSeededIndividual(&(pop->member[i]));
    	}
    }

    fprintf(stdout, "\tSeeded Solutions = [%d] out of [%d]\n", pop_size - num_random, pop_size);
    return pop;
}

void freePopulation(Population * pop){
    int i;
    
    for(i = 0; i < pop->size; i++)
        freeIndividual(&(pop->member[i]));
    
    free(pop->member);
    free(pop);
}



/******************************************************************************
 *****************                MANIPULATION                *****************
 *****************************************************************************/

void determineFitness(Population * pop){
    int i;
     
    for (i = 0; i < pop->size; i++)
        evaluateFitness(&(pop->member[i]));
    
    // FIX - make the above function have a wrapper in individual.c 
    //       (This file should never reference fitness.c directly)

}

// The compare function for qsort
int compare(const void * p1, const void * p2){
    return ((Individual *)p1)->fitness - ((Individual *)p2)->fitness;
}

void sortByFitness(Population * pop){
    qsort(pop->member, pop->size, sizeof(Individual), compare);
}


void evolvePopulation(Population * pop, int crossover_type, int mutation_type){
    int i;
    
    for(i=0; i + 1 < pop->size; i = i + 2)
        // NOTE - I chose to check CROSSOVER_RATE here instead of within the crossover
        //        functions to reduce the overhead associated with every function call
        if(randomNumber() < CROSSOVER_RATE){
            if(crossover_type == 1)
                onePointCrossover(&(pop->member[i]), &(pop->member[i + 1]));
            else
                twoPointCrossover(&(pop->member[i]), &(pop->member[i + 1]));
        }
                
    if(mutation_type == 1)
        for(i=0; i < pop->size; i++)
            mutateRotationally(&(pop->member[i]));
    else
        for(i=0; i < pop->size; i++)
            mutateRandomly(&(pop->member[i]));
}



/******************************************************************************
 *****************               VISUALIZATION                *****************
 *****************************************************************************/

void printPopulation(Population * pop){
    int i;

    for (i = 0; i < pop->size; i++)
        printIndividual(&(pop->member[i]));

    printSummaryStatistics(pop);
    
#if (defined DIVERSITY)
    printPopDiversity(pop);
#endif
}

void printSummaryStatistics(Population * pop){
    double mean;
    double differenceSum;
    double sd;
    
    int max = 0;
    int min = INT_MAX;
    int fitnessSum;
    int i;
    
    fitnessSum = 0;
    for(i = 0; i < pop->size; i++){
        
        fitnessSum = fitnessSum + pop->member[i].fitness;
        
        if(pop->member[i].fitness > max)
            max = pop->member[i].fitness;
        
        if(pop->member[i].fitness < min)
            min = pop->member[i].fitness;
    }
    
    mean = (double) fitnessSum / pop->size;
    
    differenceSum = 0;
    for(i = 0; i < pop->size; i++)
        differenceSum = differenceSum + pow(mean - pop->member[i].fitness, 2);
    
    sd = sqrt(differenceSum / (pop->size-1));
    
    fprintf(stdout, "\nAverage = %.3lf\n", mean);
    fprintf(stdout, "SD      = %.3lf\n", sd);
    fprintf(stdout, "Min     = %d\n", min);
    fprintf(stdout, "Max     = %d\n", max);
    
//    // concise version
//    fprintf(stdout, "Stats : %.5lf,\t%.5lf,\t%d,\t%d\n", mean, sd, min, max);
}


// CREATED BY: Ahmed Al-Wattar
void printPopDiversity(Population * pop) {
    static int old_ind_distance;    // the previously calculated ind_distance
    int ind_distance; // the hamming distance between a pair of individuals
	int pop_distance; // the sum of the hamming distance between each pair of individuals in the population
	int sum_hamming;  // sum of the hamming distances of (the hamming distances between two individuals)
    int hamming_normalizer;

    int *uniqueFitness; // measures if a phenotype (fitness) is unique
    int sum_unique;
    double percent_unique;
    int i, g, j;
    
    pop_distance = 0;
    sum_hamming = 0;
    uniqueFitness = calloc(pop->size, sizeof(int));
    
    for(i=0; i < pop->size - 1; i++) {
		for (j= i+1; j < pop->size; j++) {
            
            // calculate the hamming distance between a pair of individuals
            ind_distance = 0;
            for (g=0; g < getNumNodes(); g++)
                if((pop->member[i]).encoding[g] != (pop->member[j]).encoding[g])
                    ind_distance++;
            
            pop_distance = pop_distance + ind_distance;
            
            // sum the hamming distance of the hamming distances
            if ((i != 0) && (j != 1) && (ind_distance != old_ind_distance))
				sum_hamming++;
                        
            // calculate the number of different phenotypes
            if (abs((pop->member[i]).fitness - (pop->member[j]).fitness) <= MAX_PHENOTYPE_DEVIATION)
                uniqueFitness[j] = 1;
            
			old_ind_distance = ind_distance;
		}
	}
    
    // calculate the percentage of unique individuals in the population
    sum_unique = 0;
    for (i=0; i < pop->size; i++)
        if(uniqueFitness[i] == 0)
            sum_unique++;
    percent_unique = (double) sum_unique / pop->size;
    
    
    hamming_normalizer = (pop->size * pop->size * getNumNodes()) / 2.0;
	fprintf(stdout,"\nHammig distance sum [%f] h[%f] --- Unique % -->[%f]\n\n",
                    (double) pop_distance / hamming_normalizer,
                    (double) sum_hamming / hamming_normalizer,
                    percent_unique);
    
    free(uniqueFitness);
}

// CREATED BY: Ahmed Al-Wattar
#define MAX_NUM_TYPES 10 // FIX - make this dynamic!!!
void printGeneComposition(Population * pop) {
    int ** occurrence;
    int num_genes = getNumNodes();
    int num_types = MAX_NUM_TYPES;   // FIX
    int g, i, t;
    
	occurrence = malloc(sizeof(int *) * num_genes);
    for(i=0; i < num_genes; i++)
        occurrence[i] = calloc(num_types, sizeof(int));
        
    // count the occurrence of each allele
	for(i=0; i < pop->size; i++)
        for(g=0; g < num_genes; g++)
            occurrence[g][(pop->member[i]).encoding[g]]++;

    // print the occurrence data
	for (g = 0; g < num_genes; g++) {
		fprintf(stdout,"\nGene[%d]---> \t", g);
        
		for (t=0; t < getNumArch(getTaskType(g)); t++)
			fprintf(stdout, "\t[%%%d]",(int)((100 * occurrence[g][t]) / (float) pop->size));
	}
    fprintf(stdout, "\n");
    
    for(i=0; i < num_genes; i++)
        free(occurrence[i]);
    free(occurrence);
}