#ifndef DATA_H_
#define DATA_H_

#include "PlatformConfig.h"






//enum Mode {SWOnly=0x0001,HWOnly=0x0002,HybSW=0x0003,HybHW=0x0013, CustomHW=0x0004,
//	CustomHWnSW=0x0005, NumMode
//};
enum Mode {SWOnly,HWOnly,HybSW,HybHW, CustomHW,
	CustomHWnSW, NumMode
};
enum Operations {OpAdd,OpSub,OPMult,OpShift,OpBlank, NumOperation};

enum PRModules {adder,sub,mult,shift,blank, NumPRModules};
enum PRRID {Math0,Math1,Math2,Math3,Math4,NumPRRID};


struct TimerTime
{
	unsigned  start;
	unsigned  end;
};

struct Simulation
{
	enum PRRID PRRUsed;
	unsigned char  Reused;
	struct TimerTime ConfigTime;
	struct TimerTime ExecTime;

};

/*
 * store time delay for software and hardware
 * TODO delete this
 */


struct PRRProcess {
	int No;
	enum PRRID PRR_ID[BUFFER_SIZE];
};

/* TODO separate this structure into separate structure one is read only and the other is read/write
 *  Create different files to update the internal properties, instead for the stupid global variable tech.
 *  each functions takes one value which is ID and store or return the appropriate value.
 *  I guess it's better to separate the structures instead of having one for everything.
 *
 *
 * */

struct Emulation
{
	unsigned int  HWdelay;
	unsigned int  SWdelay;
};
struct data {
	int op1;
	int op2;
	unsigned  isAdd_op1 : 1;
	unsigned  isAdd_op2: 1;
	unsigned :0 ;
};
struct node {
	 unsigned int  id;
	 unsigned char  operation;
	 enum Mode mode;
	 unsigned int next;
	 struct data D;
	 unsigned int  initPrio; // initial priority to start with
	 unsigned int  CanRun;
	 int TypeID;
	 int power;
	 int arch;
	 struct Emulation Emu;

};

struct nodeData
{
	enum Mode mode;
	unsigned int Done ;
	int result;
	unsigned int  Q;
	struct Simulation Sim; // move to a separate struct

};
struct TaskType{
	int ID;
	char * name;
	unsigned int  SWET;
	unsigned int  HWET;
	int SWPriority;
//	int ConfigTime[BUFFER_SIZE];
	unsigned int  CanRun;
	//enum PRModules Module;

};

struct tasksTableToken
{
	struct node *dfg;
	struct taskData *TaskTable;
	void * token;
	int ID;
	int size;
};
//struct DFG {
//	int size;
//	struct node dfg[MAX_NO_OF_NODES];
//};




// extern struct node node;
 //struct DFG DFG;



enum SystemStates {Start,CfgDone,TaskDone,None};

extern enum SystemStates State;
//extern struct DFG DFGArray[];
//extern struct TaskType TasksTypes[];

void Init_TasksTypes(int ,int );
void reinitTasksTable( int numberOfTasks);
void CleanTasksTable(void);
inline unsigned char  isTaskDone(int ID);
inline void taskDone(int ID);
inline unsigned char  isTaskQed(int ID);
inline void taskQed(int ID);
inline int getTaskResult( int ID);
inline void setTaskResult (int ID,int value);
void getTaskSimulation( int ID,struct Simulation *value);
void setTaskSimulation (int ID,struct Simulation *value);
inline enum Mode getTaskMode( int ID);
inline void setTaskMode (int ID,enum Mode value);
inline void setTaskCounter(int value);
inline void decTaskCounter(void);
inline int getTaskCounter(void);
void setTaskSimExecTimeStart(int ID, unsigned int value );
void setTaskSimExecTimeEnd(int ID, unsigned int value );
void setTaskSimConfTimeStart(int ID, unsigned int value );
void setTaskSimConfTimeEnd(int ID, unsigned int value );
void setTaskSimReused(int ID, unsigned char value );
void setTaskSimPrrUsed(int ID, enum PRRID value );
inline unsigned int getTaskTypeHWET( int ID);
inline void setTaskTypeHWET (int ID,unsigned int  value);
inline unsigned int getTaskTypeSWET( int ID);
inline void setTaskTypeSWET (int ID,unsigned int value);
inline  int getTaskTypeSWPrio( int ID);
inline void setTaskTypeSWPrio (int ID,int value);
//inline unsigned int getTaskTypeConfigTime( int ID, int pRRID);
//inline void setTaskTypeConfigTime (int ID,int pRRID, unsigned int value);
inline unsigned  int getTaskTypeCanRun( int ID);
inline void setTaskTypeCanRun (int ID,unsigned int value);
void SetNodeTaskType(struct node *dFG, int taskID, int NewTypeID);
int IsTaskSimReused(int  );
int getTaskSimPrrUsed(int );

struct node * CreateDFG(int size);
void CleanDFG(struct node * dfg);
void CopyDFG(struct node *dst, struct node *src,int size);
unsigned int IsNodeOp1Address(struct node * dfg , int id);
unsigned int IsNodeOp2Address(struct node * dfg , int id);
int GetNodeOp1Value(struct node * dfg , int id);
int GetNodeOp2Value(struct node * dfg , int id);
unsigned int GetNodeEmulationHWdelay(struct node * dfg , int id);
unsigned int GetNodeEmulationSWdelay(struct node * dfg , int id);
void SetNodeEmulationHWdelay(struct node * dfg , int id, unsigned int newValue);
void SetNodeEmulationSWdelay(struct node * dfg , int id,unsigned int newValue);
unsigned int  GetNodeID(struct node * dfg , int id);
enum Mode  GetNodeMode(struct node * dfg , int id);
unsigned int  GetNodeNextNode(struct node * dfg , int id);
unsigned int  GetNodeInitPrio(struct node * dfg , int id);
unsigned int  GetNodeCanRun(struct node * dfg , int id);
int  GetNodeTaskType(struct node *dFG, int taskID);
void SetNodeOp1Address(struct node * dfg , int id,int isaddress);
 void SetNodeOp2Address(struct node * dfg , int id,int isaddress);
 void SetNodeOp1Value(struct node * dfg , int id,int value);
 void SetNodeOp2Value(struct node * dfg , int id,int value);
 void SetNodeID(struct node *dFG, int taskID, int NewID);
 void SetNodeMode(struct node *dFG, int taskID, enum Mode mode);
 void SetNodeCanRun(struct node *dFG, int taskID, unsigned int canrun);
 void SetNodeNext(struct node *dFG, int taskID, int NewID);

 int  GetNodeArch(struct node *dFG, int taskID);
 void SetNodeArch(struct node *dFG, int taskID, int arch);
 int  GetNodePower(struct node *dFG, int taskID);
 void SetNodePower(struct node *dFG, int taskID, int power);

#endif /* DATA_H_ */
