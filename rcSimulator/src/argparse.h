/*
 * argparse.h
 *
 *  Created on: Jun 11, 2013
 *      Author: aalwatta
 */

#ifndef ARGPARSE_H_
#define ARGPARSE_H_


#define MAX_NUMBER_NODES 500
#define BUFFER_SIZE 255
#define PRR_FILE_NAME  "/export/home/shares/sharegroup1/conf/prr.conf"
#define ARCH_FILE_NAME "/export/home/shares/sharegroup1/conf/arch_exp1.conf"
#define DFG_FILE_NAME "/export/home/shares/sharegroup1/conf/dfgs/B2_dfgTemplateB2_25_40_2.conf"

struct ArgData {
	char fnDFG[BUFFER_SIZE];
	char fnPRR[BUFFER_SIZE];
	char fnArch[BUFFER_SIZE];
	int PRRsSet;
	int scheduler;
	int taskMigration;
	int iteration;
	int printDFG;
	int taskGraph;
	int noQSearch;

};

void InitArgdatar(struct ArgData *arg);
int  parseArgs(int argc,  char *argv[],struct ArgData * argdatar);
#endif /* ARGPARSE_H_ */
