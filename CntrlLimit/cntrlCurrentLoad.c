/* ******************************************************************
 * �������� ���� ���������� I*t (I^2*t)
 * ******************************************************************/

#include "added.h"
#include "AdcCntrl/AdcCntrl.h"
#include "ModBus/modbus.h"
#include "CntrlLimit/cntrlLimit.h"

#define deltaT 0.001 /*1ms*/

#define getDeltaT() 0.001

float IbyT;
float IbyTLimit;
float nomCurrent, nomCurrentH;
float (*cntrlLimitAlg)(void);

float getIbyT(void){
	float tmp;
	tmp = getLoadCurrent();
	if(tmp > nomCurrentH){
		IbyT += getDeltaT();
	}
	else{
		if(IbyT > 0){
			if(tmp < nomCurrent)
			IbyT -= getDeltaT();
			if(IbyT < 0)
				IbyT = 0;
		}
	}
	return 0;
}

float getSqrIbyTnow(float current){
	float tmp;
	tmp = (current * current);
	tmp -= (nomCurrent * nomCurrent);
	return (tmp * getDeltaT());
}

float getSqrIbyT(void){
	float current;
	current = getLoadCurrent();

	if(current > (nomCurrentH)){
		IbyT += getSqrIbyTnow(current);
	}
	else{
		if(current < nomCurrent){
			IbyT += getSqrIbyTnow(current);
		}
	}
	if(IbyT < 0)
		IbyT = 0;
	return IbyT;
}

void initIbyTParam(void){
	float time;
	time = getRegister(_OVERLOAD_TIME);
	time *= 100;
	IbyTLimit = getLimitIbyT();
	IbyTLimit /= 10; 	//1000;		// ��� ��������� ���� � �
	IbyTLimit *= 1.4142;
	nomCurrent = getRegister(_MOTOR_CURRENT_NOM);//getRegister(_NOMINAL_CURRENT);
	nomCurrent *= 1.4142;
	nomCurrent /= 10;

	if(!getRegister(_OVERLOAD_TYPE)){
		cntrlLimitAlg = getSqrIbyT;
		IbyTLimit = (IbyTLimit * IbyTLimit) - (nomCurrent * nomCurrent);
		if(IbyTLimit < 0)
			IbyTLimit = 0;
		IbyTLimit *= (time / 1000);
	}
	else{
		cntrlLimitAlg = getIbyT;
		nomCurrent = IbyTLimit;
		IbyTLimit = time / 1000;
	}
	nomCurrentH = nomCurrent * 1.02;
}

void initIbyTLimit(void){
	initIbyTParam();
	IbyT = 0;
}

void cntrlIbyT(void){
	(*cntrlLimitAlg)();
	if(IbyT < 0)
		IbyT = 0;
}

UINT8 testIbyT(void){
	if(IbyT > IbyTLimit)
		return 1;
	else
		return 0;
}

float getValIbyT(void){
	float tmp;
	tmp = IbyT * 100.0;
	return (tmp / IbyTLimit);
}
