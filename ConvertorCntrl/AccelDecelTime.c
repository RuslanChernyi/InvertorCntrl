/*
    ��������� ������� �������/����������
*/

#include "ModBus/modbus.h"
#include "AdcCntrl/AdcCntrl.h"
#include "CntrlLimit/cntrlLimit.h"

#define _bigTime 30

#define _stepFrequencyAccelDecel 0.1 /* 0.034 */

int AccelTime, DecelTime;
float stepAccel, stepDecel;
int kTimeDecel, kTimeDecelMax, timeAdded;
float cosFiTime;
float limitCurrentTm, limitCurrentId;

void setTimeAccel(int time){
	AccelTime = time;
}


void setTimeDecel(int time){
    DecelTime = time;
}


void presetAccelDecel(void){
	float tmp;
	/*
		�������� ������� � ������������ ������� �� �������
		S-�������� ��������������
	*/
	tmp = getRegister(_ACCEL_TIME);
	stepAccel = _stepFrequencyAccelDecel;
	tmp = tmp / 5;
//		if(tmp < 5)
//			AccelTime = 5;
//		else

	AccelTime = tmp;
	if(AccelTime < 2)
		AccelTime = 2;

	tmp = getRegister(_DECEL_TIME);
		stepDecel = _stepFrequencyAccelDecel;
		tmp = tmp / 5;
		DecelTime = tmp;
		if(!getRegister(_US_BREAK_KEY)){
			if(DecelTime < 10)
				DecelTime = 10;
		}

		if(DecelTime < 2)
			DecelTime = 2;

	kTimeDecel = getRegister(_KOEF_DECEL_TIME);
	kTimeDecel /= 5;
	kTimeDecelMax = getRegister(_KOEF_DECEL_MAX_TIME);
	kTimeDecelMax /= 5;
	timeAdded = kTimeDecelMax;

	cosFiTime = getRegister(_MOTOR_COS_FI_NOM);
	cosFiTime = cosFiTime / 100;
	limitCurrentTm = getNominalCurrent();
	limitCurrentTm *= (1.4142 / 50);
	limitCurrentId = limitCurrentTm * cosFiTime;

}

int getAccelTime(void)
{
	float cosinus;
	int time;
	time = AccelTime;
	if(testOvercurrent() | testOvervoltag()){
		return time + kTimeDecel;
	}
	else{
		cosinus = getImVal();
		if(cosinus > limitCurrentTm){
			cosinus = getIdVal() / cosinus;
			if(cosinus > cosFiTime)
				return time + kTimeDecel;
			if(getIdVal() > (limitCurrentId))
				return time + + kTimeDecel;

			if(getOutToDcCurrent() > limitCurrentTm)
				return time + + kTimeDecel;

			return time;
		}
	}
	return time;
}


int getDecelTime(void)
{
//	if(getIdVal()<0){
//		return DecelTime + kTimeDecel;
//	}
//	if(getAveragIm()<0){
	if(testOvervoltag()){
		if(timeAdded){
			timeAdded++;
		}else{
			timeAdded = kTimeDecelMax;
		}
		//return DecelTime + kTimeDecel;
	}else{
		if(timeAdded){
			timeAdded--;
		}
	}
	return DecelTime + timeAdded;
}

void rstDecelReg(void){
	timeAdded = kTimeDecelMax;
}

float getStepDecelCheng(void){
	return stepDecel;
}

float getStepAccelCheng(void){
	return stepAccel;
}


