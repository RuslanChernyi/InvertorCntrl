/*
 * rs232hub.c
 *
 *  Created on: 03 апр. 2017 г.
 *      Author: test
 */


#include <stddef.h>
#include "added.h"
#include "ModBus/modbus.h"
#include "UART/cntrlUartTransfer.h"
#include "UART/Flow232Cntrl.h"
#include "ErrorTrack/ErrorTrack.h"

#define _line232max 8

void cntrlModbuAndShellRs232(void){
	UINT16 cnt, tmp;
	char rc[_line232max];
	if(testRcBufCnt()){
		cnt = 0;
		while(testRcBufCnt() && (cnt < _line232max)){
			getCharUart(&rc[cnt]);
			cnt++;
		}
		for(tmp=0; tmp<cnt; tmp++){
			UartFlowControl(&rc[tmp]);
		}
		for(tmp=0; tmp<cnt; tmp++){
			rcvErrReq(&rc[tmp]);
		}
	}else{
		UartFlowControl(NULL);
		rcvErrReq(NULL);
		testSCI1_Sts();//testSCI2_Sts();
	}
}

void initModbusAndShell232(void){
	InitFlow();
	chengFlow232cntrl(&cntrlModbuAndShellRs232);
}

