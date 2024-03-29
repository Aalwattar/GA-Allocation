#ifndef PROCESSORS_H_
#define PROCESSORS_H_

#include "data.h"
#define YES 1
#define NO 0

enum ProcessorType {TypeHW, TypeSW};




struct Processor
{	unsigned long ConfigTime; // Used to calc softprio for rcsSChedIII
	unsigned long ExecCount;
	unsigned long ConfigCount;
	int Busy;
	int CurrentTaskID;
	int CurrentTaskType;
	int CurrentArch;
	unsigned int ID;
	enum ProcessorType Type;

};



struct NodeData
{
	int TaskType;
	int Arch;
	int TaskID;
	unsigned long ExecCount;
};


struct PE
{
	struct Processor * pe;
	int size;
};

struct PEs
{
	struct PE * HWPE;
	struct PE * SWPE;
};


unsigned int GetTimer(void);
void ResetTimer(void);
struct Processor * CreateProcessors( int , enum ProcessorType );
int IsProcessorBusy( struct Processor *);
void SetProcessorBusy( struct Processor *);
void SetProcessorNotBusy( struct Processor *);
int CheckCurrentTaskType( struct Processor *);
int TickProcessor( struct Processor *);
int LoadProcessor( struct Processor *, struct NodeData  );
int TickAllProcessors(struct Processor *, int , struct node *);
void FreeProcessors(struct Processor * processor);
unsigned int Ticker(struct PEs *, struct node *);
void CleanAllProcessor(struct PEs *pEs);
void CreateAllPEs(struct PEs *pEs,int noOfPRRs, int noOfGPPs);
void CleanAllPEs(struct PEs *pEs);
void InitProcessors(struct Processor* processors, int size,enum ProcessorType type);
#endif /* PROCESSORS_H_ */
