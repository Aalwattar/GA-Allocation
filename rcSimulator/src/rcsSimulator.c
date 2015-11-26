/*
 * rcsSimulato+1r.c
 *
 *  Created on: Jun 24, 2013
 *      Author: aalwatta
 */
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include "data.h"
#include "processors.h"
#include "reconfiguration.h"
#include "PlatformConfig.h"
#include "queue.h"
#include "schedulers.h"
#include "displayResults.h"
#include "drawTaskAllocation.h"
#include "argparse.h"
#include "rcsSimulator.h"
#include "common_interfaces.h"
#include "io.h"

//static int TypeIndexMap[8] = { 0, ARCH_1, ARCH_2, ARCH_3, ARCH_4, ARCH_5,
//                               ARCH_6, ARCH_7
//                             };

struct global_rcsSimualator {
	int noPRRs;
	int noGPPs;
	struct PEs pEs;
	struct node *dFG;
	int dfgSize;
	Architecture_Library *arch;
	int tableIndexMap[BUFFER_SIZE];

};

static struct global_rcsSimualator Global_local;
void calculateTaskIndex(Common_Interface*);
int calculatePower(struct node *, Architecture_Library *, int );



int InitSimulator(Common_Interface *mn) { /*FIXME fix tmprrval */


	unsigned int *pRRConfigValues;
	Global_local.dfgSize = mn->dfg.num_nodes;
	Global_local.noGPPs = mn->setup.num_gpps;
	Global_local.noPRRs = mn->setup.num_prrs;
	Global_local.arch = &mn->archlib;

   unsigned int tmpprrval[100];
   int i;
	for (i = 0; i < Global_local.noPRRs; i++) {
		tmpprrval[i] = mn->setup.prr_config_time[i];
	}

	calculateTaskIndex(mn);
	//initTaskTypeData(mn);
	Global_local.dFG = CreateDFG(Global_local.dfgSize);
	convertDFG(Global_local.dFG, &(mn->dfg));
	pRRConfigValues = init_pRRrConfigValues(tmpprrval, Global_local.noPRRs);
	sortDecend (pRRConfigValues, Global_local.noPRRs );
	CreateAllPEs(&Global_local.pEs, Global_local.noPRRs, Global_local.noGPPs);
	initPRRsConfigTime(pRRConfigValues, Global_local.noPRRs);

	 initTaskModeandSWtime(mn, Global_local.dFG);
	free(pRRConfigValues);
	return 1;
}

int CleanSimulator() {
	CleanAllPEs(&(Global_local.pEs));
	CleanDFG(Global_local.dFG);
	/*TODO Clear Global_local.dfg*/

	return 1;
}

void loadArchectures(int dFGsize, struct node* dFG, struct SimData* simData) {

	int l;
	char tmpmode;
	for (l = 0; l < dFGsize; l++) {
		//		SetNodeTaskType(dFG, l,
		//				(Global_local.tableIndexMap[GetNodeTaskType(dFG, l)]
		//						+ simData->typeData[l]));
		SetNodeArch(dFG, l, simData->typeData[l]);
		tmpmode=Global_local.arch->task[GetNodeTaskType(dFG, l) - 1].impl[simData->typeData[l]].mode[0];
		if ( SWOnly==GetNodeMode(dFG,l))
		{
			continue;
		}
		if (tmpmode	== 'S'	|| tmpmode == 's') {

			fprintf(stderr,
					"ERROR[RunSimulator] Architecture is not hardware for task mode \n");
			exit(EXIT_FAILURE);
		}

		SetNodeEmulationHWdelay(dFG, l,
				Global_local.arch->task[GetNodeTaskType(dFG, l) - 1].impl[simData->typeData[l]].exec_time);

		//SetNodePower(dFG,l,Global_local.arch->task[GetNodeTaskType(dFG, l)].impl[simData->typeData[l]].exec_power);

//		fprintf(stderr, "task %d, type %d Ex %d arch %d \n", l,
//				GetNodeTaskType(dFG, l), GetNodeEmulationHWdelay(dFG, l),
//				GetNodeArch(dFG, l));


	}
}

int RunSimulator(struct SimData *simData, struct SimResults *simResults) {

	struct node *dFG;
	int dFGsize = 0;
	int (*schedule)(Queue , struct Counts *, struct PEs *, struct node *, int)=NULL;

	Queue ReadyQ;
	static struct Counts counters = { 0, 0, 0, 0, 0, 0 };
	int w;

	ReadyQ = CreateQueue(MAX_QUEUE_TASKS);
	Init_TasksTypes(Global_local.arch->num_tasks, Global_local.noPRRs);


	dFGsize = Global_local.dfgSize;
	dFG = CreateDFG(dFGsize);

	if (dFGsize != simData->noOfNodes) {
		fprintf(stderr,
				"The number of nodes does not match The DFG I am using !!!! \n");
		exit(EXIT_FAILURE);
	}

	CopyDFG(dFG, Global_local.dFG, dFGsize);

	loadArchectures(dFGsize,  dFG, simData);
	updateCanRun(dFG,Global_local.arch,dFGsize,Global_local.noPRRs);

	InitProcessors(Global_local.pEs.HWPE->pe, Global_local.pEs.HWPE->size,
			TypeHW);
	InitProcessors(Global_local.pEs.SWPE->pe, Global_local.pEs.SWPE->size,
			TypeSW);
	RstCounters(&counters);

	if (IS_FLAG_TRUE(simData->flags,RCSSCHED_I)) {
		schedule = RCSchedI;
	} else if (IS_FLAG_TRUE(simData->flags,RCSSCHED_II)) {
		schedule = RCSchedII;
	} else if (IS_FLAG_TRUE(simData->flags,RCSSCHED_III)) {
		schedule = RCSchedIII;

	} else {
		fprintf(stderr,
				"ERROR[RunSimulator] You should enable one simulator  \n");
		exit(EXIT_FAILURE);
	}



	for (w = 0; w < simData->iteration; w++) {
#if  SCHED_I_EN
		Init_Rand_Prr(AVAILABLE_PRR);
#endif

#if DEBUG_PRINT
		fprintf(stdout, "Processing: DFG[%d] with [%d] nodes ITERATION [%d] \n",
				i, dFGsize, w);
#endif
		/*
		 * Reset Things before every DFG
		 */

		MakeEmpty(ReadyQ);
		/* Reset Configuration Counter */
		ResetConfigCount();
		setTaskCounter(dFGsize);
		reinitTasksTable(dFGsize);
		ResetTimer();
		RstCounters(&counters);

		/*
		 * Start Processing DFG
		 */


		do {
			/*
			 * TODO Cannot remember Why I have these three states!! Might be
			 * duplicated code!! Double check if they are necessary and rewrite
			 * the code if necessary.
			 */
			switch (State) {
			case CfgDone:

				schedule(ReadyQ, &counters, &Global_local.pEs, dFG,simData->flags);
				Ticker(&Global_local.pEs, dFG);
				State = TaskDone;
				break;
			case TaskDone:
				AddTask2Queue(ReadyQ, dFG, dFGsize);
				schedule(ReadyQ, &counters, &Global_local.pEs, dFG,simData->flags);
				Ticker(&Global_local.pEs, dFG);
				State = TaskDone;
				break;
			case Start:
				AddTask2Queue(ReadyQ, dFG, dFGsize);
				State = TaskDone;
				break;
			case None:
				break;
			default:
				fprintf(stderr, "ERROR: Unknown state ...\n");
				break;
			}

		} while (getTaskCounter());
		if (IS_FLAG_TRUE(simData->flags,PRINT_DFG_DATA)) {
			print_DFG(dFG, Global_local.noPRRs);
		}
		if (IS_FLAG_TRUE(simData->flags,GENERATE_TASK_GRAPH)) {

		struct Draw ** graph;
		unsigned int graphtime;
		FILE *fdGraph; // tex file to show the allocations of task on each PE
		char fileNameST[255];
		sprintf(fileNameST, "%s_dfg_%d_it_%d.txt", GRAPH_FILE, Global_local.dfgSize, w);
		fdGraph = fopen(fileNameST, "w");
		if (!fdGraph) {
			fprintf(stderr, "ERROR[main] Cannot open File %s\n", fileNameST);
			exit(EXIT_FAILURE);
		}

		graphtime = GetTimer() / SCALING;
		graph = CreateDraw(graphtime + 1,
				Global_local.noGPPs + Global_local.noPRRs);
		GenerateGraph(graph, dFG, SCALING,Global_local.noPRRs);
		DrawGraph(graph, graphtime, fdGraph, Global_local.noPRRs,
				Global_local.noGPPs);
		CleanDraw(graph, graphtime + 1);
		fclose(fdGraph);
		}
		simResults->noHW2SWMigration = counters.HW2SWMig;
		simResults->noHWBusyCounter = counters.busyCounterHW;
		simResults->noOfConfiguration = GetConfigCount();
		simResults->noOfReuse = counters.ReuseCounter;
		simResults->noOfSWTasks = counters.SWTasksCounter;
		simResults->noSW2HWMigration = counters.SW2HWMig;
		simResults->noSWBusyCounter = counters.busyCounterSW;
		simResults->totalTime = GetTimer();
        simResults->power = calculatePower(dFG,Global_local.arch,Global_local.noPRRs);

	}

	CleanDFG(dFG);
	CleanTasksTable();
	DisposeQueue(ReadyQ);
return EXIT_SUCCESS;
}

int getSWArch(Architecture_Library *archLib, int tType) {
	int i;
	char archMode;
/* NOTE this picks the first sw arch if there are more than one
 * Probably it should be changed to pick the last just to be consistent with
 * other functions (load sw ex time thingy )
 */
	for (i = 0; i < archLib->task[tType].num_impl; ++i) {
		archMode = archLib->task[tType].impl[i].mode[0];
		if (archMode == 'S' || archMode == 's') {
			return i;
		}
	}
	return -1;
}

int calculatePower(struct node *dFG, Architecture_Library *archLib, int numPRRs) {
	int index = 0;
	int totalPower = 0;
	int taskType;
	int arch;
	do {
		taskType = GetNodeTaskType(dFG, index) - 1;
		arch = GetNodeArch(dFG, index);

		if (numPRRs <= getTaskSimPrrUsed(index)
				|| SWOnly == GetNodeMode(dFG, index)) {
			if (0 > (arch = getSWArch(archLib, taskType))) {
				fprintf(stderr,
						"ERROR[CalculatePower] No Software Architecture WHATTTT!!! \n");
				exit(EXIT_FAILURE);
			}
			totalPower += archLib->task[taskType].impl[arch].exec_power;
			continue;
		}
		totalPower += archLib->task[taskType].impl[arch].exec_power;
		if (IsTaskSimReused(index)) {
			totalPower += archLib->task[taskType].impl[arch].config_power;

		}
	} while (GetNodeNextNode(dFG, index++));

	return totalPower;
}
void calculateTaskIndex(Common_Interface* mn) {
	int i;
	int sum = 0;
	for (i = 0; i <= mn->archlib.num_tasks; ++i) {

		Global_local.tableIndexMap[i] = sum;
		sum += mn->archlib.task[i].num_impl;
	}

}
