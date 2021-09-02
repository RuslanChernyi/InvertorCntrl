/*
 * cntrlErrShell.c
 *
 *  Created on: 19 апр. 2017 г.
 *      Author: test
 */

#include <string.h>
#include <stdlib.h>
#include "added.h"
#include "UART/Flow232Cntrl.h"
#include "UART/cntrlUartTransfer.h"
#include "ErrorTrack.h"
#include "AnswCmdDecode.h"
#include "SysTimer/sysTime.h"

#define _lenthReq 48

#define _time_hold 2000

char *answBuf, *answChr;
UINT16 answCnt;

char reqBuf[_lenthReq];
UINT16 reqCnt;
UINT32 oldTime;

void trsmErrAnsw(void){
	while(answCnt){
		if(putCharUart(*answChr)){
			answChr++;
			answCnt--;
		}else
			break;
	}
	if(!answCnt){
		initModbusAndShell232();
		if(answBuf!=NULL){
			free(answBuf);
			answBuf = NULL;
		}
	}
}

void freeReqErr(void){
	UINT16 i;
	for(i=0; i<_lenthReq; i++)
		reqBuf[i] = 0;

	reqCnt = 0;
	oldTime = getCountTimer();
}

void rcvErrReq(char* rcv){
	UINT32 time;
	if(rcv != NULL){
		if(reqCnt < _lenthReq){
			reqBuf[reqCnt] = *rcv;
			reqCnt++;
		}
		answCnt = cntrlTrackCmd(reqBuf, &answBuf);
		if(answCnt){
			answChr = answBuf;
			chengFlow232cntrl(&trsmErrAnsw);
			freeReqErr();
		}
		oldTime = getCountTimer();
	}else{
		time = getCountTimer();
		if((time-oldTime)>_time_hold){
			freeReqErr();
		}
	}
}
