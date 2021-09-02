/* ********************************************************
 * ���������� ������� ������������ DC
 * ********************************************************/

#include "added.h"
#include "CntrlConvertor.h"
#include "statusConvertor.h"
#include "AdcCntrl/AdcCntrl.h"
#include "ModBus/modbus.h"
#include "ReleCntrl.h"
#include "motorCntrl.h"
#include "CntrlLimit/cntrlLimit.h"
#include "cntrlPWM/cntrlPwm.h"
#include "interface/Oscilograph.h"
#include "AnalogInput/AnalogInput.h"
#include "IRcompensation.h"
#include "DC_Decelaration.h"
#include "OsCntrl/OsCntrl.h"
#include "ErrorTrack/ErrorTrack.h"
#include "RectifireCntrl/RectifireCntrl.h"

#define getUdPrecharge() ((540*3)/4)
#define _limitCharg 1.5

#if (defined __HIGH_POWER)
int ChargConer;
#endif

UINT16 getCntrlChargConvertor(UINT16 *mode){
	static float oldVoltag;
	if(testErrorInv()){
		SET_DELAY(CONV_DL, c20ms);
		return _error_convertor;
	}

#if (defined __HIGH_POWER)
//	if(getTestSinchro() != (_NORMA_SINCHRO | _NORMA_PHASE) )
//		return _error_convertor;
#endif

	if(!testRun()){
		*mode = 0;
		return _stop_convertor;
	}

	switch(*mode){
		default:
		case _start_charg:{
			oldVoltag = 0;
			presetAdc(2);
#if (defined __HIGH_POWER)
			ChargConer = _MaxConer;
#endif
			onChargeRele();
			*mode = _check_charge_conv;
			initOsPid(0);
			SET_DELAY(CONV_DL, c10ms);
			return _charg_convertor;
		}
		case _check_charge_conv:{

#if (defined __HIGH_POWER)
//			if(ChargConer > 0)
//				ChargConer-=(_MaxConer / 64);
//			else
//				ChargConer = 0;
//			setConer(ChargConer);
#endif

			presetAdc(2);
			oldVoltag = oldVoltag - getIntegrVoltagDC();//getVoltagDC();
			if(oldVoltag<0)
					oldVoltag= -oldVoltag;
			if((/*getVoltagDC()*/getIntegrVoltagDC() > getUdPrecharge()) &&
			   (oldVoltag < _limitCharg)){
				*mode = _on_main_pwr;
				SET_DELAY(CONV_DL, c10ms);
			}else{
				oldVoltag = getIntegrVoltagDC();//getVoltagDC();
				SET_DELAY(CONV_DL, c20ms);
			}
			return _charg_convertor;
		}
		case _on_main_pwr:{
			UINT16 tmp;

			tmp = presetAdc(2);
			if(tmp < 64){
				SET_DELAY(CONV_DL, c1ms);
				return _charg_convertor;
			}

#if !(defined __USE_EXTERN_SIFU)
			offChargeRele();
#endif
#ifndef __BLOCK_PWR
#if (!(defined __HIGH_POWER))
			onPwrRele();
#endif
#if (defined __HIGH_POWER)
			fullOpenRect(); //setConer(0);
#endif
#endif
#if !(defined __USE_EXTERN_SIFU)
			SET_DELAY(CONV_DL, c100ms);
#else
			SET_DELAY(CONV_DL, c2s);
#endif
			*mode = _on_invertor;

			InitOSC(0);
			initOvervoltCntrl();
			initOverloadCntrl();
			initIbyTLimit();
			initAverageMsr();
			presetAccelDecel();
			presetFreqToVoltag();
			initAnalogTaskSignal();
			initAnalogOsSignal();
			initOverloadCntrl();
			initIrCompensation();
			initDcDecelation();
			initTrackError();
			initIqStab();
			initEconomParam();
			return _charg_convertor;
		}
		case _on_invertor:{
			enableCntrlPwm();
			presetAdc(1);
			if(testRunMode()){
				return _run_convertor;
			}
			if(testJobMode()){
				return _job_convertor;
			}
			return _off_convertor;
		}
	}
}
