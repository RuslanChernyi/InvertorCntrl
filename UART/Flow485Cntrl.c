/*
 * Flow485Cntrl.c
 *
 *  Created on: 6 ���. 2015
 *      Author: Tester
 */

#include "stddef.h"
#include "added.h"
#include "cntrlUART.h"

void(*Flow485Cntrl)(void);

void initFlow485Cntrl(void){
	Flow485Cntrl = NULL;
}

void cntrFlow485(void){
	if(Flow485Cntrl != NULL)
		(*Flow485Cntrl)();
}

void chengFlow485cntrl(void(*cntrl)(void)){
	Flow485Cntrl = cntrl;
}
