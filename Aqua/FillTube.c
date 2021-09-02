/*
 * FillTube.c
 *
 *  Created on: 1 ����. 2015
 *      Author: Tester
 *
 *      ���������� �����
 */


#include "aqua.h"
#include "SysTimer/sysTime.h"
#include "AnalogInput/AnalogInput.h"
#include "ConvertorCntrl/CntrlConvertor.h"
#include "ModBus/modbus.h"
#include "OsCntrl/OsCntrl.h"
#include "ConvertorCntrl/motorCntrl.h"

#define c1sec 1000

UINT32 timeFillTube;
UINT8 stateFillTube;

UINT16 initFillTube(void){
	int tmp;
	tmp = getRegister(_FILL_TUBE_TEMP);
	tmp = tmp / 5;
	setTimeAccel(tmp);

	tmp = getRegister(_FILL_TUBE_TASK);
	setFtaskOs(((float)tmp)/10);
	timeFillTube = getRegister(_FILL_TUBE_TIME);
	timeFillTube *= 60;
	stateFillTube = 0;
	return getRegister(_FILL_TUBE_ENABLE);
}

char testFuillTube(void){
	float tmp;
	tmp = getRegister(_FILL_TUBE_LEVEL);
	tmp = tmp / 10;
	if(tmp > getAnalogOs())
		return 1;
	else
		return 0;
}

UINT8 cntrlFillTube(void){
    switch(stateFillTube){
        case 0:{
            float tmp;
            tmp = getRegister(_FILL_TUBE_TASK);
            tmp = tmp * 0.1;
            setFtaskOs(tmp);
            timeFillTube = getRegister(_FILL_TUBE_TIME);
            if(!timeFillTube)
                timeFillTube = 0x0fffff;
            timeFillTube *= 3;
            stateFillTube = 1;
            break;
        }
        case 1:{
            float tmp;
            tmp = getRegister(_FILL_TUBE_TASK);
            tmp = tmp / 10;
            if(tmp <= getInvertorFreq()){
                stateFillTube = 2;
            }
            break;
        }
        case 2:{
            float tmp = getRegister(_FILL_TUBE_LEVEL);
            if(tmp){
                tmp = tmp / 10;
                if(tmp < getAnalogOs()){
                    stateFillTube = 3;
                }
            }
            timeFillTube--;
            if(!timeFillTube)
                stateFillTube = 4;
            break;
        }
        case 3:{
            float tmp = getRegister(_FILL_TUBE_LEVEL);
            tmp = tmp / 10;
            if(tmp < getAnalogOs())
                return 1;
            else
                stateFillTube = 2;
            break;
        }
        case 4:{
            return 0x0ff;
        }
    }
    return 0;
}
