/*
 * rcsSimulator.h
 *
 *  Created on: Jun 24, 2013
 *      Author: aalwatta
 */

#ifndef RCSSIMULATOR_H_
#define RCSSIMULATOR_H_

#include "common_interfaces.h"

#define PRINT_DFG_DATA 0x01
#define TASK_MIGRATION 0x02
#define RCSSCHED_I 	   0x04
#define RCSSCHED_II    0x08
#define RCSSCHED_III   0x10
#define GENERATE_TASK_GRAPH 0x20
#define NO_SEARCH_Q	   0x40

#define QUEUE_SIZE 	   25

#define INIT_FLAGS(flag) ((flag)= 0x0)
#define SET_FLAG_VALUE(x,flag)  ((x)|=(flag))
#define IS_FLAG_TRUE(x,flag) (((x)&(flag))&& 1)
struct SimData
{
	unsigned int flags;
	int noOfNodes;
	int typeData[500];
	int iteration;

};

struct SimResults
{
	unsigned int totalTime;
	unsigned int noOfReuse;
	unsigned int noOfConfiguration;
	unsigned int noSW2HWMigration;
	unsigned int noHW2SWMigration;
	unsigned int noOfSWTasks;
	unsigned int noSWBusyCounter;
	unsigned int noHWBusyCounter;
        unsigned int power;
};



void initPRRsConfigTime(unsigned int * prrTime, int noPRRs);
int InitSimulator(Common_Interface *);
int CleanSimulator();
int RunSimulator(struct SimData *simData, struct SimResults *simResults);




#endif /* RCSSIMULATOR_H_ */
