/*
 * ���������� ����������
 */
 
#include "CntrlConvertor.h"
#include "cntrlPWM/cntrlPwm.h"
#include "SysTimer/sysTime.h"
#include "blink/blink.h"
#include "interface/TaskCntrl.h"
#include "interface/DiscrOutPut.h"
#include "CntrlLimit/cntrlLimit.h"
#include "statusConvertor.h"
#include "ReleCntrl.h"
#include "ErrorTrack/ErrorTrack.h"
#include "RectifireCntrl/RectifireCntrl.h"
#include "ModBus/Registers.h"
#include "AnalogInput/AnalogInput.h"
#include "OsCntrl/OsCntrl.h"
#include "Aqua/aqua.h"

UINT16 modeConvertor, subMode;
unsigned int tmp_stat=0;
unsigned int costyl_control(void);

extern unsigned int looseBeltCounter;


INIT_DELAY(CONV_DL)

INIT_DELAY(Mirror_DL)

UINT16 getModeConvertor(void){
	return modeConvertor;
}

UINT8 testRunState(void){
	if(modeConvertor == _run_convertor)
		return 1;
	if(modeConvertor == _job_convertor)
		return 2;
	return 0;
}

void protectCntrl(void){
    float tmp, tmp1;

	if(testLimitVoltag()){
		setOvervoltagAlrm();
	}

	if((modeConvertor == _run_convertor) ||
		modeConvertor == _job_convertor){
		if(testLimitCurrent()){
			setOverCurrent();
		}
	}

	if(testLooseBelt())
	{
	    if(getPidOs() > (0.3 * getRegister(_F_MAX_OS)))
	    {
	        if (getAnalogOs() < (0.2*getNormaOs()))
	                {
	                    // looseBeltCounter is in milliseconds, get Register is in seconds
	                           if(looseBeltCounter > getRegister(_LOOSE_TIME)*100) //17.08.2021 Ruslan Chernyi
	                                    {
	                                        setLooseBelt();
	                                        modeConvertor=_stop_convertor;
	                                        looseBeltCounter = 0;//17.08.2021 Ruslan Chernyi
	                                        return;
	                                    }
	                            //looseBeltCounter initialized in SysTime.c
	                    }
	    }
	}
}

void cntrlConvertor(void){
	protectCntrl();

	cntrlStartStopDir();

	if(TEST_DELAY(Mirror_DL)){
		SET_DELAY(Mirror_DL, c50ms)
		refreshRegisterMbus();
		cntrlDiscrPinOut();
	}
if(costyl_control())
{
    if(check_relay_ready()==3)
        {
           setStatusConvertor(_RUN_CONVERTOR);
           tmp_stat=1;
        }
        else
        {
        modeConvertor=_stop_convertor;
        tmp_stat=0;
        }
}

//	if((getCurrentConer() - c120dgr)&0x08000){
//		onDistOK1();
//	}else{
//		offDistOK1();
//	}

	switch(modeConvertor){
		default:
		case _off_convertor:{
			if(WHILE_DELAY(CONV_DL)){
				return;
			}
			offChargeRele();
			initTaskMain();
			offRectifire();
			modeConvertor = getCntrlOffConvertor(&subMode);
			setLedMode(_ERROR_LED, _off_led);
			setLedMode(_WORCK_LED, _off_led);
			break;
		}
		case _stop_convertor:{
			if(WHILE_DELAY(CONV_DL)){
				return;
			}
			initTaskMain();
#ifdef _BROKEN_PHASE_CNTRL
			initBrokenPhaseCntrl();
#endif
			modeConvertor = getCntrlStopConvertor(&subMode);
			setLedMode(_WORCK_LED, _splash_led);
			setLedMode(_ERROR_LED, _off_led);
			setLedMode(_ADD_LED, _off_led);
			break;
		}
		case _charg_convertor:{
			if(WHILE_DELAY(CONV_DL)){
				return;
			}
			modeConvertor = getCntrlChargConvertor(&subMode);
			setLedMode(_WORCK_LED, _blink_led);
			break;
		}
		case _run_convertor:{
			modeConvertor = getCntrlRunConvertor(&subMode);
			setLedMode(_WORCK_LED, _fast_blink_led);
			break;
		}
		case _job_convertor:{
			modeConvertor = getCntrlStopConvertor(&subMode);
			break;
		}
		case _error_convertor:{
		    cntrlErrorTrack();
			storySinglTrack();
#if (defined __HIGH_POWER)
			fullCloseRect();
#endif

#if (defined __USE_EXTERN_SIFU)
			offChargeRele();
#endif
			modeConvertor = _error_store_fin;
			break;
		}
		case _error_store_fin:{
#if (defined __HIGH_POWER)
			fullCloseRect();
#endif

#if (defined __USE_EXTERN_SIFU)
			offChargeRele();
#endif
			if(WHILE_DELAY(CONV_DL)){
				return;
			}
			modeConvertor = getCntrlErrorConvertorState(&subMode);
			setLedMode(_ERROR_LED, _fast_blink_led);
			SET_DELAY(CONV_DL, c10ms)
			break;
		}
		case _postrun_pause_stop:{
			if(WHILE_DELAY(CONV_DL)){
				setLedMode(_WORCK_LED, _blink_led);
				setLedMode(_ADD_LED, _blink_led);
				return;
			}
			modeConvertor = _stop_convertor;
		}
	}
}

void initCntrlConvertor(void){
	modeConvertor = 0;
	subMode = 0;
	setLedMode(_ERROR_LED, _off_led);
	setLedMode(_WORCK_LED, _off_led);
	setLedMode(_ADD_LED, _off_led);

	ADD_DELAY(CONV_DL, 0);
	ADD_DELAY(Mirror_DL, 0);
	initOverloadCntrl();
	initOvervoltCntrl();
	rstCntrlPwm();
}
