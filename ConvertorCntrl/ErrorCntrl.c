/* ******************************************************************
 * ��������� ��������� ������ ���������������
 * ******************************************************************/

#include "CntrlConvertor.h"
#include "statusConvertor.h"
#include "cntrlPWM/cntrlPwm.h"
#include "SysTimer/sysTime.h"
#include "RectifireCntrl/RectifireCntrl.h"

UINT16 getCntrlErrorConvertorState(UINT16 *mode){


#if (defined __HIGH_POWER)
	if(getTestSinchro() != (_NORMA_SINCHRO | _NORMA_PHASE) ){
		setStatusConvertor(_ALARM_INV | _BROLEN_RECT);
		return _error_store_fin;
	}
#endif


	if(testRunMode()){
		rstPwmCntrl();
		return _error_store_fin;
	}else{
		if(testStatusConvertor(_ALARM_INV | _BROLEN_PHASE))
			return _off_convertor;
		if(testStatusConvertor(_AlgoritmError)){
			if(testAlgoritmErr()){
				return _error_store_fin;
			}else{
				return _off_convertor;
			}
		}
	}

	return _off_convertor;
}
