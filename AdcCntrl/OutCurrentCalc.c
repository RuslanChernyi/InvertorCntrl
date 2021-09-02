/*
 * ���������� ������� ��������� ���� ������������
 * ��������� ����������
 */

#include "added.h"
#include <math.h>
#include "algebra/Trigonom.h"
#include "OutCurrentCalc.h"
#include "ConvertorCntrl/statusConvertor.h"
#include "ConvertorCntrl/CntrlConvertor.h"
#include "AdcCntrl.h"
#include "cntrlPWM/cntrlPwm.h"
#include "FIR_Filtr.h"

#define c1bySQRT3 ((float)0.57735)
#define cMinTan ((float)0.01)
#define cPi ((float)3.14159)

stCurrentOut OutCurrentVal;
INT8 statusOutCurrent;

void refrOutCurrent(void){
	statusOutCurrent = getOutCurrent(&OutCurrentVal);
}

float getIdVal(void){
	float tmp;
	disablePwmInt();
	tmp = OutCurrentVal.currentId;
	enablePwmInt();
	return tmp;
}

float getIqVal(void){
	float tmp;
	disablePwmInt();
	tmp = OutCurrentVal.currentIq;
	enablePwmInt();
	return tmp;
}

float getImVal(void){
	float tmp;
	disablePwmInt();
	tmp = OutCurrentVal.current;
	enablePwmInt();
	return tmp;
}

float getAlfaVal(void){
	float tmp;
	disablePwmInt();
	tmp = OutCurrentVal.coner;
	enablePwmInt();
	return tmp;
}

INT8 getStatusVal(void){
	return statusOutCurrent;
}

INT8 getOutCurrent(pCurrentOut current){
	if(testRunState()){
		float iAl, iBe;
		float Id, Iq;
		float sinA, cosA;
		UINT16 alfa;

		alfa = getCurrentConer();

//		if(testStepDirection() < 0){
//			iBe = getMomentaryCurrentB();
//			iAl = -getMomentaryCurrentA()-iBe;
//			iBe = -iAl - iBe;
//			alfa = alfa + c90dgr;
//		}else{
//			alfa = alfa + c120dgr;
//			alfa = alfa - c90dgr;
//			iBe = getMomentaryCurrentB();
//			iAl = -getMomentaryCurrentA()-iBe;
//		}

		iBe = getMomentaryCurrentB();
		iAl = -getMomentaryCurrentA()-iBe;
//		iAl = getMomentaryCurrentA();
		alfa = alfa - c120dgr;
//		alfa = alfa - c180dgr;

		iBe *= 2;
		iBe += iAl;
		iBe = iBe * c1bySQRT3;

		sinA = getSin(alfa);
		cosA = getCos(alfa);

		Id = cosA * iAl;
		Id += sinA * iBe;
		if(testStepDirection() < 0){
			Iq = sinA * iAl;
			Iq -= cosA * iBe;
		}else{
			Iq = cosA * iBe;
			Iq -= sinA * iAl;
		}

		current->currentId = Id;
		current->currentIq = Iq;

		iAl = Id*Id + Iq*Iq;
		current->current = sqrt(iAl);

		if(Id < cMinTan){
			if(Iq < cMinTan){
				current->coner = 0;

				return -2;
			}else{
				if(Iq < (cMinTan * 10))
					current->coner = 0;
				else
					current->coner = (cPi/2);

				return -1;
			}
		}else{
			current->coner = atan(Iq/Id);
		}

		return 1;

	}else{
		current->current = 0;
		current->coner = 0;
		current->currentId = 0;
		current->currentIq = 0;
		return 0;
	}
}

Int16 FiltrIq[Ntap];
float flFiltrIq;

float getFiltrIq(void){
	return flFiltrIq;
}

void initFiltrIv(void){
	Int16 i;
	flFiltrIq = 0;
	for(i=0; i<Ntap; i++)
		FiltrIq[i]=0;
}

void cntrlFiltrIv(void){
	Int16 val;
	val = (Int16)(OutCurrentVal.currentIq*100);
	val = getFiltrVal(FiltrIq, val);
	flFiltrIq = val;
	flFiltrIq = flFiltrIq/100;
}
