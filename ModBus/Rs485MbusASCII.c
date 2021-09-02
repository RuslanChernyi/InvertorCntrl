/*
 * Rs485MbusASCII.c
 *
 *  Created on: 4 ����. 2014
 *      Author: Tester
 *
 *  �������� ModbusASCII
 *
 */

#include "modbus.h"
#include "SysTimer/sysTime.h"
#include "stdlib.h"
#include "ModBus.Def"
#include "CommandSlave.h"
#include "rs485Mbus.h"

#define _min_ascii_msg 17
#define _START_ASCII _START
#define _number_buf_data 128
#define _max_wait_time 1000

UINT32 timeRecive;

char testASCIImsg(char *msg, UINT16 number){
	UINT16 cnt;
//	UINT32 tmp;
	if(number == 1){
		if(msg[0] == _START){
			timeRecive = getCountTimer();
			return 0;
		}
		return -1;
	}

//	tmp = getCountTimer();
//	tmp = tmp - timeRecive;
	if((getCountTimer() - timeRecive) > _max_wait_time)
		return -1;

	if(number < _min_ascii_msg)
		return 0;
	cnt = 1;
	while(cnt < number){
		if(msg[cnt] == STOP_2){
			if(msg[cnt-1] == STOP_1)
				return 1;
		}
		cnt++;
	}
	return 0;
}

char cntrlRs485ASCII(char *msg, UINT16 *num, UINT16 maxNum){
	UINT16 number;
	UINT16 cnt;
	char buf[_number_buf_data];
	cnt = 0;
	number = *num;

	cnt = ConvertString(msg, number, buf);
	if(cnt > 0){
		if ( !LRC ( buf, cnt ) ){
			if(testAdress485(buf)){
				switch (getCmdCode(buf)){
					case 0x01 :     //������� ������ ��������� SLAVE-�
					{
						cnt = CmdGetCoils( buf, cnt );
						break;
					}
					case 0x02 :     //������� ������ ��������� SLAVE-�
					{
						cnt = CmdGetInputBits( buf, cnt );
						break;
					}
					case 0x03 :     //������� ������ ��������� SLAVE-�
					{
						cnt = CmdGetReadingReg( buf, cnt );
						break;
					}
					case 0x04 :     //������� ������ ��������� SLAVE-�
					{
						cnt = CmdGetInputReg( buf, cnt );
						break;
					}
					case 0x05 :     //������� ������ ��������� SLAVE-�
					{
						cnt = CmdCoilCheng( buf, cnt );
						break;
					}
					case 0x06 :     //������� ��������� 1-�� �������� SLAVE-�
					{
						cnt = CmdRegSet( buf, cnt );

						break;
					}
					case 0x010 :     //������� ��������� ���������� ��������� SLAVE-�
					{
						cnt = CmdRegsNeedSet( buf, cnt );
						break;
					}
					default : return _UnKnownCommand;
				}

			}else{
				cnt = 0;
			}
		}else{
			cnt = 0;
		}
	}

	if(cnt > 0){
		*num=ConvertMessag(buf, msg, cnt);
	}

	return cnt;
}
