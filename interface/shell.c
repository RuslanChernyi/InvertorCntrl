/*
 * shell.c
 *
 *  Created on: 8 ����. 2014
 *      Author: Tester
 *
 *  �������� ���������� ��������� ������
 *
 */

#include "added.h"
#include "string.h"
#include "stdlib.h"
#include "shell.h"
#include "UART/flow485cntrl.h"
#include "UART/cntrlUartTransfer.h"
#include "UART/cntrlUART.h"
#include "SysTimer/sysTime.h"

#define _num_shell_cmd 2
#define _min_shell_cmd_lenth 5
#define _lenth_shell_cmd 16

#define _wait_msg_time ((UINT32)45000)

//const char *CmdArray[_num_shell_cmd] = {
//		"--help",
//		"--track"
//};

UINT16 shellMsgPntr;
UINT32 timeWait;
char* shellMsg, *trstPntrShell;

void initShellCntrl(void){
	UINT16 i;
	shellMsg = malloc(sizeof(char)*_lenth_shell_cmd);
	shellMsgPntr = 0;
	for (i = 0; i < _lenth_shell_cmd; i++)
		shellMsg[i] = 0;
}

void trstMsgShell(void){
	if(shellMsgPntr){
		if(putCharUart485(*trstPntrShell)){
			trstPntrShell++;
			shellMsgPntr--;
		}
	}else{
		free(shellMsg);
		initShellCntrl();
		initModbusAndShell();
	}
}

void cntrlShellMsg(UINT8 shellByte){
	char tmp;
	if(shellMsgPntr < _lenth_shell_cmd){
		shellMsg[shellMsgPntr] = shellByte;
		shellMsgPntr++;
		timeWait = getCountTimer();
	}

	if((getCountTimer()-timeWait) > _wait_msg_time){
		for (shellMsgPntr = 0; shellMsgPntr < _lenth_shell_cmd; shellMsgPntr++)
			shellMsg[shellMsgPntr] = 0;
		shellMsgPntr = 0;
		timeWait = getCountTimer();
	}

	tmp = testShellMsg(&shellMsg, shellMsgPntr);
	if(tmp > 0){
		shellMsgPntr = tmp;
		trstPntrShell = shellMsg;
		chengFlow485cntrl(trstMsgShell);
	}
}

INT16 testShellMsg(char* *msg, UINT16 number){
	UINT16 i;
	char *str;
	if(number < _min_shell_cmd_lenth){
		return 0;
	}

	str = *msg;
	i = testCmdShell_Help(&str, number);
	if(i > 0){
		free(*msg);
		*msg = str;
		return i;
	}

	i = testCmdShell_Track(msg, number);
	if(i > 0){
		free(*msg);
		*msg = str;
		return i;
	}

	return 0;
}

