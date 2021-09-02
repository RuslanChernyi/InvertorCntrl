/*
 * Rs485hub.c
 *
 * ������� ��������� ����� �� ���������� modbus & shell
 *
 *  Created on: 5 ����. 2014
 *      Author: Tester
 */

#include "added.h"
#include "interface/shell.h"
#include "ModBus/modbus.h"
#include "UART/cntrlUartTransfer.h"
#include "flow485cntrl.h"
#include "ModBus/rs485Mbus.h"

#define UART485RxBufDataSize()  testRcBuf485Cnt()
#define UART485GetChar(var) getCharUart485(var)

void cntrlModbuAndShellRecive(void){
	UINT16 tmp;
	if(UART485RxBufDataSize())
	{
		UART485GetChar((char*)&tmp);

		cntrlMbusMsg(tmp);

		cntrlShellMsg(tmp);
	}else{
		testSCI3_Sts();//testSCI2_Sts();
	}
}

void initModbusAndShell(void){
	initMbusMsg();
	chengFlow485cntrl(cntrlModbuAndShellRecive);
	initModbusType();
}
