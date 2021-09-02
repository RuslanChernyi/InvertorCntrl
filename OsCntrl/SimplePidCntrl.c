/*
 * SimplePidCntrl.c
 *
 *  Created on: 12 ����. 2014
 *      Author: Tester
 */

/*
* ��� ��������� ����������������
* ��������� � ����� �������� �����
*/

#include "added.h"
#include "AnalogInput/AnalogInput.h"
#include "ModBus/modbus.h"
#include "OsCntrl.h"

float OsTaskPid;
float K1os, K2os, K3os;
float valueOsM1, valueOsM2;
float maxOs, minOs, derrOs;
UINT16 DirectionReg;

void setLimitOs(float max, float min){
	maxOs = max;
	minOs = min;
}

void cntrlPIDregulator(void){
	float valueOS, deltaOS;
	valueOS = getAnalogOs();

	if(DirectionReg)
		valueOS = valueOS - getNormaOs();
	else
		valueOS = getNormaOs() - valueOS;

	if(valueOS < 0)
	    deltaOS = valueOS + derrOs;
	else
	    deltaOS = derrOs - valueOS;
	if(deltaOS > 0)
	    valueOS = 0;

	deltaOS = (valueOS * K1os);
	deltaOS += (valueOsM1 * K2os);
	deltaOS += (valueOsM2 * K3os);
	valueOsM2 = valueOsM1;
	valueOsM1 = valueOS;
	valueOS = deltaOS + OsTaskPid;

	if(valueOS > maxOs){
	    valueOS = maxOs;
		setStatusOs(_OVERREG_SIMPLE_REG);
		return;
	}else{
		clrStatusOs(_OVERREG_SIMPLE_REG);
	}
	if(valueOS < minOs){
	    valueOS = minOs;
		setStatusOs(_UNDERREG_SIMPLE_REG);
		return;
	}else{
		clrStatusOs(_UNDERREG_SIMPLE_REG);
	}
	OsTaskPid = valueOS;
}

void initPIDmplr(void){
    float kp, ki ,kd;
        kp = getRegister(_P_OS_PID);
        kp /= 100;
        ki = getRegister(_I_OS_PID);
        ki /= 10000;
        kd = getRegister(_D_OS_PID);
        kd /= 500;

        DirectionReg = getRegister(_DIRECTION_REG_OS);
        K1os = kp + ki + kd;
        K2os = -kp - (2 * kd);
        K3os = kd;

        derrOs = getRegister(_DELTA_OS);
        derrOs = derrOs / 10;
}

float getDeltaError(void){
    return derrOs;
}

void initPIDregulator(void){
	clrPIDCalc();
	initPIDmplr();
}

void clrPIDCalc(void){
    OsTaskPid = 0;
    valueOsM1 = 0;
    valueOsM2 = 0;
}
float setPidOs(float val){
    OsTaskPid = val;
    return OsTaskPid;
}
float getPidOs(void){
	return OsTaskPid;
}
