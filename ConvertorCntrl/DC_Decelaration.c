/*
 * DC_Deceleration.c
 *
 *  Created on: 13 ����. 2014
 *      Author: Tester
 */

#include "added.h"
#include "ModBus/modbus.h"
#include "cntrlPWM/cntrlPwm.h"
#include "ConvertorCntrl/motorCntrl.h"
#include "CntrlLimit/cntrlLimit.h"

float FreqDcDecel;
INT16 TimeDecelDc, TimeDecelDcBgn;
float DcVoltagLevel;

void initDcDecelation(void){
	FreqDcDecel = getRegister(_FREQ_DC_DECEL);
	FreqDcDecel /= 10;
	TimeDecelDc = getRegister(_TIME_DC_DECEL);
	TimeDecelDc *= 100;
	TimeDecelDcBgn = TimeDecelDc;
	DcVoltagLevel = getRegister(_VOLTAG_DC_DECEL);
	DcVoltagLevel = scaleVoltagPwm(DcVoltagLevel);
	if(!TimeDecelDc)
		FreqDcDecel = 0;
}

INT16 DcDecelatration(float *freq, float *voltag){
	float tmp;
	if(FreqDcDecel){
		tmp = *freq;
		if(tmp < 0) tmp = -tmp;

		if(tmp < FreqDcDecel){
			enableCntrlPwm();
			if(!testOvercurrent()){
				setStepAmpl(DcVoltagLevel);
			}else{
				setStepAmpl(0);
			}
//			setStepAmpl(DcVoltagLevel);
			setStepConer(0);
			TimeDecelDc--;
			return TimeDecelDc;
		}else{
			TimeDecelDc = TimeDecelDcBgn;
			return -1;
		}
	}else
		return -1;
}
