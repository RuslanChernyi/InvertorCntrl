/*
 * �������� �������� ���������� ���������
 */

#include "added.h"
#include "stdlib.h"
#include "AdcCntrl.h"
#include "SysTimer/sysTime.h"
#include "ModBus/modbus.h"
#include "ConvertorCntrl/CntrlConvertor.h"
#include "CntrlLimit/cntrlLimit.h"

#ifdef __HIGH_POWER
#define mshtCurrentConst 64.0
#else
#define mshtCurrentConst 512.0
#endif

#define mshtVoltagConst 512.0

#define cSqrt_2 1.4142

#define mshtTemperature ((float)/*0.079*/0.05/((float)32))

#define mshtAdcConstCurrent ((4096/1.4142)*4) /*(0x01000 << 4) / sqrt(2) / 2 / 2*/
#define mshtAdcConstVoltag ((4096/1.4142)*8) /*(0x01000 << 4) / sqrt(2) / 2 */
#define c100msFM 320 /*31250 T��������� �� 100 ���*/

float mshtCurrent, mshtCurrent2, mshtVoltag;
UINT16 zerroShiftPhaseA, zerroShiftPhaseB;

INIT_DELAY(MESURE_DL)

void initMessureHD(void){
	ADD_DELAY(MESURE_DL, c1ms);
}

void cntrlAdcMessure(void){
	static UINT16 cntDlMsr=0;
	if(TEST_DELAY(MESURE_DL)){
		SET_DELAY(MESURE_DL, c1ms);
		cntrlAverageMsr();
		cntrlIntegreteADC();
		if(!cntDlMsr){
			cntDlMsr = 10;
			/*cntrlZerroShift();*/
		}else
			cntDlMsr--;
	}
}

void initMessureMsht(void){
	float tmp;
	tmp = getNominalCurrent();
	tmp = tmp / 10;
	tmp *= getMashtCurrent();
	tmp = tmp / mshtCurrentConst;
	tmp = tmp / mshtAdcConstCurrent;
	mshtCurrent = tmp;

	tmp = getNominalCurrent();
	tmp = tmp / 10;
	tmp *= getMashtCurrent2();
	tmp = tmp / mshtCurrentConst;
	tmp = tmp / mshtAdcConstCurrent;
	mshtCurrent2 = tmp;

	tmp = getNominalVoltag();
	tmp = tmp / 10;
	tmp *= getMashtVoltag();
	tmp *= cSqrt_2;
	tmp = tmp / mshtVoltagConst;
	tmp = tmp / mshtAdcConstVoltag;
	mshtVoltag = tmp;
}

void initMessure(void){
	initMessureMsht();
	/*zerroShiftPhaseA = 0;
	zerroShiftPhaseB = 0;*/
	zerroShiftPhaseA = getAveregResult(_CURRENT_A_VAL);
	zerroShiftPhaseB = getAveregResult(_CURRENT_B_VAL);
}

float getMomentaryCurrentA(void){
	float tmp = zerroShiftPhaseA;
	tmp -= getMomenteryResult(_CURRENT_A_VAL);
	tmp *= mshtCurrent;
	return tmp;
}

float getMomentaryCurrentB(void){
	float tmp = zerroShiftPhaseB;
	tmp -= getMomenteryResult(_CURRENT_B_VAL);
	tmp *= mshtCurrent2;
	return tmp;
}

float getMomentaryVoltagDC(void){
	float tmp = getMomenteryResult(_VOLTAG_DC_VAL);
	tmp *= mshtVoltag;
	return tmp;
}

float getIntegrVoltagDC(void){
	float tmp = getAveregResult(_VOLTAG_DC_VAL);
	tmp *= mshtVoltag;
	return tmp;
}

float getTemperature(void){
	float tmp = getAveregResult(_Temperature_VAL);
	tmp *= mshtTemperature;
	return tmp;
}

UINT16 presetAdc(char cmd){
	static UINT16 *val;
	UINT16 cnt;
	switch(cmd){
		default:{
			val = malloc(sizeof(UINT16) * 128);
			for(cnt=0; cnt<128; cnt++)
				val[cnt] = 0;
			cnt = 0;
			break;
		}
		case 1:{
			UINT32 summ;
			summ = 0;
			for(cnt=1; cnt<=val[0]; cnt++)
				summ += val[cnt];
			summ = summ / val[0];
			zerroShiftPhaseA = summ;

			summ = 0;
			for(cnt=1; cnt<=val[0]; cnt++)
				summ += val[cnt+64];
			summ = summ / val[0];
			zerroShiftPhaseB = summ;

			cnt = val[0];
			free(val);
			break;
		}
		case 2:{
			if(val[0] < 64){
				cnt = ++val[0];
				val[cnt] = getMomenteryResult(_CURRENT_A_VAL);
				val[cnt+64] = getMomenteryResult(_CURRENT_B_VAL);
			}
		}
	}
	return cnt;
}
/*
void cntrlZerroShift(void){
	UINT16 tmp;
	static UINT32 summA = 0, summB = 0;
	static UINT16 cntSumm = 256;

	if(cntSumm){
		cntSumm--;
		summA += getAveregResult(_CURRENT_A_VAL);
		summB += getAveregResult(_CURRENT_B_VAL);
	}
	else{
		cntSumm = 256;
		tmp = summA >> 8;
		summA = 0;

		if(tmp != zerroShiftPhaseA){
			if(tmp > zerroShiftPhaseA)
				zerroShiftPhaseA++;
			else
				zerroShiftPhaseA--;
		}

		tmp = summB >> 8;
		summB = 0;

		if(tmp != zerroShiftPhaseB){
			if(tmp > zerroShiftPhaseB)
				zerroShiftPhaseB++;
			else
				zerroShiftPhaseB--;
		}
	}

}
*/

UINT16 compareMidleValue(UINT16 val, UINT16 midle){
	if(val > midle)
		midle++;
	else{
		if(val < midle)
			midle--;
	}
	return midle;
}

void cntrlZerroShift(UINT16 zerro){
	//UINT16 tmp;
	float freq;
	static UINT32 summA = 0, summB = 0;
	static UINT16 cntSumm = 0, cntPrd = 0;

	if(getStatusRun()){
		return;
	}
	if(testOvercurrent()){
	    summA = 0;
        summB = 0;
        cntPrd = 0;
        cntSumm = 0;
	    return;
	}

	freq = getCurrentFreq();
	if(freq < 0)
		freq = -freq;

	/*fixPhaseCurrent(zerro, ferq);*/

	if(freq > 0.5){
		cntSumm++;
		summA += getMomenteryResult(_CURRENT_A_VAL);
		summB += getMomenteryResult(_CURRENT_B_VAL);
		if(zerro & 0x08000){
			cntPrd++;
			if(cntPrd > 9){
				cntPrd++;
				if(cntSumm > 1024){
					summA = summA / cntSumm;
					summB = summB / cntSumm;

//					summA += zerroShiftPhaseA;
//					summB += zerroShiftPhaseB;

					zerroShiftPhaseA = compareMidleValue((UINT16)summA, zerroShiftPhaseA);//(UINT16)(summA >> 1);
					zerroShiftPhaseB = compareMidleValue((UINT16)summB, zerroShiftPhaseB);//(UINT16)(summB >> 1);

					summA = 0;
					summB = 0;
					cntPrd = 0;
					cntSumm = 0;
				}
			}
		}
	}else{
		if(freq > 0.1){
			if(zerro & 0x0c000){
				cntPrd++;
				cntSumm++;
				summA += getAveregResult(_CURRENT_A_VAL);
				summB += getAveregResult(_CURRENT_B_VAL);
				if(cntPrd > 3){
					if(cntSumm > 1024){
						summA = summA / cntSumm;
						summB = summB / cntSumm;

//						summA += zerroShiftPhaseA;
//						summB += zerroShiftPhaseB;
//
//						zerroShiftPhaseA = (UINT16)(summA >> 1);
//						zerroShiftPhaseB = (UINT16)(summB >> 1);

						zerroShiftPhaseA = compareMidleValue((UINT16)summA, zerroShiftPhaseA);
						zerroShiftPhaseB = compareMidleValue((UINT16)summB, zerroShiftPhaseB);

						summA = 0;
						summB = 0;
						cntSumm = 0;
					}
					cntPrd = 0;
				}
			}else{
				cntPrd++;
				if(cntPrd > c100msFM){
					cntSumm++;
					summA += getAveregResult(_CURRENT_A_VAL);
					summB += getAveregResult(_CURRENT_B_VAL);
					if(cntSumm > 31250){
						summA = summA / cntSumm;
						summB = summB / cntSumm;

//						summA += zerroShiftPhaseA;
//						summB += zerroShiftPhaseB;
//
//						zerroShiftPhaseA = (UINT16)(summA >> 1);
//						zerroShiftPhaseB = (UINT16)(summB >> 1);

						zerroShiftPhaseA = compareMidleValue((UINT16)summA, zerroShiftPhaseA);
						zerroShiftPhaseB = compareMidleValue((UINT16)summB, zerroShiftPhaseB);

						summA = 0;
						summB = 0;
						cntPrd = 0;
						cntSumm = 0;
					}
				}
			}
		}
	}

/*
	if(zerro & 0x08000){
		cntPrd++;
		freq = getCurrentFreq();
		if(freq > 0.5){
			if(cntPrd > 9){
				if(cntSumm > 1024){
					summA = summA / cntSumm;
					summB = summB / cntSumm;

					summA += zerroShiftPhaseA;
					summB += zerroShiftPhaseB;

					zerroShiftPhaseA = (UINT16)(summA >> 1);
					zerroShiftPhaseB = (UINT16)(summB >> 1);

					summA = 0;
					summB = 0;
					cntPrd = 0;
					cntSumm = 0;
				}
			}
		}else{
			if(freq > 0.1){
				if(cntPrd > 1){
					if(cntSumm > 62500){
						summA = summA / cntSumm;
						summB = summB / cntSumm;

						summA += zerroShiftPhaseA;
						summB += zerroShiftPhaseB;

						zerroShiftPhaseA = (UINT16)(summA >> 1);
						zerroShiftPhaseB = (UINT16)(summB >> 1);

						summA = 0;
						summB = 0;
						cntPrd = 0;
						cntSumm = 0;
					}
				}
			}else{
				if(freq == 0){
					if(cntSumm > 62530){
						summA = summA / cntSumm;
						summB = summB / cntSumm;

						summA += zerroShiftPhaseA;
						summB += zerroShiftPhaseB;

						zerroShiftPhaseA = (UINT16)(summA >> 1);
						zerroShiftPhaseB = (UINT16)(summB >> 1);

						summA = 0;
						summB = 0;
						cntPrd = 0;
						cntSumm = 0;
					}
				}
			}
		}
	}
*/
}

float getOutToDcCurrent(void){
	float a, b, c;
	a = getMomentaryCurrentA();
	b = getMomentaryCurrentB();
	c = a + b;
	a = (a<0)?-a:a;
	b = (b<0)?-b:b;
	c = (c<0)?-c:c;
	if(a<b)
		a = b;
	if(a<c)
		a = c;
	return a;
}
