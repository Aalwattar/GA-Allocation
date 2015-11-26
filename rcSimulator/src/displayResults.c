/*
 * displayResults.c
 *
 *  Created on: Jun 3, 2013
 *      Author: aalwatta
 */
#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#include"PlatformConfig.h"




void print_DFG(struct node * dFG, int noPRRs)
{

        int i=0;
        struct Simulation Sim;
        char tmpstr[255];
        unsigned int  totalConfT=0;

        do {
				getTaskSimulation(i,&Sim);
				sprintf(tmpstr,"%u",Sim.ConfigTime.start);
                fprintf (stdout,"Node [%3d] -->T[%5u] R[%5s]  %s Config %4d Exec %4d %s[%d] Reuse[%3s] Prio[%d] Type[%d] \n",
								GetNodeID(dFG,i),

								Sim.ExecTime.start,Sim.PRRUsed<noPRRs?tmpstr:"-",
								Sim.PRRUsed<noPRRs?"RECONF": "SW COM",
								Sim.ConfigTime.end-Sim.ConfigTime.start,
								Sim.ExecTime.end-Sim.ExecTime.start,
								Sim.PRRUsed<noPRRs?"PRR":"GPP",
								Sim.PRRUsed<noPRRs?Sim.PRRUsed:Sim.PRRUsed-noPRRs,
                        		Sim.Reused?"YES":"NO",
                        		getTaskTypeSWPrio(GetNodeTaskType(dFG,i)),
                        		GetNodeTaskType(dFG,i)	);

                totalConfT+=Sim.ConfigTime.end-Sim.ConfigTime.start;

        } while (GetNodeNextNode(dFG,i++));

              printf(" Total Conf Time [%4u] , \n",	totalConfT);

}




