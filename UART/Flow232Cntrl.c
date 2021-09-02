/*
 * Flow232Cntrl.c
 *
 *  Created on: 31 марта 2017 г.
 *      Author: test
 */



#include "stddef.h"
#include "added.h"
#include "cntrlUART.h"

void(*Flow232Cntrl)(void);

void initFlow232Cntrl(void){
	Flow232Cntrl = NULL;
}

void cntrFlow232(void){
	if(Flow232Cntrl != NULL)
		(*Flow232Cntrl)();
}

void chengFlow232cntrl(void(*cntrl)(void)){
	Flow232Cntrl = cntrl;
}
