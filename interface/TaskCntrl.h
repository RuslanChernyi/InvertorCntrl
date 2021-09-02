/*
 * TaskCntrl.h
 *
 *  Created on: 16 трав. 2014
 *      Author: Tester
 */

#ifndef ___TASK_CNTRL_H___
#define ___TASK_CNTRL_H___

#define _run_motor 1
#define _stop_motor 0

float getMainTask(char mode);
void cntrlStartStopDir(void);

float getTaskAct(void);
void initTaskMain(void);

#endif /* TASKCNTRL_H_ */
