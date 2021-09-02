/* ********************************************************
 * �������� ������� ����������
 * ********************************************************/

#include "added.h"
#include "AdcCntrl/AdcCntrl.h"
#include "ModBus/modbus.h"
#include "ConvertorCntrl/CntrlConvertor.h"
#include "Regulator.h"

#define _TAU_REG_I_LIM 0.3e-3

extern stCurrentOut OutCurrentVal;

stRegData CurrentLimByF;
float KPclB, KIclB, KPclS, KIclS;
float limCurrent, limCurrentBig, limCurrentSmall, maxCurrent;

float getCurrentLimit(void){
    return limCurrent;
}

void initKoefOverload(void){
    float tmp;
    float KP, KI/*, KD*/;
    //  уровень токоограниячения
	tmp = getLimitCurrent();
	tmp *= (1.4142 / 10);
	limCurrent = tmp;
	limCurrentBig = (tmp * 1.2) - limCurrent;
	limCurrentSmall = (tmp * 1.1) - limCurrent;

	KI = getOverloadKi();
	KI = KI * 1e-7;
	KP = getOverloadKp();
	KP = KP * 1e-4;
	KPclB = KP;
	KIclB = KI;

	KI = getRegister(_I_KOEF_LIM_I_BY_U);
    KI = KI * 1e-8;
    KP = getRegister(_P_KOEF_LIM_I_BY_U);
    KP = KP * 1e-5;
	setKoefReg(KP, KI, 0, _TAU_REG_I_LIM, &CurrentLimByF);
    KPclS = KP;
    KIclS = KI;

    if((KPclS == 0)&&(KIclS == 0)){
        KPclS = KPclB;
        KIclS = KIclB;
    }
    if((KPclB == 0)&&(KIclB == 0)){
        KPclB = KPclS;
        KIclB = KIclS;
    }

    KP = getRegister(_MAX_INV_FREQ);
    KP = KP * 0.1;
    setLimReg(0, KP, &CurrentLimByF);

}

void initOverloadCntrl(void){
	clrReg(&CurrentLimByF);
	initKoefOverload();
	maxCurrent = getAlarmCurrent();
}

void cntrlOverload(void){
	float tmp;

//	tmp = getAveragIm();
//
//	tmp = tmp - limCurrent;


	tmp = getImVal();

	tmp = tmp - limCurrent;

	if(tmp > limCurrentBig){
	    CurrentLimByF.Kp = KPclB;
	    CurrentLimByF.Ki = KIclB;
	}else if(tmp < limCurrentSmall){
	    CurrentLimByF.Kp = KPclS;
        CurrentLimByF.Ki = KIclS;
	}

	cntrlRegulyatorPI(tmp, &CurrentLimByF);
}

float getOverloadFrequency(void){
	return CurrentLimByF.reg;/**/
}

UINT8 testOvercurrent(void){
	if((CurrentLimByF.reg) > 0)//05.07.17
		return 1;
	else
		return 0;
}

UINT8 testLimitCurrent(void){
	float Ia, Ib, Ic;
	Ia = getMomentaryCurrentA();
	Ib = getMomentaryCurrentB();
	Ic = Ib+Ia;
	if(Ia > maxCurrent)
		return 1;
	if(Ib > maxCurrent)
		return 1;
	if(Ic > maxCurrent)
		return 1;
	if(Ia < -maxCurrent)
		return 1;
	if(Ib < -maxCurrent)
		return 1;
	if(Ic < -maxCurrent)
		return 1;

	return 0;
}

UINT16 testLooseBelt(){ //17.08.2021 Ruslan Chernyi
    //Goes to CntrlConvertor.c --> protectCntrl(void)
    /*контроль сухого хода*/
    float  iust;
    iust = getLooseBeltUst();
    unsigned int temp = OutCurrentVal.current;
    if(temp < iust)
         return 1;
    else
         return 0;
}

