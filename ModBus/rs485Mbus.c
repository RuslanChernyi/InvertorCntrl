/*
 * rs485Mbus.c
 *
 *  Created on: 6 ���. 2015
 *      Author: Tester
 */

#include "added.h"
#include "modbus.h"
#include "UART/cntrlUART.h"
#include "UART/cntrlUartTransfer.h"
#include "UART/flow485cntrl.h"
#include "Rs485MbusAscii.h"

#define _lenth_msg 128

UINT8 mbusMsg[_lenth_msg];
UINT16 mbusMsgPntr;
UINT16 *trstPntrMbus;
char (*testMbusMsg)(char *msg, UINT16 number);
char (*answMbusMsg)(char *msg, UINT16 *number, UINT16 maxNum);
UINT8 adress485Mbus;

char testAdress485(char *buf){
	if(buf[0] == adress485Mbus)
		return 1;
	else
		return 0;
}

void initModbusType(void){
	testMbusMsg = testASCIImsg;
	answMbusMsg = cntrlRs485ASCII;
}



void initMbusMsg(void){
	UINT16 i;
	for(i=0; i<_lenth_msg; i++)
		mbusMsg[i] = 0;
	mbusMsgPntr = 0;

	adress485Mbus = getRegister(_ADRESS_RS485);
}

void trstMsgMbus(void){
	if(mbusMsgPntr){
		if(putCharUart485(*trstPntrMbus)){
			trstPntrMbus++;
			mbusMsgPntr--;
		}
	}else{
		initModbusAndShell();
	}
}

void cntrlMbusMsg(UINT8 msgByte){
	char tmp;
	if(mbusMsgPntr < _lenth_msg){
		mbusMsg[mbusMsgPntr] = msgByte;
		mbusMsgPntr++;
	}

	tmp = (*testMbusMsg)((char*)mbusMsg, mbusMsgPntr);
	if(tmp > 0){
		tmp = (*answMbusMsg)((char*)mbusMsg, &mbusMsgPntr,_lenth_msg);
		if(tmp){
			trstPntrMbus = mbusMsg;
			chengFlow485cntrl(trstMsgMbus);
		}
		else{
			initMbusMsg();
		}
	}else{
		if(tmp < 0)
			mbusMsgPntr = 0;
	}
}

