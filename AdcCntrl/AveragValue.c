/*
 * ���������� ����������� � ����������� �������
 */

#include "added.h"
#include "AdcCntrl.h"
#include "ConvertorCntrl/CntrlConvertor.h"
#include "stdlib.h"
#include "CntrlLimit/cntrlLimit.h"
#include "blink/blink.h"

#define _CURRENT_POINT 256
#define _VOLTAG_POINT 256
#define _CURRENT_POINT_MASK 255
#define _VOLTAG_POINT_MASK 255
#define _MUDUL_POINT 64

float arrayIa[_CURRENT_POINT], arrayIb[_CURRENT_POINT];
//float arrayIm[_MUDUL_POINT], arrayId[_MUDUL_POINT], arrayIq[_MUDUL_POINT];
float arrayUdc[_VOLTAG_POINT];
UINT16 pntrAveragI, pntrAveragU, dlAverag, numberAverag, dlAveragUdc;
UINT16 pntrModul;
float summIm, summId, summIq;
float averagIm, averagId, averagIq, averagUd = 0;

void calkAveregUd (void){
	UINT16 i;
	float summ = 0;
	for(i = _VOLTAG_POINT; i; i--){
		summ += arrayUdc[i-1];
	}
	summ = summ / _VOLTAG_POINT;
	averagUd = summ;
}
void initAverageMsr(void){
	UINT16 i;
//	float tmp = getIntegrVoltagDC();
	for(i=0; i<_CURRENT_POINT; i++){
		arrayIa[i] = 0;
		arrayIb[i] = 0;
		arrayUdc[i] = 540.0;
	}
	pntrAveragI = 0;
	pntrAveragU = 0;
	dlAverag = getAveragDl();
	dlAveragUdc = 1;

	pntrModul = 0;
	summIm = 0;
	summId = 0;
	summIq = 0;
	averagIm = 0;
	averagId = 0;
	averagIq = 0;
	calkAveregUd();//averagUd = 0;
}

void cntrlAverageModul(void){
	if(pntrModul < _MUDUL_POINT){
		summId += getIdVal();
		summIq += getIqVal();
		summIm += getImVal();
		pntrModul++;
	}else{
		pntrModul = 0;
		averagIm = summIm / _MUDUL_POINT;
		averagId = summId / _MUDUL_POINT;
		averagIq = summIq / _MUDUL_POINT;

		summIm = 0;
		summId = 0;
		summIq = 0;
	}
}

float getAveragIm(void){
	return averagIm;
}

float getAveragId(void){
	return averagId;
}

float getAveragIq(void){
	return averagIq;
}

UINT16 getAveragDl(void){
	float f = getInvertorFreq();
	if(f > 25.0){
		f = 5000/f;
		numberAverag = (UINT16)f;
		return 1;
	}
	if(f > 8.0){
		f = 1666.7/f;
		numberAverag = (UINT16)f;
		return 3;
	}
	if(f > 3.50){
		f = 400/f;
		numberAverag = (UINT16)f;
		return 25;
	}
	if(f > 1.0){
		f = 200/f;
		numberAverag = (UINT16)f;
		return 25;
	}
	numberAverag = 250;
	return 40;
}

void cntrlAverageMsr(void){
	dlAverag--;
	if(!dlAverag){
		dlAverag = getAveragDl();
		arrayIa[pntrAveragI] = getMomentaryCurrentA();
		arrayIb[pntrAveragI] = getMomentaryCurrentB();
		pntrAveragI++;
		pntrAveragI = pntrAveragI & (_CURRENT_POINT_MASK);
	}
	dlAveragUdc--;
	if(!dlAveragUdc){
		//dlAverag = getAveragDl();
		arrayUdc[pntrAveragU] = getMomentaryVoltagDC();
		pntrAveragU++;
//		if(testOvervoltag())
//			dlAveragUdc = 800;
//		else
			dlAveragUdc = 40;
		if(!(pntrAveragU & 0x03f)){
			calkAveregUd();
		}
		pntrAveragU = pntrAveragU & (_VOLTAG_POINT_MASK);
	}
	cntrlAverageModul();
}

float getAverageVoltagDC(void){
	return averagUd;
}

float getMaxVoltagDC(void){
	UINT16 i, cnt = 0;
	float summ = 0;
	float prev = arrayUdc[_VOLTAG_POINT-1];
//	setPin1();
	for(i = _VOLTAG_POINT-1; (i-1); i--){
		if(prev <= arrayUdc[i-1]){
			if(arrayUdc[i-1] > arrayUdc[i-2]){
				summ += arrayUdc[i-1];
				cnt++;
			}
		}

		prev = arrayUdc[i-1];
	}
	if(cnt)
		summ = summ / cnt;
	else
		summ = getAverageVoltagDC();
//	clrPin1();
	return summ;
}

float getRmsCurrent(UINT8 cur){
	float *arrayI;
	float summ;
	UINT16 number, pntr, numberAveragI;

	arrayI = (cur)?arrayIa:arrayIb;
	numberAveragI = numberAverag;

	pntr = pntrAveragI;
	number = numberAveragI-1;
	summ = (arrayI[pntr]<0)?-arrayI[pntr]:arrayI[pntr];
	while(number){
		pntr--;
		pntr = pntr & (_CURRENT_POINT-1);
		summ += (arrayI[pntr]<0)?-arrayI[pntr]:arrayI[pntr];
		--number;
	}
	summ = summ / numberAveragI;
	return summ;
}
