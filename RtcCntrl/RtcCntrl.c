/*
 * RtcCntrl.c
 *
 *  Created on: 31 ����. 2015
 *      Author: Tester
 *
 *  ������ ����� ��������� �������
 *
 */

#include "stddef.h"
#include "SysTimer/sysTime.h"
#include "RtcCntrl.h"
#include "cntrlI2C/I2Ccontrol.h"

#define ControlTimeID 0b011010000
#define _RAED_FULL_DATA -8

#define _YEAR_DS1307 6
#define _DATA_DS1307 4
#define _TIME_DS1307 0

#define _TIME_NUM_RD 3
#define _TIME_NUM_WR 4

#define _DATA_NUM_RD 2
#define _DATA_NUM_WR 3

#define _YEAR_NUM_RD 1
#define _YEAR_NUM_WR 2

#define _NO_REGUlAR_READ

#define _MAX_MSG_TM 9

stI2CTask RtcTask;
char RtcMsg[_MAX_MSG_TM];
stFullRtcData CurrentRtc;

#ifndef _NO_REGUlAR_READ

INIT_DELAY(_RTC_DL)

#endif

void initRtClock(void){
	char i;

	//������������� �����
	RtcTask.adress = ControlTimeID;
	RtcTask.data = RtcMsg;
	RtcTask.next = NULL;
#ifndef _NO_REGUlAR_READ
	SET_DELAY(_RTC_DL, c1sec);
#endif

	CurrentRtc.second = 0;
	CurrentRtc.time.minute = 0;
	CurrentRtc.time.hour = 0;
	CurrentRtc.week = 0;
	CurrentRtc.data.day = 0;
	CurrentRtc.data.month = 0;
	CurrentRtc.year = 0;

	RtcTask.type = COMPLITE_OPERATION;
	for(i=0; i<_MAX_MSG_TM; i++)
		RtcMsg[i] = 0;
	if(getRegSecund()&0x080)
		setRegSecund(0);
}

#ifndef _NO_REGUlAR_READ
void cntrlRtcData(void){
	//������ �����
	if(TEST_DELAY(_RTC_DL)){
		SET_DELAY(_RTC_DL, c1sec);
		if(RtcTask.type == COMPLITE_OPERATION){
			CurrentRtc.second = RtcMsg[1] & 0x07f;
			CurrentRtc.time.minute = RtcMsg[2];
			CurrentRtc.time.hour = RtcMsg[3];
			CurrentRtc.week = RtcMsg[4];
			CurrentRtc.data.day = RtcMsg[5];
			CurrentRtc.data.month = RtcMsg[6];
			CurrentRtc.year = RtcMsg[8];
			CurrentRtc.year <<= 8;
			CurrentRtc.year |= RtcMsg[7];

			RtcTask.data = RtcMsg;
			RtcMsg[0] = 0;
			RtcTask.type = READ_I2C;
			RtcTask.count = _RAED_FULL_DATA;
			if(!PutI2CTask(&RtcTask))
				RtcTask.count = COMPLITE_OPERATION;
		}
	}
}
#endif

stRtClock getCurrentTime(void){
	return CurrentRtc.time;
}

stRtcData getCurrentData(){
	return CurrentRtc.data;
}

UINT16 getRegTime(void){
	UINT16 time;
	char msg[4];

	while(RtcTask.type != COMPLITE_OPERATION){
		I2C_Control();
	}

	do{
		RtcTask.data = msg;
		msg[0] = _TIME_DS1307;
		RtcTask.type = READ_I2C;
		RtcTask.count = -_TIME_NUM_RD;
		while(!PutI2CTask(&RtcTask))
			I2C_Control();
		while(RtcTask.type != COMPLITE_OPERATION){
			I2C_Control();
		}
	}while(RtcTask.count != 0);
	if(RtcTask.count)
		return 0;

	time = (UINT16)msg[3];
	time <<= 8;
	time |= 0x0ff & (UINT16)msg[2];
	return time;
}

char setRegTime(UINT16 reg){
	stRtClock time;
	time.minute = reg &0x0ff;
	time.hour = reg >> 8;
	while(setCurrentTime(time));
	return 1;
}

char setCurrentTime(stRtClock time){
	char msg[4];
	while(RtcTask.type != COMPLITE_OPERATION){
		I2C_Control();
	}
	do{
		RtcTask.data = msg;
		RtcTask.count = _TIME_NUM_WR;
		msg[2] = time.minute;
		msg[3] = time.hour;
		msg[0] = _TIME_DS1307;
		msg[1] = 0;
		RtcTask.type = WRITE_I2C;

		while(!PutI2CTask(&RtcTask))
			I2C_Control();
		while(RtcTask.type != COMPLITE_OPERATION){
			I2C_Control();
		}
	}while(RtcTask.count != 0);

	if(RtcTask.count)
		return 1;

	return 0;
}

UINT16 getRegData(void){
	UINT16 data;
	char msg[4];
	while(RtcTask.type != COMPLITE_OPERATION){
		I2C_Control();
	}

	do{
		RtcTask.data = msg;
		msg[0] = _DATA_DS1307;
		RtcTask.type = READ_I2C;
		RtcTask.count = -_DATA_NUM_RD;
		while(!PutI2CTask(&RtcTask))
			I2C_Control();
		while(RtcTask.type != COMPLITE_OPERATION){
			I2C_Control();
		}
	}while(RtcTask.count != 0);
	if(RtcTask.count)
		return 0;

	data = msg[1];
	data <<= 8;
	data |= (0x0ff & (UINT16)msg[2]);
	return data;
}

char setRegData(UINT16 reg){
	stRtcData data;
	data.month = reg &0x0ff;
	data.day = reg >> 8;
	while(setCurrentData(data));
	return 1;
}

char setCurrentData(stRtcData data){
	char msg[3];
	while(RtcTask.type != COMPLITE_OPERATION){
		I2C_Control();
	}
	do{
		RtcTask.data = msg;
		RtcTask.count = _DATA_NUM_WR;
		msg[1] = data.day;
		msg[2] = data.month;
		msg[0] = _DATA_DS1307;
		RtcTask.type = WRITE_I2C;

		while(!PutI2CTask(&RtcTask))
			I2C_Control();
		while(RtcTask.type != COMPLITE_OPERATION){
			I2C_Control();
		}
	}while(RtcTask.count != 0);

	if(RtcTask.count)
		return 1;

	return 0;
}

UINT16 getRegYear(void){
	UINT16 data;
	char msg[4];
	while(RtcTask.type != COMPLITE_OPERATION){
		I2C_Control();
	}

	do{
		RtcTask.data = msg;
		msg[0] = _YEAR_DS1307;
		RtcTask.type = READ_I2C;
		RtcTask.count = -_YEAR_NUM_RD;
		while(!PutI2CTask(&RtcTask))
			I2C_Control();
		while(RtcTask.type != COMPLITE_OPERATION){
			I2C_Control();
		}
	}while(RtcTask.count != 0);
	if(RtcTask.count)
		return 0;

	data = (UINT16)msg[1];
//	data <<= 8;
//	data |= 0x02000;
	return data;
}

char setCurrentYear(UINT16 year){
	char msg[3];
	while(RtcTask.type != COMPLITE_OPERATION){
		I2C_Control();
	}
	do{
		RtcTask.data = msg;
		RtcTask.count = _YEAR_NUM_WR;
		msg[1] = year & 0x0ff;
	//	msg[2] = year >> 8;
		msg[0] = _YEAR_DS1307;
		RtcTask.type = WRITE_I2C;

		while(!PutI2CTask(&RtcTask))
			I2C_Control();
		while(RtcTask.type != COMPLITE_OPERATION){
			I2C_Control();
		}
	}while(RtcTask.count != 0);

	if(RtcTask.count)
		return 1;

	return 0;
}

UINT16 getRegSecund(void){
	UINT16 time;
	char msg[4];
	while(RtcTask.type != COMPLITE_OPERATION){
		I2C_Control();
	}

	do{
		RtcTask.data = msg;
		msg[0] = _TIME_DS1307;
		RtcTask.type = READ_I2C;
		RtcTask.count = -_TIME_NUM_RD;
		while(!PutI2CTask(&RtcTask))
			I2C_Control();
		while(RtcTask.type != COMPLITE_OPERATION){
			I2C_Control();
		}
	}while(RtcTask.count != 0);
	if(RtcTask.count)
		return 0;

	time = (UINT16)msg[2];
	time <<= 8;
	time |= 0x0ff & (UINT16)msg[1];
	return time;
}

char setCurrentSecund(stRtClock time){
	char msg[4];
	while(RtcTask.type != COMPLITE_OPERATION){
		I2C_Control();
	}
	do{
		RtcTask.data = msg;
		RtcTask.count = _TIME_NUM_WR;
		msg[2] = time.hour;
		msg[3] = 0;
		msg[0] = _TIME_DS1307;
		msg[1] = time.minute;
		RtcTask.type = WRITE_I2C;

		while(!PutI2CTask(&RtcTask))
			I2C_Control();
		while(RtcTask.type != COMPLITE_OPERATION){
			I2C_Control();
		}
	}while(RtcTask.count != 0);

	if(RtcTask.count)
		return 1;

	return 0;
}

char setRegSecund(UINT16 reg){
	stRtClock time;
	time.minute = reg &0x0ff;
	time.hour = reg >> 8;
	while(setCurrentSecund(time));
	return 1;
}

UINT32 getCurrentTimeAndData(void){
	UINT32 td;
	UINT16 tmp;
	tmp = getRegTime();
	td = tmp;
	td <<= 16;
	tmp = getRegData();
	td |= ((UINT32)tmp)&0x0ffff;
	return td;
}

INT8 getNVRam(UINT8 adr, UINT16 *val){
    UINT16 data;
    char msg[4];
    while(RtcTask.type != COMPLITE_OPERATION){
        I2C_Control();
    }

    do{
        RtcTask.data = msg;
        msg[0] = adr;
        RtcTask.type = READ_I2C;
        RtcTask.count = -_DATA_NUM_RD;
        while(!PutI2CTask(&RtcTask))
            I2C_Control();
        while(RtcTask.type != COMPLITE_OPERATION){
            I2C_Control();
        }
    }while(RtcTask.count != 0);
    if(RtcTask.count)
        return -1;

    data = msg[1];
    data <<= 8;
    data |= (0x0ff & (UINT16)msg[2]);
    *val = data;
    return 0;
}


char setNVRam(UINT8 adr, UINT16 val){
    char msg[3];
    while(RtcTask.type != COMPLITE_OPERATION){
        I2C_Control();
    }
    do{
        RtcTask.data = msg;
        RtcTask.count = _DATA_NUM_WR;
        msg[1] = val >> 8;
        msg[2] = val & 0x0ff;
        msg[0] = adr;
        RtcTask.type = WRITE_I2C;

        while(!PutI2CTask(&RtcTask))
            I2C_Control();
        while(RtcTask.type != COMPLITE_OPERATION){
            I2C_Control();
        }
    }while(RtcTask.count != 0);

    if(RtcTask.count)
        return -1;

    return 0;
}
