/*
 * io.h
 *
 *  Created on: Jul 10, 2013
 *      Author: aalwatta
 */

#ifndef IO_H_
#define IO_H_
#include "common_interfaces.h"

int convertDFG(struct node *dFG, DFG *rawDFG);
unsigned int generateCanRunMask( unsigned int prr);
unsigned int * init_pRRrConfigValues(unsigned int *srcarr ,int size);
void initPRRsConfigTime(unsigned int * prrTime, int noPRRs);
void initTaskTypeData(Common_Interface* );
void updateCanRun( struct node *dFG, Architecture_Library *,int numNodes,int numPRRs);
void sortDecend (unsigned int *arr, int size );
int cmpfunc (const void * a, const void * b);
void initTaskModeandSWtime(Common_Interface* mn, struct node *dFG) ;
#endif /* IO_H_ */
