/*
 * пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅ
 */

#include "added.h"
#include "SysTimer/sysTime.h"
#include "AdcCntrl/AdcCntrl.h"
#include "ModBus/modbus.h"
#include "OsCntrl.h"
#include "KaskadOs.h"
#include "cntrlPidSimple.h"
#include "AnalogInput/AnalogInput.h"
#include "Aqua/aqua.h"

char modeOs;
UINT16 delayPID;
UINT16 statusOS;
UINT16 OsTaskMode;
float taskFinvOs;

INIT_DELAY(PID_DL)

void (*PidOsPrg)(void);

void setFtaskOs(float task){
	taskFinvOs = task;
}

float getFtaskOs(void){
	return taskFinvOs;
}

void initOsPid(char full){
	delayPID = c10ms;
	modeOs = getRegister(_MODE_OS_PID);
	//05.08.21 изменено
	//modeOs=3;
	switch(modeOs){
		default:{
			PidOsPrg = &noPidOs;
			taskFinvOs = 0;
			break;
		}
		case 1:{
			//пїЅпїЅпїЅпїЅпїЅпїЅпїЅ(пїЅпїЅпїЅпїЅпїЅпїЅпїЅ) пїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ
			PidOsPrg = &simplePidOs;
			initPidCntrlSimple();
			taskFinvOs = 0;
			break;
		}
		case 2:{
			//пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ
			PidOsPrg = &KaskadPidOs;
			break;
		}
		case 3:{
			//пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ "пїЅпїЅпїЅпїЅ" пїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ
			initAqua();
			PidOsPrg = &waterPidOs;
			break;
		}
	}
	if(full){
		ADD_DELAY(PID_DL, c10ms);
	}else{
		SET_DELAY(PID_DL, c10ms);
	}
	OsTaskMode = getRegister(_MODE_TASK_OS);
}

void setAtomPidDelay(UINT16 delay){
	delayPID = delay;
	SET_DELAY(PID_DL, delayPID);
}

void cntrlOsPid(void){
	(*PidOsPrg)();
}

void simplePidOs(void){/*
  пїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ*/
	if(TEST_DELAY(PID_DL)){
		SET_DELAY(PID_DL, c20ms);
		cntrlPidSimple();
	}
}

void noPidOs(void){/*
  пїЅпїЅпїЅ пїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ*/
	if(TEST_DELAY(PID_DL)){
		SET_DELAY(PID_DL, c250ms);
	}
}

void waterPidOs(void){/*
  пїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ*/
	if(TEST_DELAY(PID_DL)){
		SET_DELAY(PID_DL, delayPID);
		cntrlStateWater();
	}
}

void KaskadPidOs(void){/*
  пїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ*/
	if(TEST_DELAY(PID_DL)){
		SET_DELAY(PID_DL, delayPID);
        KaskadOsPidCntrl();
	}
}

float getNormaOs(void){
	float tmp;
	switch(OsTaskMode){
		default:{
			tmp = 0;
			break;
		}
		case 0:{
			tmp = getAnalogTask();
			break;
		}
		case 1:{
			tmp = getRegister(_OS_PID_TASK);
			tmp *= 0.01;
			break;
		}
	}
	return tmp;
}

void setStatusOs(UINT16 stat){
	statusOS |= stat;
}

void clrStatusOs(UINT16 stat){
	statusOS &= ~stat;
}

UINT16 getStatusOs(void){
	return statusOS;
}
