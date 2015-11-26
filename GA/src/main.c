/*******************************************************************************
 * Author   : Jennifer Winer
 * 
 * Project  : A DFG Off-Line Task Scheduler for FPGA
 *              - The Genetic Algorithm for determining the ideal implementation
 *                  for each task's operation
 * 
 * Created  : May 7, 2013
 * Modified : July 24, 2013
 ******************************************************************************/

/*******************************************************************************
 * Filename : main.c
 * 
 * Purpose  : Contains main and executes the GA
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "config.h"
#include "selection.h"
#include "replacement.h"
#include "fitness.h"

// FIX - Don't make these global variables
static int MAX_NUM_GENERATIONS = 150;
static int POP_SIZE = 0;

static int crossover_type = 2;
static int mutation_type = 1;
static int replacement_type = 1;
static int selection_type = 1;


void initParameters(int, char **);
void freeParameters(void);
void setPopSize(int);


int main(int argc, char * argv[]){
    Population * pop, * selected;
    Individual * best_solution;
    int generation_num;
    
    initParameters(argc, argv);
    pop = genSeededPopulation(POP_SIZE);
    
#if (defined DIVERSITY)
    printGeneComposition(pop);
#endif

    determineFitness(pop);
    sortByFitness(pop);

    generation_num = 0;
    while(generation_num < MAX_NUM_GENERATIONS){
        
    #if  (defined VERBOSE || defined DIVERSITY)
        fprintf(stdout, "\n-----------------   GENERATION %d   -----------------\n", generation_num + 1);
        printPopulation(pop);
    #endif
        
        // FIX - use function pointers instead of flags + if statement
        if(selection_type == 1)
            selected = tournamentSelection(pop);
        else 
            selected = randomSelection(pop);
            
        // FIX - use function pointers instead of flags + if statement
        evolvePopulation(selected, crossover_type, mutation_type);
        determineFitness(selected);
        
        // FIX - use function pointers instead of flags + if statement
        if(replacement_type == 1)
            pop = replaceAll(pop, selected);
        else  
            pop = retainBest(pop, selected);

        generation_num++;
    }
    
    fprintf(stdout, "\nFINAL RESULT:\n");
    printPopulation(pop);
    
    fprintf(stdout, "\nBEST SOLUTION:\n");
    best_solution = findBest(pop);
    printIndividual(best_solution);
    
    freePopulation(pop);
    freeParameters();
    return EXIT_SUCCESS;
} 

// FIX - too long (separate into more than one function)
// FIX - Make a function that prints out its usage (help function)
void initParameters(int argc, char ** argv){
    char * arch_filename = DEFAULT_ARCH_FILENAME;
    char * dfg_filename = DEFAULT_DFG_FILENAME;
    char * prr_filename = DEFAULT_PRR_FILENAME;
    int seed = randSeed();
    int c;

    opterr = 0;
    while((c = getopt(argc, argv, "a:b:c:d:f:g:h:m:n:o:p:r:s:t:Vvw:")) != -1){
        switch(c){
            case 'a':
                arch_filename = optarg;
                break;
                
            case 'b':
                if(strncasecmp("one-point", optarg, strlen(optarg))  == 0 || 
                    strncasecmp("one_point", optarg, strlen(optarg)) == 0 ||
                    strncasecmp("one point", optarg, strlen(optarg)) == 0 )
                    crossover_type = 1;
                else if(strncasecmp("two-point", optarg, strlen(optarg)) != 0 &&
                        strncasecmp("two_point", optarg, strlen(optarg)) != 0 &&
                        strncasecmp("two point", optarg, strlen(optarg)) != 0 ){
                    fprintf(stderr, "Unknown crossover option : %s\n", optarg);
                    exit(1);
                }
                break;
                
            case 'c':
                setCrossoverRate(atof(optarg));
                break;
                
            case 'd':
                dfg_filename = optarg;
                break;
                
            case 'f':
                setFitnessFunction(optarg);
                break;
                
            case 'g':
                MAX_NUM_GENERATIONS = atoi(optarg);
                break;
                
            case 'h':
                setSetupIndex(atoi(optarg));
                break;
                
            case 'm':
                setMutationRate(atof(optarg));
                break;
                
            case 'n':
               if(strncasecmp("random", optarg, strlen(optarg)) == 0)
                    mutation_type = 2;
                else if(strncasecmp("rotationally", optarg, strlen(optarg)) != 0){ 
                    fprintf(stderr, "Unknown mutation option : %s\n", optarg);
                    exit(1);
                }
                break;
                
            case 'p':
                setPopSize(atoi(optarg));
                break;
                
            case 'r':
                if(strncasecmp("keep-best", optarg, strlen(optarg))  == 0 ||
                    strncasecmp("keep_best", optarg, strlen(optarg)) == 0 ||
                    strncasecmp("keep best", optarg, strlen(optarg)) == 0)
                    replacement_type = 2;
                else if(strncasecmp("all", optarg, strlen(optarg)) != 0){
                    fprintf(stderr, "Unknown replacement option : %s\n", optarg);
                    exit(1);
                }
                break;

            case 's':
                if(strncasecmp("random", optarg, strlen(optarg)) == 0)
                    selection_type = 2;
                else if(strncasecmp("tournament", optarg, strlen(optarg)) != 0){ 
                    fprintf(stderr, "Unknown selection option : %s\n", optarg);
                    exit(1);
                }
                break;
                
            case 't':
                seed = atoi(optarg);
                break;
                
            case 'V':
            case 'v':
                fprintf(stdout, "Offline Scheduler version 2.0  (GA + Napoleon + rcSimulator)\n");
            #ifdef DEBUG
                fprintf(stdout, "Using the DEBUG build\n\n");
            #endif
            
            #ifdef VERBOSE
                fprintf(stdout, "Using the VERBOSE build\n\n");
            #endif
            
            #ifdef STATS
                fprintf(stdout, "Using the STATS build\n\n");
            #endif
                
                fprintf(stdout, "Please see https://github.com/Aalwattar/GA for more information\n");
                exit(0);
                
            case 'w':
                setRuntimeWeight(atof(optarg));
                break;
                
            case ':':   
                fprintf(stderr, "Option -%c requires an operand\n", optopt);
                break;
            case '?':
                fprintf(stderr, "Unrecognized option: -%c\n", optopt);

            default:
                exit(1);
        }
    }

    if(optind < argc){
        printf("Non-option argument %s\n", argv[optind]);
        exit(1);
    }
    
    // FIX - Check the return value
    seedRandGenerator(seed);
    

    if(initScheduler(arch_filename, dfg_filename, prr_filename) == EXIT_FAILURE)
        exit(1);

    
    if(POP_SIZE == 0){
        POP_SIZE = getNumNodes();
    }
    
    fprintf(stdout, "Parameters:\n");
    fprintf(stdout, "\tFitness Function = %s\n\n", getFitnessFunction());
    fprintf(stdout, "\tPopulation Size       = %d\n", POP_SIZE);
    fprintf(stdout, "\tNumber of Generations = %d\n\n", MAX_NUM_GENERATIONS);
    fprintf(stdout, "\tMutation Rate  = %.4lf\n", getMutationRate());
    fprintf(stdout, "\tCrossover Rate = %.4lf\n", getCrossoverRate());
    fprintf(stdout, "\tRuntime Weight = %.4lf\n", getRuntimeWeight());
    fprintf(stdout, "\tPower Weight   = %.4lf\n\n", 1.0 - getRuntimeWeight());
    
    if(crossover_type == 1)
        fprintf(stdout, "\tCrossover Algorithm   = One point Crossover\n");
    else
        fprintf(stdout, "\tCrossover Algorithm   = Two point Crossover\n");
    
    if(mutation_type == 1)
        fprintf(stdout, "\tMutation Algorithm    = Rotational Mutation\n");
    else
        fprintf(stdout, "\tMutation Algorithm    = Random Mutation\n");
    
    if(selection_type == 1)
        fprintf(stdout, "\tSelection Algorithm   = Tournament Selection\n");
    else
        fprintf(stdout, "\tSelection Algorithm   = Random Selection\n");
    
    if(replacement_type == 1)
        fprintf(stdout, "\tReplacement Algorithm = Replace All\n\n");
    else
        fprintf(stdout, "\tReplacement Algorithm = Keep Best\n\n");
    
    // FIX - make this dynamic
    fprintf(stdout, "\tSeed             = %d\n", seed);
    fprintf(stdout, "\tPercent seeding  = %.4lf\n", (1.0 - PERCENT_POP_RANDOM));
}


void freeParameters(void){
    freeScheduler();
}

void setPopSize(int size){
    if(2 <= size && size <= 10000){
        POP_SIZE = size;
        return;
    }
    
    fprintf(stderr, "Invalid population size %d.\n", size);
    fprintf(stderr, "The population size must be an integer between 0 and 1\n");
    
    exit(1);
}
