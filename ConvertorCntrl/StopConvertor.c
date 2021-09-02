/* ********************************************************
 * ���������� ��������� ���� ���������������
 * ********************************************************/

#include "added.h"
#include "CntrlConvertor.h"
#include "AdcCntrl/AdcCntrl.h"
#include "cntrlPWM/cntrlPwm.h"
#include "ModBus/modbus.h"
#include "statusConvertor.h"
#include "ErrorTrack/ErrorTrack.h"

UINT16 getCntrlStopConvertor(UINT16 *mode){
	getMainTask(_run_motor);
	switch(*mode){
		default:
		case _chek_stop:{
			clrConvertorRunData();
			rstPwmCntrl();
			rstCntrlPwm();
			*mode = _chek_cmd_conv;
			rstErrorInvertor();
			readSinglTrack();
			SET_DELAY(CONV_DL, c10ms);
			return _stop_convertor;
		}
		case _chek_cmd_conv:{
			if(testRun()){
				*mode = _presetup_conv;
//				presetAdc(0);
				SET_DELAY(CONV_DL, c20ms)
			}else{
				*mode = _chek_cmd_conv;
				SET_DELAY(CONV_DL, c100ms)
			}

			if(testAllDriverError()){
				rstErrorInvertor();
				initStatusConvertor();
			}

			return _stop_convertor;
		}
		case _presetup_conv:{
			initMessure();
			presetAdc(0);
			SET_DELAY(CONV_DL, c20ms);
			return _charg_convertor;
		}
	}
}
