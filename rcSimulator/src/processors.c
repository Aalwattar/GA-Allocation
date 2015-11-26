/*
 * processors.c
 *
 *  Created on: 2013-05-14
 *      Author: aalwatta
*/

#include <stdio.h>
#include <stdlib.h>
#include "processors.h"
#include "data.h"
#include "reconfiguration.h"

static unsigned int Timer;


/*
 * TODO change small Functions to inline
 */


/*****************************************************************************
* This function retrieves the Timer
*
* @param        None
*
* @return       u32 ConfigCount Value
*
* @note         None.
*
******************************************************************************/
unsigned int GetTimer(void)
{
        return Timer;
}


/*****************************************************************************
* This function resets (=0) the Timer Counter
*
* @param        None
*
* @return       none
*
* @note         None.
*
******************************************************************************/

void ResetTimer(void)
{
	Timer =0;
}

/*****************************************************************************
* This function Increments the Timer It's
*
* @param        None
*
* @return       None
*
* @note         None.
*
*
*
******************************************************************************/
 void IncTimer(void)
{
	 Timer++;
}






 inline unsigned int  CalcuateExecTime(unsigned int  orig,unsigned int  new)
 {unsigned int  tmp;
 	double ratio;
 	if (!orig)
 		return new;
 	ratio=(((double) (int ) ((int )new - (int )orig)) * EXEC_TIME_LEARNING_RATIO);
 tmp=((int ) orig+ (int ) ratio);


 return tmp;
// return new;
 }


 /* TODO ADD Config time to the PRR instead of to the taskstypes it would be faster
  * This if the SW task ET is shorter than MATH0+Config but longer than MATH0 alone*/
 void CalcSWPrio(unsigned int ID,struct Processor *processor, int size )
 { int i;
 	if(getTaskTypeSWET(ID)==0 || getTaskTypeHWET(ID)==0 ) return;

 	if(getTaskTypeSWET(ID)<getTaskTypeHWET(ID))
 	{
// 		fprintf(stderr,"changing prio to zero for task %d\n",ID);
 		setTaskTypeSWPrio(ID,0);
 		return;
 	}


 	for (i=0 ; i<size ;i++)
 	{

 		if(processor[i].ConfigTime==0) continue;
 		if (getTaskTypeSWET(ID) <= (getTaskTypeHWET(ID)+processor[i].ConfigTime))
 		{
 			setTaskTypeSWPrio(ID,i);
 			return;
 		} else if(getTaskTypeSWPrio(ID) <i)
 		{
 			setTaskTypeSWPrio(ID,i);
 		}


 	}

 }

void InitProcessors(struct Processor* processors, int size,enum ProcessorType type) {
	int i;
	for (i = 0; i < size; i++) {
		processors[i].Busy = NO;
		processors[i].CurrentTaskType = 0;
		processors[i].CurrentArch = 0;
		processors[i].ExecCount = 0;
		processors[i].ConfigCount = 0;
		processors[i].ConfigTime = 0;
		processors[i].ID = 0;
		processors[i].Type = type;
		processors[i].ID = i;

	}
}

struct Processor * CreateProcessors( int size, enum ProcessorType type)
{

struct Processor *processors;
	processors=(struct Processor *)malloc(sizeof(struct Processor ) *size);
  if (!processors) 
  {
    fprintf(stderr, "ERROR [InitProcessors] Allocating memory to processors \n");
    exit(EXIT_FAILURE); 
  }
	InitProcessors(processors,size,type);

	return processors;
}

 void FreeProcessors(struct Processor * processor)
{
	free(processor);
}

 int IsProcessorBusy( struct Processor *processor)
{
	return processor->Busy;
}

void SetProcessorBusy( struct Processor *processor)
{
	processor->Busy=YES;
}

void SetProcessorNotBusy( struct Processor *processor)
{
	processor->Busy=NO;
}

int CheckCurrentTaskType( struct Processor *processor)
{
	return processor->CurrentTaskType;
}


int TickProcessor( struct Processor *processor)
{

	if (processor->Busy && processor->ExecCount)
	{
		if(!(--processor->ExecCount))
		{
			processor->Busy=NO;
		}
		return (int) processor->ExecCount;

	}

	 return -1;
}

int LoadProcessor( struct Processor *processor, struct NodeData node )
{

	if (processor->Busy)
	{
		fprintf(stderr,"ERROR[LoadProcessor] Processor is busy\n");
		return 1;
	}
	setTaskSimExecTimeStart(node.TaskID,GetTimer());

	processor->Busy=YES;
	processor->CurrentTaskType=node.TaskType; /*TODO fix this */
	processor->CurrentArch=node.Arch;
	processor->ExecCount=node.ExecCount;
	processor->CurrentTaskID=node.TaskID;
#if DEBUG_PRINT
	fprintf(stderr,"loading task [%d] Type [%d] with ExecCount [%lu] \n", \
			processor->CurrentTaskID, processor->CurrentTaskType, processor->ExecCount);
#endif
	return 0;
}

int TickAllProcessors(struct Processor *processor, int size, struct node *dFG)
{
	int i;


	for (i =0 ; i< size ; i ++ )
	{

		if(!TickProcessor(processor+i))
			{
#if DEBUG_PRINT
				fprintf(stderr,"Task [%d] running on PRR[%d] is completed \n", processor[i].CurrentTaskID,i);
#endif
				State=TaskDone;
				taskDone(processor[i].CurrentTaskID);
				decTaskCounter();
				setTaskSimExecTimeEnd(processor[i].CurrentTaskID,GetTimer());

				/*
				 * FIXME change the calculation functions for Rcsched_III
				 * they simply does not make any sense.
				 */
#if RCS_SCHED_III
			if (processor[i].Type == TypeHW) {
				setTaskTypeHWET(processor[i].CurrentTaskType,
						CalcuateExecTime(
								getTaskTypeHWET(processor[i].CurrentTaskType),
								GetNodeEmulationHWdelay(dFG,
										processor[i].CurrentTaskID)));

			} else {
				setTaskTypeSWET(processor[i].CurrentTaskType,
						CalcuateExecTime(
								getTaskTypeSWET(processor[i].CurrentTaskType),
								GetNodeEmulationSWdelay(dFG,
										processor[i].CurrentTaskID)));

			}


			CalcSWPrio(processor[i].CurrentTaskType, processor, size);

#endif

		//	task completed TODO call a function that represent the complete of this task
			}

	}

	return 0;
}


unsigned int Ticker(struct PEs *pEs, struct node * dFG)
{
	IncTimer();
	TickAllProcessors(pEs->HWPE->pe, pEs->HWPE->size,dFG);
	TickAllProcessors(pEs->SWPE->pe, pEs->SWPE->size,dFG);
	TickConfig(pEs->HWPE->pe);

	return GetTimer();
}

void CreateAllPEs(struct PEs *pEs,int noOfPRRs, int noOfGPPs)
{
	pEs->HWPE=(struct PE*) malloc(sizeof(struct PE));
	pEs->SWPE=(struct PE*) malloc(sizeof(struct PE));
	pEs->HWPE->size=noOfPRRs;
	pEs->SWPE->size =noOfGPPs;
	pEs->HWPE->pe=CreateProcessors(pEs->HWPE->size,TypeHW );
	pEs->SWPE->pe=CreateProcessors(pEs->SWPE->size, TypeSW);

}

void CleanAllPEs(struct PEs *pEs)
{
	FreeProcessors(pEs->HWPE->pe);
	FreeProcessors(pEs->SWPE->pe);
    free(pEs->HWPE);
	free(pEs->SWPE);

}

