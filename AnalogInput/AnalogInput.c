/*
 * AnalogInput.c
 *
 *  Created on: 4 ����. 2014
 *      Author: Tester
 */

#include "added.h"
#include "AdcCntrl/AdcCntrl.h"
#include "AnalogInput.h"
#include "ModBus/modbus.h"

#define c10mA 10.0
#define c10V 10.0
#define c16mA 16.0
#define c20mA 20.0
#define c4mA 4.0

#define c4mAMsr (((float)63450.0 * 4) / 20)
#define c16mAMsr (((float)63450.0 * 16) / 20)
#define c20mAMsr ((float)63450.0 / 20)
#define cMsr10mA (((float)63450.0 * 10) / 20)
#define cMsr20mA (63450.0)
#define cMsr10V (63450.0)

#define _LEVEL_MIN_ANALOG 1000.0

stAnalogInputLim limitOS, limitTask;
float inputTask, inputOs;
float signalOs, signalTask;
float paramOs, paramTask;

float getAnalogSignal(UINT16 val, pAnalogInputLim lim){
	float tmp;

	if(val>_LEVEL_MIN_ANALOG)
	    tmp = val;
	else
	    tmp = 0;

	tmp -= lim->minShift;

	if(tmp < 0/*lim->minShift*/)
		tmp = 0/*lim->minShift*/;

	if(tmp > lim->maxShift)
		tmp = lim->maxShift;

	tmp *= lim->mshtShift;

//	if(lim->type == _4_20mA)
//		tmp += c4mA;

	return tmp;
}

float getAnalogParam(float val, pAnalogInputLim lim){
	if(lim->type == _4_20mA){
		val = val - c4mA;
		if(val < 0)
			val = 0;
	}

	val *= lim->paramMsht;
	val += lim->minMsht;

	if(val < lim->minMsht)
		val = lim->minMsht;

	if(val > lim->maxMsht)
		val = lim->maxMsht;

	return val;
}

void cntrlAnalogTask(void){
	UINT16 tmp;

	tmp = getAveregResult(_TASK_ANALOG_VAL);

	signalTask = getAnalogSignal(tmp, &limitTask);
	paramTask = getAnalogParam(signalTask, &limitTask);
}

float getAnalogTask(void){
	return paramTask;
}

float getSignalTask(void){
	return signalTask;
}

void cntrlAnalogOS(void){
	UINT16 tmp;

	tmp = getAveregResult(_OS_INP_VAL);

	signalOs = getAnalogSignal(tmp, &limitOS);
	paramOs = getAnalogParam(signalOs, &limitOS);
}

float getAnalogOs(void){
	return paramOs;
}

float getSignalOs(void){
	return signalOs;
}

void initAnalogTaskSignal(void){
	float val, level;
	UINT16 type;

	type = getRegister(_ANALOG_INP_MODE);
	switch(type){
		default:
		case _0_10V:{
			level = getRegister(_MIN_LEVEV_CORR_AN_IN);
			level = level / 1000;
			limitTask.minShift = level * cMsr10V;

			val = getRegister(_MAX_LEVEV_CORR_AN_IN);
			val = (1000/*+ 100%*/ - val) / 1000;
			limitTask.maxShift = val * cMsr10V;

			val = c10V;
			limitTask.mshtShift = val/(limitTask.maxShift - limitTask.minShift);
			break;
		}
		case _0_10mA:{
			level = getRegister(_MIN_LEVEV_CORR_AN_IN);
			level = level / 1000;
			limitTask.minShift = level * cMsr10mA;

			val = getRegister(_MAX_LEVEV_CORR_AN_IN);
			val = (1000/*+ 100%*/ - val) / 1000;
			limitTask.maxShift = val * cMsr10mA;

			limitTask.mshtShift = c10mA/(limitTask.maxShift - limitTask.minShift);
			break;
		}
		case _4_20mA:{
			level = getRegister(_MIN_LEVEV_CORR_AN_IN);
			level = level / 1000;
			limitTask.minShift = (level * cMsr20mA)/* + c4mAMsr*/;

			val = getRegister(_MAX_LEVEV_CORR_AN_IN);
			val = (1000/*+ 100%*/ - val) / 1000;
			limitTask.maxShift = val * cMsr20mA;

			val = c20mA;
			limitTask.mshtShift = val/(limitTask.maxShift - limitTask.minShift);
			break;
		}
	}

	if(getRegister(_MODE_OS_PID))
		level = getRegister(_MIN_TASK_OS);
	else
		level = getRegister(_MIN_TASK_AN_IN);
	level = level / 10;/*0;*/
	limitTask.minMsht = level;

	if(getRegister(_MODE_OS_PID))
		val = getRegister(_MAX_TASK_OS);
	else
		val = getRegister(_MAX_TASK_AN_IN);
	val = val / 100;
	limitTask.maxMsht = val;
	limitTask.paramMsht = (val - level);

	switch(type){
		default:
		case _0_10V:{
			limitTask.paramMsht = limitTask.paramMsht/c10V;
			break;
		}
		case _0_10mA:{
			limitTask.paramMsht = limitTask.paramMsht/c10mA;
			break;
		}
		case _4_20mA:{
			limitTask.paramMsht = limitTask.paramMsht/c16mA;
			break;
		}
	}

	limitTask.type = type;
}

void initAnalogOsSignal(void){
	float val, level;
	UINT16 type;

	type = getRegister(_OS_INP_MODE);
	switch(type){
		default:
		case _0_10V:{
			level = getRegister(_MIN_LEVEV_CORR_OS_IN);
			level = level / 1000;
			limitOS.minShift = level * cMsr10V;

			val = getRegister(_MAX_LEVEV_CORR_OS_IN);
			val = (1000/*+ 100%*/ - val) / 1000;
			limitOS.maxShift = val * cMsr10V;

			val = (float)c10V;
			level = (limitTask.maxShift - limitTask.minShift);
			limitOS.mshtShift = val/level;
			break;
		}
		case _0_10mA:{
			level = getRegister(_MIN_LEVEV_CORR_OS_IN);
			level = level / 1000;
			limitOS.minShift = level * cMsr10mA;

			val = getRegister(_MAX_LEVEV_CORR_OS_IN);
			val = (1000/*+ 100%*/ - val) / 1000;
			limitOS.maxShift = val * cMsr10mA;

			limitOS.mshtShift = c10mA/(limitTask.maxShift - limitTask.minShift);
			break;
		}
		case _4_20mA:{
			level = getRegister(_MIN_LEVEV_CORR_OS_IN);
			level = level / 1000;
			limitOS.minShift = (level * cMsr20mA)/* + c4mAMsr*/;

			val = getRegister(_MAX_LEVEV_CORR_OS_IN);
			val = (1000/*+ 100%*/ - val) / 1000;
			limitOS.maxShift = val * cMsr20mA;

			val = c20mA;
			limitOS.mshtShift = val/(limitTask.maxShift - limitTask.minShift);
			break;
		}
	}

	level = getRegister(_MIN_OS_AN_IN);
	level = level / 100;
	limitOS.minMsht = level;
	val = getRegister(_MAX_OS_AN_IN);
	val = val / 100;
	limitOS.maxMsht = val;
	limitOS.paramMsht = (val - level);

	switch(type){
		default:
		case _0_10V:{
			limitOS.paramMsht = limitOS.paramMsht/c10V;
			break;
		}
		case _0_10mA:{
			limitOS.paramMsht = limitOS.paramMsht/c10mA;
			break;
		}
		case _4_20mA:{
			limitOS.paramMsht = limitOS.paramMsht/c16mA;
			break;
		}
	}

	limitOS.type = type;
}

void initAllAnalogInput(void){
	initAnalogOsSignal();
	initAnalogTaskSignal();
}

void mirrorAnalogInp(void){
	float tmp;
	UINT16 out;
	tmp = getSignalOs();
	tmp *= 10;
	out = (UINT16)tmp;
	SetInpReg(_ANALOG_INPUT_OS, out);
	tmp = getSignalTask();
	tmp *= 10;
	out = (UINT16)tmp;
	SetInpReg(_ANALOG_INPUT_TASK, out);

	tmp = getAnalogTask();
	tmp *= 10;
	out = (UINT16)(tmp);
	SetInpReg(_ANALOG_VALUE_TASK, out);
	tmp = getAnalogOs();
	tmp *= 10;
	out = (UINT16)(tmp);
	SetInpReg(_ANALOG_VALUE_OS, out);
}
