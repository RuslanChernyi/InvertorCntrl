/*
 * IRcompensation.c
 *
 *  Created on: 29 ���. 2014
 *      Author: Tester
 */

#include "IRcompensation.h"
#include "AdcCntrl/AdcCntrl.h"
#include "CntrlConvertor.h"
#include "ModBus/modbus.h"

stIrCompensation Compensation;

float getAddedFrequency(void){
	return Compensation.addedFreq;
}

float getAddedVoltag(void){
	return Compensation.addedVoltag;
}

void initIrCompConst(void){
	float tmp;
	Compensation.FrequencyCompensation = getRegister(_MAGTETIC_FREQ);
	Compensation.FrequencyCompensation = Compensation.FrequencyCompensation / 100;
	Compensation.CosFiNorma = getRegister(_MOTOR_COS_FI_NOM);
	Compensation.CosFiNorma =  Compensation.CosFiNorma / 100;

	if(Compensation.FrequencyCompensation){
		tmp = getRegister(_MAGTETIC_VOLTAG);
		tmp = tmp / 10;
		Compensation.KoefAddedVoltag = tmp / Compensation.FrequencyCompensation;
		tmp = getRegister(_FREQ_BUMP);
		tmp = tmp / 100;
		Compensation.KoefAddedFreq = tmp / Compensation.FrequencyCompensation;
	}
}

void initIrCompensation(void){
	Compensation.addedFreq = 0;
	Compensation.addedVoltag = 0;
	Compensation.KoefAddedFreq = 0;
	Compensation.KoefAddedVoltag = 0;
	initIrCompConst();
}

void cntrlIrCompensation(void){
	float cosinus;
	float level;
	if(Compensation.FrequencyCompensation){
		level = getInvertorFreq();
		if(level < 0) level = -level;
		if(level < Compensation.FrequencyCompensation){
			cosinus = getAveragIm();
			if(cosinus)
				cosinus = getAveragId() / cosinus;
			else
				cosinus = 0;
			if(cosinus < 0) cosinus = -cosinus;
			level = Compensation.FrequencyCompensation - level;
			Compensation.addedVoltag = level * Compensation.KoefAddedVoltag;
			if(cosinus > Compensation.CosFiNorma){
				level *= cosinus * Compensation.KoefAddedFreq;
				Compensation.addedFreq = level;
			}else{
				Compensation.addedFreq = 0;
			}
		}else{
			Compensation.addedVoltag = 0;
			Compensation.addedFreq = 0;
		}
	}

}
