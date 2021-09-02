/* ******************************************************************
 * �������� ������ ePWM
 * ******************************************************************/

/*
 * amplVout - ��������� ��������� ���������� 0x0...0x01fff(0x03fff) -> 0..1.0(2.0)
 * stepVout - ��� ������� ��������� ���������� (X*65535) dgr per period PWM (0x07fff -> 180 dgr)
 * */

#include "added.h"
#include "cntrlPwm.h"
#include "algebra/Trigonom.h"
#include "AdcCntrl/AdcCntrl.h"
#include "ConvertorCntrl/CntrlConvertor.h"
#include "blink/blink.h"
#include "ModBus/modbus.h"
#include "ConvertorCntrl/statusConvertor.h"

UINT32 conerVout;
INT32 stepVout;
float nominalStep, nominalPwr;
UINT16 sectorConer, sectorPntr;
float amplVout;
UINT16 TimeTS;

float getAmplVout(void){
    return amplVout;
}

UINT16 getCurrentConer(void)
{
	return (conerVout>>16);
}

UINT16 getSectorConer(void){
	return sectorConer;
}

void setPwmConer(void){
	conerVout = 0;
}

/*
 * ������������� ��� ���������� ������� ���������. ������� (�����������)
 */
void presetPwmTime(/*UINT16 pwm, */UINT16 pwr){
	float tmp;
	UINT16 pwm;
	pwm = getRegister(_PWM_FRC);
	switch(pwm){
		default:{
			pwm = 3125;
			break;
		}
		case 1:{
			pwm = 1562;
			break;
		}
		case 2:{
			pwm = 4688;
			break;
		}
		case 3:{
			pwm = 6250;
			break;
		}
	}
	tmp = c180dgrBy32;
	tmp *= 2;
	tmp *= pwr;
	tmp /= pwm;
	tmp /= 100;
	nominalStep = tmp;
	nominalPwr = pwr / 100;
	TimeTS = getPeriodPWM();
	sectorConer = 0;
	sectorPntr = 0;
	amplVout = 0;
	conerVout = 0;
	stepVout = 0;
	initBreak();
}

void rstPwmCntrl(void){
	cntrlIO_PIN(0);
	setStepConer(0);
	setStepAmpl(0);
}

void enableCntrlPwm(void){
	setStepConer(0);
	setStepAmpl(0);
	cntrlIO_PIN(1);
}

void setStepConer(float pwr){
	float tmp;
	tmp = nominalStep;
	tmp *= pwr;
	tmp /= nominalPwr;
	disablePwmInt();
	stepVout = (INT32)tmp;
	enablePwmInt();
}

INT16 testStepDirection(void){
	if(stepVout&0x080000000)
		return -1;
	else{
		if(stepVout)
			return 1;
		else
			return 0;
	}
}

void setStepAmpl(float pwr){
	amplVout = pwr;
}

void rotateVectorVout(void)
{
	conerVout += stepVout;
	sectorPntr = conerVout >> 16;
	sectorConer = sectorPntr % c60dgr;
	sectorPntr = sectorPntr / c60dgr;
	if(sectorPntr > 5){
		sectorPntr = 5;
		sectorConer = c60dgr-1;
	}
}

void getTau(UINT16 coner, float* TA, float *TB)
{
	*TA = getSin(c60dgr-coner) * c2bySqrt3;

	*TB = getSin(coner) * c2bySqrt3;
}

void getTimePWM(float TA, float TB, UINT16 *time)
{
	float T0;
	UINT16 A,B,C;

	T0 = (float)TimeTS;

	TA *= amplVout;
	TA *= (float)TimeTS;
	TA /= (float)mshtTime;
	TB *= amplVout;
	TB *= (float)TimeTS;
	TB /= (float)mshtTime;

	if(TA > T0)
		TA = T0;
	if(TB > T0)
		TB = T0;

	if((TA + TB) > T0){
		/*TA = T0 / 2;
		TB = T0 / 2;*/
		if(TA > TB)
			TB = T0 - TA;
		else{
			if(TB > TA)
				TA = T0 - TB;
			else{
				TA = T0 / 2;
				TB = T0 / 2;
			}
		}
	}

	T0 -= TA;
	T0 -= TB;
	if(T0 < 0)
		T0 = 0;
	T0 /= 2;

	switch(sectorPntr){
		case 0:{
			A = (UINT16)T0;
			B = (UINT16)(T0+TA);
			C = (TimeTS-(UINT16)T0);
			break;
		}
		case 1:{
			A = (UINT16)(T0+TB);
			B = (UINT16)(T0);
			C = (TimeTS-(UINT16)T0);
			break;
		}
		case 2:{
			A = (TimeTS-(UINT16)T0);
			B = (UINT16)(T0);
			C = (UINT16)(T0+TA);
			break;
		}
		case 3:{
			A = (TimeTS-(UINT16)T0);
			B = (UINT16)(T0+TB);
			C = (UINT16)(T0);
			break;
		}
		case 4:{
			A = (UINT16)(T0+TA);
			B = (TimeTS-(UINT16)T0);
			C = (UINT16)(T0);
			break;
		}
		case 5:{
			A = (UINT16)T0;
			B = (TimeTS-(UINT16)T0);
			C = (UINT16)(T0+TB);
			break;
		}
		default:
		case 6:{
			A = (TimeTS >> 1);
			B = (TimeTS >> 1);
			C = (TimeTS >> 1);
		}
	}
	if(A > TimeTS)
		A = TimeTS;
	if(B > TimeTS)
		B = TimeTS;
	if(C > TimeTS)
		C = TimeTS;
	*time = A; time++;
	*time = B; time++;
	*time = C;
}

void getPhaseTimePWM(UINT16 *time){
	UINT16 coner, conerPrev;
	float tauA, tauB;
	static char errorFound = 0;
//	setPin1();
	if(testErrorInv()){
		if(!errorFound){
			rstPwmCntrl();
			errorFound = 1;
		}
	}else{
		errorFound = 0;
		refrOutCurrent();
		conerPrev = getCurrentConer();
		rotateVectorVout();
		coner = getSectorConer();
		getTau(coner, &tauA, &tauB);
		getTimePWM(tauA, tauB, time);
		conerPrev = coner ^ conerPrev;
		conerPrev &= (0x08000 | 0x04000);
		cntrlZerroShift(conerPrev);/**/
		setFlagPwmSet();
	//	clrPin1();
		if(getStatusRun() < 0){
			cntrlBreak();
		}else{
			rstBreak();
		}
	}
}
