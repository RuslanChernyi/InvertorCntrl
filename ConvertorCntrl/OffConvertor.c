/* ********************************************************
 * ����� - "�������� ��������"
 * ********************************************************/

#include "CntrlConvertor.h"
#include "ModBus/modbus.h"
#include "AdcCntrl/AdcCntrl.h"
#include "cntrlPWM/cntrlPwm.h"
#include "statusConvertor.h"
#include "RectifireCntrl/RectifireCntrl.h"

UINT16 getCntrlOffConvertor(UINT16 *mode){
	getMainTask(_run_motor);
	switch(*mode){
		default:
		case _chek_off:{
			clrConvertorRunData();
			//setZerroShiftAdc();
			rstPwmCntrl();

#if (defined __HIGH_POWER)
			if(getTestSinchro() != (_NORMA_SINCHRO | _NORMA_PHASE) )
				return _error_convertor;
#endif


			*mode = _chek_redy_conv;
			initStatusConvertor();
			SET_DELAY(CONV_DL, c100ms);
			return _off_convertor;
		}
		case _chek_redy_conv:{
			rstErrorInvertor();
			*mode = 0;
			SET_DELAY(CONV_DL, c100ms)
			return _stop_convertor;
		}
	}
}
