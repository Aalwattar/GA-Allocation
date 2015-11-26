/*
 * io.c
 *
 *  Created on: Jul 10, 2013
 *      Author: aalwatta
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"
#include "PlatformConfig.h"
#include "schedulers.h"
#include "common_interfaces.h"

#include "io.h"

int convertDFG(struct node *dFG, DFG *rawDFG)
{
    int i;
    int tmp;

    int *deparr=calloc(rawDFG->num_regs,sizeof(*deparr));

    for (i=0; i<rawDFG->num_nodes; i++) {

        if(rawDFG->node[i].output[0]=='r' || rawDFG->node[i].output[0]=='R') {
        	tmp=strtol(rawDFG->node[i].output+1,NULL,10);
            deparr[tmp]=i;
            if (rawDFG->num_regs<=tmp)
            {
            	fprintf(stderr, "ERROR: convertDFG Register ID out of range node[%d]->Reg[%d]\n",i,tmp);
            	exit(EXIT_FAILURE);
            	           }
        //    fprintf(stderr,"[%d]->[%d] \n",tmp,deparr[tmp]);
        }

    }

    for (i=0; i<rawDFG->num_nodes; i++) {

        SetNodeTaskType(dFG,i,rawDFG->node[i].task_type);
        SetNodeID(dFG,i,i);
        SetNodeMode(dFG,i,HybHW);
        SetNodeCanRun(dFG,i,0xFF);
        SetNodeNext(dFG,i,i+1);
        SetNodeArch(dFG,i,0);
        SetNodePower(dFG,i,0);
        if(rawDFG->node[i].inputs[0][0]=='r' || rawDFG->node[i].inputs[0][0]=='R') {

            SetNodeOp1Value(dFG,i,deparr[strtol(&(rawDFG->node[i].inputs[0][0])+1,NULL,10)]);
            SetNodeOp1Address(dFG,i,YES);


        } else {
            SetNodeOp1Value(dFG,i,0);
            SetNodeOp1Address(dFG,i,NO);
        }


        if(rawDFG->node[i].inputs[1][0]=='r' || rawDFG->node[i].inputs[1][0]=='R') {

            SetNodeOp2Value(dFG,i,(int)deparr[strtol(&(rawDFG->node[i].inputs[1][0])+1,NULL,10)]);
            SetNodeOp2Address(dFG,i,YES);

        } else {
            SetNodeOp2Value(dFG,i,0);
            SetNodeOp2Address(dFG,i,NO);
        }

//        fprintf(stderr,"Task#%3d op1[%3d] add[%3s], op2[%3d] add[%3s] type[%3d]\n",
//        		GetNodeID(dFG,i),
//        		GetNodeOp1Value(dFG,i),IsNodeOp1Address(dFG,i)?"YES":"NO",
//        		GetNodeOp2Value(dFG,i),IsNodeOp2Address(dFG,i)?"YES":"NO",
//        		GetNodeTaskType(dFG,i));
    }
    SetNodeNext(dFG,i-1,0);;
free(deparr);
    return EXIT_SUCCESS;
}


int cmpfunc (const void * a, const void * b)
{
    return (int)( *(unsigned int*)a - *(unsigned int*)b );
}

unsigned int * init_pRRrConfigValues(unsigned int *srcarr ,int size)
{
    unsigned int *dstarr=malloc(sizeof( *dstarr) *size );
    int i;
    for (i=0; i<size; i++) {
        dstarr[i]=srcarr[i];
    }

    return dstarr;
}
void initPRRsConfigTime(unsigned int * prrTime, int noPRRs)
{
    int i;
    for (i = 0; i < noPRRs; ++i) {

        InitPRRConfigTime(i, prrTime[i]);

    }
}

//void initTaskTypeData(Common_Interface* mn) {
//	int j, i, index = 1;
//	for (i = 0; i < mn->archlib.num_tasks; ++i) {
//
//		for (j = 0; j < mn->archlib.task[i].num_impl; ++j, index++) {
//
//			setTaskTypeDataArchNo(index,j);
//			setTaskTypeDataTaskType(index,i);
//			if (mn->archlib.task[i].impl[j].mode[0] == 'H') {
//
//				setTaskTypeDataHWDelay(index,
//				mn->archlib.task[i].impl[j].exec_time);
//				setTaskTypeDataSWDelay(index, 0);
//				setTaskTypeDataConfTime(index,mn->archlib.task[i].impl[j].config_time);
//			} else {
//				setTaskTypeDataSWDelay(index,
//						mn->archlib.task[i].impl[j].exec_time);
//				setTaskTypeDataHWDelay(index, 0);
//				setTaskTypeDataConfTime(index,0);
//			}
//
////			fprintf(stderr, "Task%d impl%d exe %d\n", index, j,
////					getTaskTypeDataConfTime(index));
//		}
//
//	}
//}



void initTaskModeandSWtime(Common_Interface* mn, struct node *dFG) {
	int j, i;
	int foundSW = 0, foundHW = 0;
	char tmpchar;

	int taskType;

	for (i = 0; i < mn->dfg.num_nodes; i++) {
		taskType = GetNodeTaskType(dFG, i)-1;
		foundSW = foundHW = NO;
		for (j = 0; j < mn->archlib.task[taskType].num_impl; ++j) {
			tmpchar = mn->archlib.task[taskType].impl[j].mode[0];

			if ('S' == tmpchar || 's' == tmpchar) {

				SetNodeEmulationSWdelay(dFG, i,
						mn->archlib.task[taskType].impl[j].exec_time);
				foundSW = YES;
			} else if ('H' == tmpchar || 'h' == tmpchar) {
				foundHW = YES;
			} else {
				fprintf(stderr,
						"ERROR [initTaskModeandSWtime] Unknown mode in "
						"arch file task[%d] arch [%d]\n",
						taskType, j);
				exit(EXIT_FAILURE);
			}


		}
		SetNodeEmulationHWdelay(dFG, i, 0);
		if (foundHW && foundSW) {
			SetNodeMode(dFG, i, HybHW);
		} else if (foundSW) {
			SetNodeMode(dFG, i, SWOnly);
		} else {
			SetNodeEmulationSWdelay(dFG, i, 0);
			SetNodeMode(dFG, i, HWOnly);
		}
//		fprintf(stderr, "task %d, type %d  mode [%s] swExec %d HWExec %d\n", i,
//				taskType + 1,
//				GetNodeMode(dFG, i) == HybHW ? "HybHW" :
//				GetNodeMode(dFG, i) == SWOnly ? "SWOnly" : "HWOnly",
//				GetNodeEmulationSWdelay(dFG, i),
//				GetNodeEmulationHWdelay(dFG, i));

	}
}
unsigned int generateCanRunMask( unsigned int prr)
{
    int i;
    unsigned int mask = 1;
    for (i = 1; i < prr; i++) {
        mask <<= 1;
        mask |= (((unsigned int) 1));
    }
    return mask;
}

void updateCanRun(struct node *dFG,  Architecture_Library *archLib, int numNodes, int numPRRs) {

	int j, i,k;

	for (i = 0; i < numNodes; i++) {
		for (j = 0,k=numPRRs-1; numPRRs > j; j++,k--) {

			if (getConfigTime(k) >= archLib->task[GetNodeTaskType(dFG,i)-1].impl[GetNodeArch(dFG,i)].config_time ) {
				SetNodeCanRun(dFG, i, generateCanRunMask(numPRRs - j));
//				fprintf(stderr, "task %d, canrun %X (%d >%d) \n", i,
//						GetNodeCanRun(dFG, i), getConfigTime(k),
//						archLib->task[GetNodeTaskType(dFG,i)-1].impl[GetNodeArch(dFG,i)].config_time);
				break;
			} else if (numPRRs==j+1) {
				fprintf(stderr,
						"ERROR [updateCanRun] Task %d Cannot fit in any PRR\n",
						i);
//				fprintf(stderr, "ERR task %d, canrun %X (%d >%d) \n", i,
//						GetNodeCanRun(dFG, i), getConfigTime(j),
//						archLib->task[GetNodeTaskType(dFG,i)-1].impl[GetNodeArch(dFG,i)].config_time);

				exit(EXIT_FAILURE);
			}
		}
	}

}

void sortDecend (unsigned int *arr, int size ){
 unsigned int  tmp;
 int i,j;

	qsort(arr, size, sizeof(unsigned int), cmpfunc);
	for (i=0,j=size-1;i<size/2; i++,--j)
	{
		tmp=arr[j];
		arr[j]=arr[i];
		arr[i]=tmp;
	}

}




