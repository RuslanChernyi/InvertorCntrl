/* ******************************************************************
 * �������� ���������� ����� ����������� ����
 * ******************************************************************/

#include "added.h"
#include "AdcCntrl/AdcCntrl.h"
#include "ModBus/modbus.h"

#define cSqrt_2 1.41/*1.4142*/
#define _minAddedDCLt 15.0
#define _minAddedDCBg 30.0
#define _minCurrent 0.2

float overvolt, deltaV1;
float maxVoltag, minVoltag, nominalVoltag;
float overvolt_K1, overvolt_K2;
float limitVoltag;

void initOverVoltKoef(void){
	overvolt_K1 = getOvervoltKi();
	overvolt_K1 = overvolt_K1 / 20000;
	overvolt_K2 = getOvervoltKp();
	overvolt_K2 = overvolt_K2 / 1000;
//	overvolt_K1 += overvolt_K2;
//	overvolt_K2 = -overvolt_K2;
	maxVoltag = getAlarmVoltag();
	minVoltag = getMinimalUd();
	limitVoltag = getRegister(_P_LEVEL_VOLTAG_LIM);
}

void initOvervoltCntrl(void){
	overvolt = 0;
	deltaV1 = 0;

	initOverVoltKoef();

	nominalVoltag = getNominalVoltag();
	nominalVoltag = nominalVoltag / 10;
	nominalVoltag *= cSqrt_2;
}

void stabilisationByUdc(float *voltag){
	float tmp = *voltag;
	tmp *= nominalVoltag;
	tmp = tmp / getMiddleVoltagDC();//getVoltagDC();//
	*voltag = tmp;
}

void cntrlOvervoltag(void){
	float tmp, delta;

	tmp = getVoltagDC() - getMiddleVoltagDC() ;
	tmp -= _minAddedDCBg;
//	tmp -= _minAddedDCLt;
//	tmp -= limitVoltag;

//	if(tmp > 0)
//		delta = overvolt_K1 * tmp + overvolt_K2 * deltaV1;
//	else{
//		if(overvolt > 0)
//			delta = overvolt_K1 * tmp + overvolt_K2 * deltaV1;
//		else
//			delta = 0;
//	}

	delta = overvolt_K1 * tmp;
	overvolt += delta;

	if(overvolt < 0)
		overvolt = 0;


	tmp = getVoltagDC() - getMiddleVoltagDC() ;
	tmp -= limitVoltag;
	if(tmp > 0)
		deltaV1 = delta * overvolt_K2;
	else
		deltaV1 = 0;

//	deltaV1 = tmp;
}

float getOvervoltagFrequency(void){
	return /*overvolt + */deltaV1;
}

UINT8 testOvervoltag(void){
	if(overvolt > 0)
		return 1;
	else
		return 0;
}

INT8 testLimitVoltag(void){
	float tmp;
	tmp = getVoltagDC();
//	tmp = tmp - maxVoltag;
	if(tmp > maxVoltag)
		return 1;
	else
		return 0;
}

INT8 testUnderVoltag(void){
	float tmp = getMiddleVoltagDC();
	if(tmp < minVoltag)
		return 1;
	else
		return 0;
}
