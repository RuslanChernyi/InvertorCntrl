/*
 * IRcompensation.h
 *
 *  Created on: 29 ���. 2014
 *      Author: Tester
 */

#ifndef ___IR_COMPENSATION_H___

typedef struct _IR_COMPENSATION{
	float addedFreq;
	float addedVoltag;
	float CosFiNorma;
	float FrequencyCompensation;
	float KoefAddedVoltag;
	float KoefAddedFreq;
}stIrCompensation, *pIrCompensation;


float getAddedFrequency(void);
float getAddedVoltag(void);
void initIrCompensation(void);
void cntrlIrCompensation(void);
void initIrCompConst(void);

#endif
