/*
 * main.c
 *
 *  Created on: Jun 24, 2013
 *      Author: aalwatta
 */

#include <stdio.h>
#include <stdlib.h>
#include <confuse.h>
#include <time.h>
//#include <sys/time.h>
//#include <sys/resource.h>
#include <unistd.h>
#include  "rcsSimulator.h"
#include  "common_interfaces.h"
#include "argparse.h"

void zeroSimData(struct SimData *sim, int size)
{
	int i;
	for (i = 0; i < size; ++i) {
		sim->typeData[i]=0;

	}
}
int main(int argc,  char* argv[] )
{
 /* start to measure the time in Unix */ 
 //////////////////////////////////////////
int type[13]={5,2,4,3,5,4,3,3,3,5,5,3,2};
	int t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13=0;
	long int count=0;
/////////////////////
 
//struct rusage usage; 
//struct timeval start, end; 
double start, end;
struct timespec ts; 
const clockid_t id = CLOCK_MONOTONIC_RAW;
//double cpu_time_used;
/* end time usage declaration */
    Common_Interface input_data;
    Hardware hardware;
    struct ArgData argdata;
    InitArgdatar(&argdata);
	if (parseArgs(argc, argv, &argdata)) {
		fprintf(stderr, "Exiting..\n\n");
		exit(EXIT_FAILURE);
	}
    
   if (  initArchLibrary(argdata.fnArch, &(input_data.archlib)))
   { fprintf(stderr,"ERROR reading file .. exiting \n");
   	  exit (EXIT_FAILURE);
   }
   if ( initDFG(argdata.fnDFG, &(input_data.dfg)))
   { fprintf(stderr,"ERROR reading file .. exiting \n");
      	  exit (EXIT_FAILURE);
      }
    if (initHardwareLibrary(argdata.fnPRR, &hardware))
    { fprintf(stderr,"ERROR reading file .. exiting \n");
       	  exit (EXIT_FAILURE);
       }
    
    input_data.setup = hardware.setups[argdata.PRRsSet];
    

	struct SimData simData={.typeData={0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0}};
	zeroSimData(&simData,500);
	struct SimResults simResults;

	simData.noOfNodes=input_data.dfg.num_nodes;
	INIT_FLAGS(simData.flags);
    if (argdata.printDFG)
    {
    	SET_FLAG_VALUE(simData.flags,PRINT_DFG_DATA);
    }

    if (argdata.taskMigration)
    {
    	SET_FLAG_VALUE(simData.flags,TASK_MIGRATION);
    }

    if (argdata.noQSearch)

    { printf("no q search\n");
    	SET_FLAG_VALUE(simData.flags,NO_SEARCH_Q);
    }
    if (argdata.taskGraph)
    {
    	SET_FLAG_VALUE(simData.flags,GENERATE_TASK_GRAPH);
    }

   simData.iteration= argdata.iteration;

    if(1==argdata.scheduler)
    	SET_FLAG_VALUE(simData.flags,RCSSCHED_I);
    else if (2==argdata.scheduler)
    	SET_FLAG_VALUE(simData.flags,RCSSCHED_II);
    else
    	SET_FLAG_VALUE(simData.flags,RCSSCHED_III);
		
	if (input_data.dfg.num_nodes!=13) 
		{
			printf("ERROR: This code has been hard coded to brute force \n"
			"All the possible solution for S4 HAL , 13 nodes \n "
			"The code has been modified in a dirty way to submit the paper ontime\n "
			"there is no point of trying larger benchmraks, it will take forever\n" 
			"if that what are you trying to do, just go home and dream about something else. \n \n"); 
			
			exit(2); 
		}
	InitSimulator(&input_data);
	//getrusage (RUSAGE_SELF,&usage);
	//start =usage.ru_utime; 
	fprintf(stdout, "total time, power, # config, # reuse, Busy,# SW Task, #HW2SW Mig, chromosome\n");
	if ( id != (clockid_t)-1 && clock_gettime( id, &ts ) != -1 )
	 start= (double)ts.tv_sec + (double)ts.tv_nsec / 1000000000.0;
	else 
		printf("error reading the time \n"); 
		
	for (t1=0;t1<type[0];t1++)
	for (t2=0;t2<type[1];t2++)
	for (t3=0;t3<type[2];t3++)
	for (t4=0;t4<type[3];t4++)
	for (t5=0;t5<type[4];t5++)
	for (t6=0;t6<type[5];t6++)
	for (t7=0;t7<type[6];t7++)
	for (t8=0;t8<type[7];t8++)
	for (t9=0;t9<type[8];t9++)
	for (t10=0;t10<type[9];t10++)
	for (t11=0;t11<type[10];t11++)
	for (t12=0;t12<type[11];t12++)
	for (t13=0;t13<type[12];t13++)
	{
	 count++;
	 simData.typeData[0]=t1;
	 simData.typeData[1]=t2;
	 simData.typeData[2]=t3;
	 simData.typeData[3]=t4;
	 simData.typeData[4]=t5;
	 simData.typeData[5]=t6;
	 simData.typeData[6]=t7;
	 simData.typeData[7]=t8;
	 simData.typeData[8]=t9;
	 simData.typeData[9]=t10;
	 simData.typeData[10]=t11;
	 simData.typeData[11]=t12;
	 simData.typeData[12]=t13;
	 
	 
	 
	RunSimulator(&simData,&simResults);
	
	fprintf(stdout, "%d, %d, %u, %u, %u, %u, %u, %d%d%d%d%d%d%d%d%d%d%d%d%d\n",
	  simResults.totalTime,simResults.power,simResults.noOfConfiguration,simResults.noOfReuse,
	  simResults.noHWBusyCounter,simResults.noOfSWTasks,simResults.noHW2SWMigration,
		t1+1,t2+1,t3+1,t4+1,t5+1,t6+1,t7+1,t8+1,t9+1,t10+1,t11+1,t12+1,t13+1); 
	}
		if ( id != (clockid_t)-1 && clock_gettime( id, &ts ) != -1 )
	 end= (double)ts.tv_sec + (double)ts.tv_nsec / 1000000000.0;
	else 
		printf("error reading the time \n"); 



	/*fprintf(stdout, "Process complete in {%d} cycles \n", simResults.totalTime);
	fprintf(stdout,
			"Number of configuration= %u SW Busy [%u] HW Busy [%u]\n",
			simResults.noOfConfiguration, simResults.noSWBusyCounter,
			simResults.noHWBusyCounter);
	fprintf(stdout,
			"SW2HW MIG [%u]  HW2SW Mig [%u] #of Reuse [%u]  #SW tasks [%u]\n",
			simResults.noSW2HWMigration, simResults.noHW2SWMigration, simResults.noOfReuse,
			simResults.noOfSWTasks);
	fprintf(stdout, "Total Power is {%d}  \n", simResults.power);
	fprintf(stdout,"_________________________________________________________________\n");
	*/
	
	
	
	
//	RunSimulator(&simData,&simResults);
//
//	fprintf(stdout, "Process complete in {%d} cycles \n", simResults.totalTime);
//	fprintf(stdout,
//			"Number of configuration= %u SW Busy [%u] HW Busy [%u]\n",
//			simResults.noOfConfiguration, simResults.noSWBusyCounter,
//			simResults.noHWBusyCounter);
//	fprintf(stdout,
//			"SW2HW MIG [%u]  HW2SW Mig [%u] #of Reuse [%u]  #SW tasks [%u]\n",
//			simResults.noSW2HWMigration, simResults.noHW2SWMigration, simResults.noOfReuse,
//			simResults.noOfSWTasks);
//	fprintf(stdout, "Total Power is {%d}  \n", simResults.power);
//

//printf("Started at: %lld.%llds\n", start.tv_sec, start.tv_usec);
//  printf("Ended at: %lld.%llds\n", end.tv_sec, end.tv_usec);
 printf("Time used %f\n", (double) (end-start));
 printf (" number of combinatins %ld\n", count); 
	CleanSimulator();
    freeArchLibrary(&(input_data.archlib));
    freeDFG(&(input_data.dfg));
    freeHardwareLibrary(&hardware);

	return (EXIT_SUCCESS);
}
