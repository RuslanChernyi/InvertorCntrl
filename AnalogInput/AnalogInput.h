/*
 * AnalogInput.h
 *
 *  Created on: 4 ρεπο. 2014
 *      Author: Tester
 */

#ifndef ___ANALOG_INPUT_H___
#define ___ANALOG_INPUT_H___

typedef struct _ANALOGINPUT{
	char type;
	float maxShift, minShift, mshtShift;
	float maxMsht, minMsht, paramMsht;
}stAnalogInputLim, *pAnalogInputLim;

#define _0_10mA 1
#define _4_20mA 2
#define _0_10V 4

float getAnalogSignal(UINT16 val, pAnalogInputLim lim);
float getAnalogParam(float val, pAnalogInputLim lim);
void cntrlAnalogTask(void);
float getAnalogTask(void);
float getSignalTask(void);
void cntrlAnalogOS(void);
float getAnalogOs(void);
float getSignalOs(void);

void mirrorAnalogInp(void);

void initAnalogOsSignal(void);
void initAnalogTaskSignal(void);
void initAllAnalogInput(void);

#endif
