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
 * Filename : fitness.c
 * 
 * Purpose  : A library of fitness functions
 ******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "fitness.h"

#include "rcsSimulator.h"       // Online Scheduler
#include "offlineScheduler.h"   // Offline Scheduler

#include "common_interfaces.h"      // FIX - change this to one include (or two at absolute maxiumum)
#include "architecture_library.h"
#include "dfg_library.h"
#include "hardware_library.h"

// FIX - Find a better mechanism than 
static double RUNTIME_WEIGHT = DEFAULT_RUNTIME_WEIGHT;
static int SETUP_INDEX = DEFAULT_SETUP_INDEX;
static int OFFLINE_SCHEDULER = 1;

static Common_Interface LIBRARY;
static Hardware HARDWARE;



/******************************************************************************
 *****************             GETTERS AND SETTERS            *****************
 *****************************************************************************/

// Hardware information
void setSetupIndex(int index){
    if(0 <= index && index <= 4){
        SETUP_INDEX = index;
        return;
    }
    
    fprintf(stderr, "Invalid setup index of [%d]\n", index);
    fprintf(stderr, "The hardware index must be a number between 0 and 4 (Please see the prr.conf for more information)\n");
    
    exit(1);
}
// FIX - make a getter and print the PRR information used at the start of the GA


// DFG information
int getNumNodes(void){
    return LIBRARY.dfg.num_nodes;
}
int getTaskType(int task_num){
    return LIBRARY.dfg.node[task_num].task_type - 1;
}


// architecture library information
int getNumArch(int task){
    return LIBRARY.archlib.task[task].num_impl;
}


// fitness function information
void setRuntimeWeight(double weight){
    if(0 <= weight && weight <= 1){
        RUNTIME_WEIGHT = weight;
        return;
    }
    
    fprintf(stderr, "Invalid runtime weight [%.3lf]\n", weight);
    fprintf(stderr, "The runtime rate must be a decimal number between 0 and 1\n");
    exit(1);
}

double getRuntimeWeight(void){
    return RUNTIME_WEIGHT;
}

void setFitnessFunction(char * args){
    if(strncasecmp("onlineSchdeuler", args, strlen(args))  == 0 || 
        strncasecmp("Online Schdeuler", args, strlen(args)) == 0 ||
        strncasecmp("rcScheduler", args, strlen(args)) == 0 ||
        strncasecmp("rcSimulator", args, strlen(args)) == 0 )
            OFFLINE_SCHEDULER = 0;
    
    else if(strncasecmp("offlineScheduler", args, strlen(args)) != 0 &&
            strncasecmp("offline Scheduler", args, strlen(args)) != 0 &&
            strncasecmp("Napoleon", args, strlen(args)) != 0){
                fprintf(stderr, "Unknown crossover option : %s\n", args);
                exit(1);
            }   
}

char * getFitnessFunction(void){
    if(OFFLINE_SCHEDULER)
        return "Offline Scheduler (Napoleon)";
    else 
        return "Online Scheduler (rcScheduler)";
}

/******************************************************************************
 *****************              FITNESS FUNCTION              *****************
 *****************************************************************************/

int initScheduler(char * arch_filename, char * dfg_filename, char * prr_filename){
    if(initArchLibrary(arch_filename, &(LIBRARY.archlib)) != EXIT_SUCCESS){
        fprintf(stderr, "FATAL ERROR: [initArchLibrary] returned EXIT_FAILURE with the argument %s\n", arch_filename);
        return EXIT_FAILURE;
    }
    
    if(initDFG(dfg_filename, &(LIBRARY.dfg)) != EXIT_SUCCESS){
        fprintf(stderr, "FATAL ERROR: [initDFG] returned EXIT_FAILURE with the argument %s\n", dfg_filename);
        return EXIT_FAILURE;
    }
    
    // initialize the appropriate fitness function
    if(OFFLINE_SCHEDULER){
        if(initNapoleon(arch_filename, dfg_filename) != EXIT_SUCCESS){
            fprintf(stderr, "FATAL ERROR: [initNapoleon] returned EXIT_FAILURE\n");
            fprintf(stderr, "Please see Napoleon's documentation for more information.\n");
            return EXIT_FAILURE;
        }
    }
    else{
        if(initHardwareLibrary(prr_filename, &(HARDWARE)) != EXIT_SUCCESS){
            fprintf(stderr, "FATAL ERROR: [initHardwareLibrary] returned EXIT_FAILURE with the argument %s\n", prr_filename);
            return EXIT_FAILURE;
        }
        LIBRARY.setup = HARDWARE.setups[SETUP_INDEX];

        // FIX - make rcSimulator's return value more descriptive ... 
        if(InitSimulator(&LIBRARY) != 1){
            fprintf(stderr, "FATAL ERROR: [InitSimulator] returned EXIT_FAILURE\n");
            fprintf(stderr, "Please see rcSimulator's documentation for more information.\n");
            return EXIT_FAILURE;
        }
    }
    
    // FIX - Change the above system into a series of function pointers instead of if statements
    
    // FIX - change verbose / exe flags into command line arguments with function pointers
    //          eg. if the pointer is null, don't print anything. Else use the printing function provided each generation????
    
    return EXIT_SUCCESS;
}


// FIX - make this funciton return all the information required to initialize an individual without setting it itself
void evaluateFitness(Individual * ind){
    struct SimData input;
    struct SimResults output;
    int i;
    
    INIT_FLAGS(input.flags);
    input.noOfNodes = getNumNodes();
    for(i=0; i<input.noOfNodes; i++)
        input.typeData[i] = ind->encoding[i];
    
    // FIX - change this to function pointers instead
    if(OFFLINE_SCHEDULER)
        getSchedule(&input, &output);
    else
        RunSimulator(&input, &output);
    
    ind->fitness = (output.totalTime * RUNTIME_WEIGHT) + (output.power * (1.0 - RUNTIME_WEIGHT));
    ind->energy = output.power;
    ind->exec_time = output.totalTime;
    ind->num_reuse = output.noOfReuse;
    
    if(OFFLINE_SCHEDULER)
        ind->prefetch = output.noOfConfiguration;
    else
        ind->prefetch = -1;
}

// DEPRECIATED - for reference purposes only!
/* void evaluateOnlineFitness(Individual * ind){
    struct SimData input;
    struct SimResults output;
    int i;
    
    // Zero the output
    output.totalTime = 0;
	output.noOfReuse = 0;
	output.noOfConfiguration = 0;
	output.noSW2HWMigration = 0;
	output.noHW2SWMigration = 0;
	output.noOfSWTasks = 0;
	output.noSWBusyCounter = 0;
	output.noHWBusyCounter = 0;

    //initialize the input
    input.dFGID = 0;    // FIX - NO LONGER IN USE
    input.noPRR = 5;    // FIX - NO LONGER IN USE
    input.noGPP = 0;    // FIX - NO LONGER IN USE
    
    input.noOfNodes = getNumNodes();
    for(i=0; i<input.noOfNodes; i++)
        input.typeData[i] = ind->encoding[i];

	RunSimulator(&input, &output);
    
    ind->fitness = output.totalTime;
    ind->energy = 0;    
    ind->exec_time = output.totalTime;
    ind->num_reuse = 0;
    ind->prefetch = 0;
}

// FIX - add error checkingThis has
void evaluateOfflineFitness(Individual * ind){
    struct SimData input;
    struct SimResults output;
    int i;

    input.noOfNodes = getNumNodes();
    for(i=0; i<input.noOfNodes; i++)
        input.typeData[i] = ind->encoding[i];
    
    // FIX - check return value
    getSchedule(&input, &output);

    ind->fitness = (output.totalTime * RUNTIME_WEIGHT) + (output.power * (1.0 - RUNTIME_WEIGHT));
    ind->energy = output.power;
    ind->exec_time = output.totalTime;
    ind->num_reuse = output.noOfReuse;
    ind->prefetch = output.noOfConfiguration;
}
*/


void freeScheduler(void){
    // FIX - change this to function pointers instead
    if(OFFLINE_SCHEDULER)
        freeNapoleon();
    else{
        CleanSimulator();
        freeHardwareLibrary(&HARDWARE);
    }
    
    freeArchLibrary(&(LIBRARY.archlib));
    freeDFG(&(LIBRARY.dfg));
}