/*
 * argparse.c
 *
 *  Created on: Jun 11, 2013
 *      Author: aalwatta
 */

#ifndef ARGPARSE_C_
#define ARGPARSE_C_


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <string.h>
#include "argparse.h"
#include "PlatformConfig.h"


void displayVersion()
{
	fprintf(stdout,"Scheduler Simulator Version [%s] \n ",VERSION_NUMBER);
	exit(EXIT_SUCCESS);
}

char displaymsg []={" usage  %s  [options] prog-and-args\n\n"
		"tool-selection option, with default in [ ]:\n\n"
		"-h --help \t\t show this message\n"
		"-V --version \t\t display version \n"
		"-v --verbose \t\t show node execution details  \n"
		"-g --task-graph \t\t Generate task graph file   \n"
		"-i --iteration \t\t number of iteration to run a DFG [1]  \n"
		"-t --task-migration \t\t enable task migration [0]  \n"
		"-q --disable-q-search \t\t disable searching the read queue for Reuse [0]  \n"
		"-k --scheduler \t\t pick scheduler (1-3) [3]  \n"
		"-d --dfg-file  \t\t followed by the DFG file to be processed [dfg.conf]\n"
		"-a --arch-file \t\t followed by architecture file to be processed [arch.conf]\n"
		"-p --prr-file  \t\t followed by PRR files (contains PRR combination) [prr.conf]\n"
		"-s --prrs-set \t\t integer to pick which PRR set to use from the PRR file [0]\n\n"};

void displayHelp(char *programName)
{
	fprintf(stdout, displaymsg,programName);
}
/*
	 * * FIXME  THIS FUNCTION NEED TO BE CHANGED COMPLETELY
	 * Does not check any error conditions,
	 * Does not check getting the right number of nodes,
	 * Need to be in this order..
	 * -n 10 -d 30 -p 33 -u 0 -s 233 -t 1 2 3 4 5
	 */

static struct option long_options[] = {
     {"dfg-file",   required_argument,   	 	  0,  'd' },
     {"arch-file",	required_argument,     		  0,  'a' },
     {"prr-file",  required_argument,			  0,  'p' },
     {"prrs-set",   required_argument, 			  0,  's' },
     {"scheduler",   required_argument, 		  0,  'k' },
     {"verbose",   no_argument,					  0,  'v' },
     {"task-graph",   no_argument,				  0,  'g' },
     {"iteration",   required_argument, 		  0,  'i' },
     {"task-migration",   no_argument, 			  0,  't' },
     {"disable-q-search",   no_argument, 			  0,  'q' },
     {"version",   no_argument, 				  0,  'V' },
     {"help",      no_argument, 				  0,  'h' },
     {0,           0,                 0,  0   }
 };

  int  parseArgs(int argc,  char *argv[],struct ArgData * argdatar){



	   int c;
	   opterr = 0;
	int long_index=0;
	   while ((c = getopt_long(argc, argv, "d:a:p:i:s:k:vVthgq",
			   	   long_options,&long_index)) != -1)
		   switch (c) {
		   case 'a':
			   strcpy( argdatar->fnArch,optarg);
			   break;
		   case 'd':
			  strcpy( argdatar->fnDFG,optarg);

			   break;

		   case 'p':
			   strcpy( argdatar->fnPRR,optarg);
			   break;
		   case 's':
			   argdatar->PRRsSet = atoi(optarg);
			   break;
		   case 'i':
		  		argdatar->iteration = atoi(optarg);

		  	 break;
		   case 'k':
		 		 argdatar->scheduler = atoi(optarg);
			if (argdatar->scheduler < 1 || argdatar->scheduler > 3) {
				fprintf(stderr,"Error scheduler value should be 1 - 3 \n");
				exit(EXIT_FAILURE);
			}
		 			   break;
		   case 'v':
			   argdatar->printDFG=1;
			   break;
		   case 'g':
			   argdatar->taskGraph=1;
			   break;
		   case 't':
			   argdatar->taskMigration=1;
			   break;
		   case 'q':
			   argdatar->noQSearch=1;
			   break;
		   case 'V':
			   displayVersion();
			   break;
		   case 'h':
			   displayHelp(argv[0]);
			   exit(EXIT_SUCCESS);
			   break;
		   case '?':
			   if (optopt == 'a' || optopt == 'd' || optopt == 'p'
					   || optopt == 's' )
				   fprintf(stderr, "Option -%c requires an argument.\n", optopt);
			   else if (isprint (optopt))
				   fprintf(stderr, "Unknown option `-%c'.\n", optopt);
			   else
				   fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
			   displayHelp(argv[0]);
			   return 1;
		   default:
			   abort();
		   }



	   return 0;
   }

  void InitArgdatar(struct ArgData *arg)
  {
  	arg->PRRsSet=0;
  	strcpy(arg->fnArch,ARCH_FILE_NAME);
  	strcpy(arg->fnDFG,DFG_FILE_NAME);
  	strcpy(arg->fnPRR,PRR_FILE_NAME);
  	arg->printDFG=0;
  	arg->scheduler=3 ;
  	arg->iteration=1;
  	arg->taskMigration=0;
  	arg->taskGraph=0;
  	arg->noQSearch=0;

  }

#endif /* ARGPARSE_C_ */
