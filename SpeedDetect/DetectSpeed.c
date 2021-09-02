/*
 * DetectSpeed.c
 *
 *  Created on: 26 ���. 2015
 *      Author: Tester
 */

#include "added.h"
#include "AdcCntrl/AdcCntrl.h"
#include "ModBus/modbus.h"
#include "ConvertorCntrl/statusConvertor.h"
#include "cntrlPWM/cntrlPwm.h"
#include "blink/blink.h"
#include "ConvertorCntrl/CntrlConvertor.h"
#include "ConvertorCntrl/motorCntrl.h"

float freqFind, voltagFind;
float stepFreqFind, deltaFindM, koefFind1, koefFind2;
float currentLimit, currentSubLim;
char findDir;

void initFintSpeed(void){
	float tmp;
	tmp = getRegister(_TIME_FIND);
	tmp = tmp * 1000;
	freqFind = getMaxInvertorFrequency();
	freqFind /= 10;
	stepFreqFind = freqFind / tmp;
	findDir = getRegister(_DIRECTION_FIND);
	currentLimit = getRegister(_CURRENT_LIMIT_SF);
	currentLimit /= 10;
	currentSubLim = currentLimit / 4;
	voltagFind = 0;
	deltaFindM = 0;
	tmp = getRegister(_FIND_KOEF_I);
	tmp /= 1000;
	koefFind2 = getRegister(_FIND_KOEF_P);
	koefFind2 /= 50;
	koefFind1 = koefFind2 + tmp;
	koefFind2 = -koefFind2;
}

UINT16 cntlFindSpeed(float *taskF, float *taskV,
					 float *currentF, float *currentV){
	float current, voltag, tmp;

	current = getImVal();
	voltag = getVoltagFromFrequency(freqFind);

	if((voltag <= voltagFind) || (freqFind == 0)){
		if(findDir){
			*taskF = -freqFind;
			*currentF = -freqFind;
		}else{
			*taskF = freqFind;
			*currentF = freqFind;
		}

		*taskV = voltagFind;
		*currentV = voltagFind;

		return _run_stage_convertor;
	}else{
		tmp = current - currentLimit;

		if((tmp < currentSubLim) && (tmp > (-currentSubLim))){
			freqFind = freqFind - stepFreqFind;
			if(freqFind < 0)
				freqFind = 0;
		}else{
			tmp = tmp * koefFind1 + deltaFindM * koefFind2;
			voltagFind += tmp;
			deltaFindM = tmp;
		}
		voltag = scaleVoltagPwm(tmp);
		setStepAmpl(voltag);
		if(findDir)
			setStepConer(-freqFind);
		else
			setStepConer(freqFind);
		return _soft_start_convertor;
	}
}
