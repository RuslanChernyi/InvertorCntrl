/* ********************************************************
 * ���������� ������� ���������
 * ********************************************************/

#include "CntrlConvertor.h"
#include "cntrlPWM/cntrlPwm.h"
#include "ModBus/modbus.h"
#include "statusConvertor.h"
#include "CntrlLimit/cntrlLimit.h"
#include "blink/blink.h"
#include "ErrorTrack/ErrorTrack.h"

float taskV, taskF;
float currentF, currentV;
UINT16 delayCnt;
UINT8 FlagPwmSet;

void clrConvertorRunData(void){
	currentF = 0;
	currentV = 0;
	taskF = 0;
	taskV = 0;
	FlagPwmSet = 0;
}

void setFlagPwmSet(void){
	FlagPwmSet = 1;
}

UINT8 getFlagPwmSet(void){
	return FlagPwmSet;
}

//float getTaskFout(void);

float getCurrentFreq(void){
	return currentF;
}

float getCurrentVinv(void){
	return currentV;
}

float getInvertorFreq(void){
	return taskF;
}

float getInvertorVinv(void){
	return taskV;
}

UINT16 getCntrlRunConvertor(UINT16 *mode){

	if(testErrorInv())
		return _error_convertor;

	switch(*mode){
		default:
		case __pre_run_stage:
		{
			SET_DELAY(CONV_DL, c10ms);
			delayCnt = 0;
			initOverloadCntrl();
			rstCntrlPwm();
			if(testSoftStart()){
				*mode = _soft_start_convertor;	//������� �� ����
				break;
			}else{
				*mode = _cold_start_convertor;
			}
		}
		case _soft_start_convertor:
		{
	#ifdef __CORC_SOFT_SATRT
			currentF = 0;
			currentV = 0;
			taskF = 0;
			taskV = 0;
			*mode = _run_stage_convertor;
			break;
	#endif
		}
		case _cold_start_convertor:
		{
			taskF = 0;
			taskV = 0;
			currentF = 0;
			currentV = 0;
			if(TEST_DELAY(CONV_DL)){
				SET_DELAY(CONV_DL, c1ms);
				*mode = _run_stage_convertor;
			}else{
				*mode = _cold_start_convertor;
			}
			break;
		}
		case _run_stage_convertor:
		{
			if(FlagPwmSet){
//				setPin2();
				FlagPwmSet = 0;
				cntrlErrorTrack();
		// 04.07.17
				cntrlOvervoltag();
				cntrlOverload();
				cntrlMotorCntrlPause(&taskF, &taskV, &currentF, &currentV);
		// 04.07.17
//				*mode = controlPwm(&taskF, &taskV, &currentF, &currentV);
//				clrPin2();
//				clrPin1();
			}
			if(TEST_DELAY(CONV_DL)){
				SET_DELAY(CONV_DL, c1ms);
				cntrlOvervoltag();
				cntrlOverload();
#ifdef _BROKEN_PHASE_CNTRL
				cntrlBrokePhase();
#endif
				*mode = controlPwm(&taskF, &taskV, &currentF, &currentV);
				cntrlIbyT();
			}
			break;
		}
		case _stop_stage_convertor:
		{
			UINT32 tmp;
			if(TEST_DELAY(CONV_DL)){
				clrConvertorRunData();
				tmp = getRegister(_STOP_PAUSE);
				if(tmp){
					rstPwmCntrl();
					rstCntrlPwm();
					tmp *= c100ms;
					SET_DELAY(CONV_DL, tmp);
					return _postrun_pause_stop;
				}
				SET_DELAY(CONV_DL, c1ms);
				return _stop_convertor;
			}
			break;
		}
	}
	return _run_convertor;
}
