/*
 * PlatfromConfig.h
 *
 *  Created on: 2013-05-24
 *      Author: aalwatta
 */

#ifndef PLATFROMCONFIG_H_
#define PLATFROMCONFIG_H_


#define VERSION_NUMBER "0.3.0"

#define GRAPH_FILE "./graph"

#define NO_OF_DFG_REP 1
#define INDEPENDENT_DFGS 1

#define RCS_SCHED_I 0
#define RCS_SCHED_III 1
#define RCS_SCHED_II 0


#define BUFFER_SIZE 255
#define MAX_TASKS_TYPES (24+1)
#define MAX_NO_OF_NODES 200
#define YES 1
#define NO 0

#define MAX_QUEUE_TASKS 250

#define SW_HW_MIG 1 // should be one all the time (to control) task migration use command line argument

#define EXEC_TIME_LEARNING_RATIO 0.2


#define DEBUG_PRINT 0

#define SCALING 50



/*
 * TASK Operation execution time.
 */


#define TASK_1_HW_DELAY 20U  //  Same HW/SW
#define TASK_1_SW_DELAY (TASK_1_HW_DELAY )
#define TASK_2_HW_DELAY 20U  //   HW/0.75SW
#define TASK_2_SW_DELAY (TASK_2_HW_DELAY /2)
#define TASK_3_HW_DELAY 80U  //  HW/1.5 SW
#define TASK_3_SW_DELAY (TASK_3_HW_DELAY *1.2 )
#define TASK_4_HW_DELAY 20U  //  HW /2.5 SW
#define TASK_4_SW_DELAY (TASK_4_HW_DELAY *3 )
#define TASK_5_HW_DELAY 15U
#define TASK_5_SW_DELAY (TASK_5_HW_DELAY*2 )
#define TASK_6_HW_DELAY 10U
#define TASK_6_SW_DELAY (TASK_6_HW_DELAY *4)
#define TASK_7_HW_DELAY 30U
#define TASK_7_SW_DELAY (TASK_7_HW_DELAY )
#define TASK_8_HW_DELAY 15U
#define TASK_8_SW_DELAY (TASK_8_HW_DELAY )
#define TASK_9_HW_DELAY 20U
#define TASK_9_SW_DELAY (TASK_9_HW_DELAY )
#define TASK_10_HW_DELAY 20U
#define TASK_10_SW_DELAY (TASK_10_HW_DELAY )

#define TASK_11_HW_DELAY 20U
#define TASK_11_SW_DELAY (TASK_10_HW_DELAY )
#define TASK_12_HW_DELAY 20U
#define TASK_12_SW_DELAY (TASK_10_HW_DELAY )
#define TASK_13_HW_DELAY 20U
#define TASK_13_SW_DELAY (TASK_10_HW_DELAY )
#define TASK_14_HW_DELAY 20U
#define TASK_14_SW_DELAY (TASK_10_HW_DELAY )
#define TASK_15_HW_DELAY 20U
#define TASK_15_SW_DELAY (TASK_10_HW_DELAY )
#define TASK_16_HW_DELAY 20U
#define TASK_16_SW_DELAY (TASK_10_HW_DELAY )
#define TASK_17_HW_DELAY 20U
#define TASK_17_SW_DELAY (TASK_10_HW_DELAY )
#define TASK_18_HW_DELAY 20U
#define TASK_18_SW_DELAY (TASK_10_HW_DELAY )
#define TASK_19_HW_DELAY 20U
#define TASK_19_SW_DELAY (TASK_10_HW_DELAY )

#define TASK_20_HW_DELAY 20U
#define TASK_20_SW_DELAY (TASK_10_HW_DELAY )
#define TASK_21_HW_DELAY 20U
#define TASK_21_SW_DELAY (TASK_10_HW_DELAY )
#define TASK_22_HW_DELAY 20U
#define TASK_22_SW_DELAY (TASK_10_HW_DELAY )
#define TASK_23_HW_DELAY 20U
#define TASK_23_SW_DELAY (TASK_10_HW_DELAY )
#define TASK_24_HW_DELAY 20U
#define TASK_24_SW_DELAY (TASK_10_HW_DELAY )
#define TASK_25_HW_DELAY 20U
#define TASK_25_SW_DELAY (TASK_10_HW_DELAY )
/*
 *FIXME PRR Configuration time for each PRR
 * DO NOT FORGET TO ADD THOSE to the init of the ConfigTime[] Array in schedulers.c
 * if you increase the number of PRR add them to the ConfiTime[] Array.
 *
 */
#define PRR_0_CONFIG_TIME 20
#define PRR_1_CONFIG_TIME 20
#define PRR_2_CONFIG_TIME 20
#define PRR_3_CONFIG_TIME 20
#define PRR_4_CONFIG_TIME 20

/*
#define PRR_5_CONFIG_TIME 20
#define PRR_6_CONFIG_TIME 20
#define PRR_7_CONFIG_TIME 20
#define PRR_8_CONFIG_TIME 20
*/

#define ARCH_1 (3)
#define ARCH_2 (4 + ARCH_1)
#define ARCH_3 (5 + ARCH_2)
#define ARCH_4 (2 + ARCH_3)
#define ARCH_5 (3 + ARCH_4)
#define ARCH_6 (3 + ARCH_5)
#define ARCH_7 (2 + ARCH_6)
#define ARCH_8 (2 + ARCH_7)

#endif /* PLATFROMCONFIG_H_ */
