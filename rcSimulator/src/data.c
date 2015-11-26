/*
 * data.c

 *
 *  Created on: Jun 1, 2011
 *      Author: ahmed
 */
#include <stdlib.h>
#include <stdio.h>
#include "data.h"
#include "PlatformConfig.h"

/*FIXME */





static struct nodeData TasksTable[MAX_NO_OF_NODES];
static int TaskCounter=0;
static struct TaskType TasksTypes[MAX_TASKS_TYPES];
enum SystemStates State=Start;



/*
 * TaskTable Struct functions
 */

void reinitTasksTable( int numberOfTasks)
{
	int i;

	for (i=0;i<numberOfTasks;i++)
	{
		TasksTable[i].Done=NO;
		TasksTable[i].Q=NO;
		TasksTable[i].result=0;
		TasksTable[i].Sim.Reused=0;
		TasksTable[i].Sim.PRRUsed=0;

		TasksTable[i].Sim.ConfigTime.start=0;
		TasksTable[i].Sim.ConfigTime.end=0;


		TasksTable[i].Sim.ExecTime.start=0;
		TasksTable[i].Sim.ExecTime.end=0;



	}
}
inline void setTaskCounter(int value)
{
	TaskCounter=value;
}
inline void decTaskCounter(void)
{
	TaskCounter--;
}
inline int getTaskCounter(void)
{
	return TaskCounter;
}

inline void CleanTasksTable(void)
{
	/*
	 * FIXME Init and clean taskTable
	 */


	//free(TasksTable);
}

inline unsigned char  isTaskDone(int ID)
{
	return TasksTable[ID].Done;
}

inline void taskDone(int ID)
{
	 TasksTable[ID].Done=YES;
}

inline unsigned char  isTaskQed(int ID)
{

	return TasksTable[ID].Q;
}
inline void taskQed(int ID)
{
	TasksTable[ID].Q=YES;
}

inline int getTaskResult( int ID)
{
	return TasksTable[ID].result;
}

inline void setTaskResult (int ID,int value)
{
	TasksTable[ID].result=value;
}


inline enum Mode getTaskMode( int ID)
{
	return TasksTable[ID].mode;
}

inline void setTaskMode (int ID,enum Mode value)
{
	TasksTable[ID].mode=value;
}

void getTaskSimulation( int ID,struct Simulation *value)
{
	value->Reused=TasksTable[ID].Sim.Reused ;
	value->PRRUsed=TasksTable[ID].Sim.PRRUsed ;

	value->ConfigTime.start=TasksTable[ID].Sim.ConfigTime.start;
	value->ConfigTime.end=TasksTable[ID].Sim.ConfigTime.end;


	value->ExecTime.start=TasksTable[ID].Sim.ExecTime.start;
	value->ExecTime.end=TasksTable[ID].Sim.ExecTime.end;


}

void setTaskSimExecTimeStart(int ID, unsigned int value )
{
	TasksTable[ID].Sim.ExecTime.start=value;

}

void setTaskSimExecTimeEnd(int ID, unsigned int value )
{
	TasksTable[ID].Sim.ExecTime.end=value;
}

void setTaskSimConfTimeStart(int ID, unsigned int value )
{
	TasksTable[ID].Sim.ConfigTime.start=value;


}

void setTaskSimConfTimeEnd(int ID, unsigned int value )
{
		TasksTable[ID].Sim.ConfigTime.end=value;

}


void setTaskSimReused(int ID, unsigned char value )
{
	TasksTable[ID].Sim.Reused=value;


}

int IsTaskSimReused(int taskID )
{
	return TasksTable[taskID].Sim.Reused;
}

void setTaskSimPrrUsed(int ID, enum PRRID value )
{

	TasksTable[ID].Sim.PRRUsed=value;

}

int getTaskSimPrrUsed(int taskID)
{

	return TasksTable[taskID].Sim.PRRUsed;

}



void Init_TasksTypes(int numTaskTypes,int numPRRs)
{
	int i,k;
	for (i =0;i<numTaskTypes;i++)
	{
		TasksTypes[i].ID=i;
		TasksTypes[i].name=0;
		TasksTypes[i].SWET=0;
		TasksTypes[i].HWET=0;
		TasksTypes[i].SWPriority=numPRRs/2 ;
//		for (k=0;k<numPRRs;k++)
//		{
//			TasksTypes[i].ConfigTime[k]=0;
//		}
		TasksTypes[i].CanRun=0XFF;
	//	TasksTypes[i].Module=1;

	}
}

/*
 * TaskType set/ get functions
 * TODO check the ID range and throw an error
 */

inline unsigned int getTaskTypeHWET( int ID)
{
	if(ID <=0 || ID>= MAX_TASKS_TYPES)
	{
		fprintf(stderr,"ERROR [TasksTypes] Index out of range 1\n");
		exit(EXIT_FAILURE);
	}
	return TasksTypes[ID].HWET;
}

inline void setTaskTypeHWET (int ID,unsigned int  value)
{
	if(ID <=0 || ID>= MAX_TASKS_TYPES)
	{
		fprintf(stderr,"ERROR [TasksTypes] Index out of range 2\n");
		exit(EXIT_FAILURE);
	}
	TasksTypes[ID].HWET=value;
}

//inline unsigned int getTaskTypeConfigTime( int ID, int pRRID)
//{
//	if(ID <=0 || ID>= MAX_TASKS_TYPES)
//	{
//		fprintf(stderr,"ERROR [TasksTypes] Index out of range 3\n");
//		exit(EXIT_FAILURE);
//	}
//	return TasksTypes[ID].ConfigTime[pRRID];
//}
//
//inline void setTaskTypeConfigTime (int ID,int pRRID, unsigned int value)
//{
//	if(ID <=0 || ID>= MAX_TASKS_TYPES)
//	{
//		fprintf(stderr,"ERROR [TasksTypes] Index out of range 4\n");
//		exit(EXIT_FAILURE);
//	}
//	TasksTypes[ID].ConfigTime[pRRID]=value;
//}

inline unsigned int getTaskTypeSWET( int ID)
{
	if(ID <=0 || ID>= MAX_TASKS_TYPES)
	{
		fprintf(stderr,"ERROR [TasksTypes] Index out of range 3\n");
		exit(EXIT_FAILURE);
	}
	return TasksTypes[ID].SWET;
}

inline void setTaskTypeSWET (int ID,unsigned int value)
{
	if(ID <=0 || ID>= MAX_TASKS_TYPES)
	{
		fprintf(stderr,"ERROR [TasksTypes] Index out of range 4\n");
		exit(EXIT_FAILURE);
	}
	TasksTypes[ID].SWET=value;
}


inline  int getTaskTypeSWPrio( int ID)
{
	if(ID <=0 || ID>= MAX_TASKS_TYPES)
	{
		fprintf(stderr,"ERROR [TasksTypes] Index out of range 5\n");
		exit(EXIT_FAILURE);
	}
	return TasksTypes[ID].SWPriority;
}

inline void setTaskTypeSWPrio (int ID,int value)
{
	if(ID <=0 || ID>= MAX_TASKS_TYPES)
	{
		fprintf(stderr,"ERROR [TasksTypes] Index out of range 6\n");
		exit(EXIT_FAILURE);
	}
	TasksTypes[ID].SWPriority=value;
}

inline unsigned  int getTaskTypeCanRun( int ID)
{
	if(ID <=0 || ID>= MAX_TASKS_TYPES)
	{
		fprintf(stderr,"ERROR [TasksTypes] Index out of range 7\n");
		exit(EXIT_FAILURE);
	}
	return TasksTypes[ID].CanRun;
}

inline void setTaskTypeCanRun (int ID,unsigned int value)
{
	if(ID <=0 || ID>= MAX_TASKS_TYPES)
	{
		fprintf(stderr,"ERROR [TasksTypes] Index out of range [%d] 8\n",ID);
		exit(EXIT_FAILURE);
	}
	TasksTypes[ID].CanRun=value;
}





/*
 * Node task functions
 */


struct node * CreateDFG(int size)
{
	struct node *dfg;
	dfg=malloc(sizeof (struct node) * size);
	return dfg;
}

void CleanDFG(struct node * dfg)
{
	free(dfg);
}

void CopyDFG(struct node *dst, struct node *src,int size)
{

	int i;
	for(i=0;i<size;i++)
	{
		dst[i]=src[i];
	}
}

unsigned int IsNodeOp1Address(struct node * dfg , int id){
	return dfg[id].D.isAdd_op1;
}

unsigned int IsNodeOp2Address(struct node * dfg , int id){
	return dfg[id].D.isAdd_op2;
}

void SetNodeOp1Address(struct node * dfg , int id,int isaddress){
	 dfg[id].D.isAdd_op1=isaddress;
}

 void SetNodeOp2Address(struct node * dfg , int id,int isaddress){
	 dfg[id].D.isAdd_op2=isaddress;
}


int GetNodeOp1Value(struct node * dfg , int id){
	return dfg[id].D.op1;
}

int GetNodeOp2Value(struct node * dfg , int id){
	return dfg[id].D.op2;
}

void SetNodeOp1Value(struct node * dfg , int id,int value){
	dfg[id].D.op1=value;
}

void SetNodeOp2Value(struct node * dfg , int id,int value){
	 dfg[id].D.op2=value;
}
unsigned int GetNodeEmulationHWdelay(struct node * dfg , int id){


	if (dfg[id].TypeID <0 || dfg[id].TypeID>=MAX_TASKS_TYPES)
	{
		fprintf(stderr,"ERROR [GetNodeEmulationHWdelay] Index out of range [%d] \n",dfg[id].TypeID);
				exit(EXIT_FAILURE);

	}
//	return getTaskTypeDataHWDelay(dfg[id].TypeID);
	return dfg[id].Emu.HWdelay;
}
unsigned int GetNodeEmulationSWdelay(struct node * dfg , int id){
	if (dfg[id].TypeID<0 || dfg[id].TypeID>=MAX_TASKS_TYPES)
	{
		fprintf(stderr,"ERROR [GetNodeEmulationSWdelay] Index out of range [%d] \n",dfg[id].TypeID);
				exit(EXIT_FAILURE);

	}

//	return  getTaskTypeDataSWDelay(dfg[id].TypeID);
	return dfg[id].Emu.SWdelay;
}

void SetNodeEmulationHWdelay(struct node * dfg , int id, unsigned int newValue){
	if(newValue <0 )
		{
			fprintf(stderr,"ERROR [SetNodeEmulationHWdelay] NEgative number \n");
			exit(EXIT_FAILURE);
		}
	dfg[id].Emu.HWdelay=newValue;
}
void SetNodeEmulationSWdelay(struct node * dfg , int id,unsigned int newValue){
	if(newValue <0 )
		{
		fprintf(stderr,"ERROR [SetNodeEmulationSWdelay] NEgative number \n");
			exit(EXIT_FAILURE);
		}
	dfg[id].Emu.SWdelay=newValue;
}

//enum PRRID  GetNodeSimPrrUSed(struct node * dfg , int id){
//	return dfg[id].;
//}
//unsigned char IsNodeSimReusd(struct node * dfg , int id){
//	return dfg[id].D.op1;
//}
//struct TimerTime GetNodeSimConfigTime(struct node * dfg , int id){
//	return dfg[id].D.op1;
//}
//struct TimerTime GetNodeSimExecTime(struct node * dfg , int id){
//	return dfg[id].D.op1;
//}


unsigned int  GetNodeID(struct node * dfg , int id){
	return dfg[id].id;
}

enum Mode  GetNodeMode(struct node * dfg , int id){
	return dfg[id].mode;
}

unsigned int  GetNodeNextNode(struct node * dfg , int id){
	return dfg[id].next;
}

unsigned int  GetNodeInitPrio(struct node * dfg , int id){
	return dfg[id].initPrio;
}

unsigned int  GetNodeCanRun(struct node * dfg , int id){
	return dfg[id].CanRun;
}



int  GetNodeTaskType(struct node *dFG, int taskID)
{

	return dFG[taskID].TypeID;

}
void SetNodeTaskType(struct node *dFG, int taskID, int NewTypeID)
{
	if(NewTypeID <=0 || NewTypeID>= MAX_TASKS_TYPES)
	{
		fprintf(stderr,"ERROR [setNodeTaskType] Index out of range [%d] id -> %d \n", NewTypeID,taskID);
		exit(EXIT_FAILURE);
	}
	dFG[taskID].TypeID=NewTypeID;

}


void SetNodeID(struct node *dFG, int taskID, int NewID)
{
	if(0>NewID )
	{
		fprintf(stderr,"ERROR [SetNodeID] ID can't be negative \n");
		exit(EXIT_FAILURE);
	}
	dFG[taskID].id=NewID;

}
void SetNodeNext(struct node *dFG, int taskID, int NewID)
{
	if(0>NewID )
	{
		fprintf(stderr,"ERROR [SetNodeNext] ID can't be negative \n");
		exit(EXIT_FAILURE);
	}
	dFG[taskID].next=NewID;

}
void SetNodeMode(struct node *dFG, int taskID, enum Mode mode)
{
	if(0>mode  || NumMode<=mode)
	{
		fprintf(stderr,"ERROR [SetNodeMode] undefined Mode \n");
		exit(EXIT_FAILURE);
	}
	dFG[taskID].mode=mode;

}

void SetNodeCanRun(struct node *dFG, int taskID, unsigned int canrun)
{

	dFG[taskID].CanRun=canrun;

}

int  GetNodeArch(struct node *dFG, int taskID)
{

	return dFG[taskID].arch;

}
void SetNodeArch(struct node *dFG, int taskID, int arch)
{
	if(arch <0 )
	{
		fprintf(stderr,"ERROR [SetNodeArch] can't be negative  \n");
		exit(EXIT_FAILURE);
	}
	dFG[taskID].arch=arch;

}
int  GetNodePower(struct node *dFG, int taskID)
{

	return dFG[taskID].power;

}
void SetNodePower(struct node *dFG, int taskID, int power)
{
	if(power <0 )
	{
		fprintf(stderr,"ERROR [SetNodePower] can't be negative \n");
		exit(EXIT_FAILURE);
	}
	dFG[taskID].power=power;
}




