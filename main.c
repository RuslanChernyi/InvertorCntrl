/*
 * main.c
 * 12.07.17 доработка регулятора токоограничения
 */

#include "DSP2833x_Device.h"

#include "added.h"
#include "core/initSys.h"
#include "SysTimer/sysTime.h"
#include "blink/blink.h"
#include "UART/cntrlUART.h"
#include "ModBus/modbus.h"
#include "ConvertorCntrl/statusConvertor.h"
#include "cntrlI2C/I2Ccontrol.h"
#include "cntrlPWM/cntrlPwm.h"
#include "ConvertorCntrl/ReleCntrl.h"
#include "ConvertorCntrl/CntrlConvertor.h"
#include "DiscretPin/DiscretPin.h"
#include "AdcCntrl/AdcCntrl.h"
#include "interface/Oscilograph.h"
#include "AnalogInput/AnalogInput.h"
#include "OsCntrl/OsCntrl.h"
#include "OsCntrl/KaskadOs.h"
#include "CntrlLimit/cntrlLimit.h"
#include "UART/flow485cntrl.h"
#include "RtcCntrl/RtcCntrl.h"
#include "RectifireCntrl/RectifireCntrl.h"
#include "interface/DiscrOutPut.h"
#include "ConvertorCntrl/IRcompensation.h"
#include "ConvertorCntrl/DC_Decelaration.h"
#include "ConvertorCntrl/motorCntrl.h"
#include "UART/Flow232Cntrl.h"
#include "ErrorTrack/ErrorTrack.h"
#include "AdcCntrl/MtoTimeCntrl.h"

INIT_DELAY(SLOW_DATA_DL)
unsigned int costyl_control(void);
UINT16 cntSlDtDl = 0;
UINT8 longDl = 0;
#define presc_button 5000
unsigned int inc_ctr=0;
unsigned int dec_ctr=0;

void cntrlSlowData(void){
	if(TEST_DELAY(SLOW_DATA_DL)){
		SET_DELAY(SLOW_DATA_DL, c20ms);
		cntSlDtDl++;
		if((cntSlDtDl & 0x3) == 0x03)
			cntrlAnalogTask();

		if(cntSlDtDl & 0x1)
			cntrlAnalogOS();


		if((cntSlDtDl & 0x7) == 0x07)
			mirrorAnalogInp();

		cntrlCooler();

		if(testRunState()){
			if(testIbyT()){
			    setIbyT_Alrm();
			}

#ifndef __CORCK_ADC_UD
			if(testUnderVoltag()){
				setUnderVoltagAlrm();
			}
#endif
		}

        //обновление коэффициентов и уставок
        if(!longDl){
            longDl = 10;
            initOverVoltKoef();
            initAnalogTaskSignal();
        }else{
            longDl--;
            switch(longDl){
                case 2:{
                    initKoefOverload();
                    initMessureMsht();
                    initPIDmplr();
                    reinitKaskadOs();
                    break;
                }
                case 5:{
                    initIbyTParam();
                    if(testRunMode()){
                        initDcDecelation();
                        initIrCompConst();
                    }
                    break;
                }
                case 7:{
                    initAnalogOsSignal();
                    initIqStab();
                    initEconomParam();
                    break;
                }
            }
        }
		/*}else{

		}*/
	}
}

int main(void) {

// Disable CPU interrupts
	DINT;

	InitSysCtrl();
	DisableDog();

	InitPieCtrl();

	// Disable CPU interrupts and clear all CPU interrupt flags:
	IER = 0x0000;
	IFR = 0x0000;

	// Section secureRamFuncs contains user defined code that runs from CSM secured RAM
	memcpy(&secureRamFuncs_runstart, &secureRamFuncs_loadstart, (Uint32)&secureRamFuncs_loadsize);

	//memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (Uint32)&RamfuncsLoadSize);

	InitFlash();
	initStagePin();
	initSysTime();
	initUART();
	initUARTAddRS();
	initUART_485();
	initPWM();
	initADC();
	/// Enable global Interrupts and higher priority real-time debug events:
	EINT;   // Enable Global interrupt INTM
	ERTM;   // Enable Global realtime interrupt DBGM

//	{
//		long tmp;
//		tmp = (long)(8000000);
//		while(tmp)
//			tmp--;
//	}


#if (defined __HIGH_POWER)

	initRectifire();

#endif

	initDiscrOut();
	initDiscrInput();
	initLed();
	InitI2C();
	initMbusFlow();
	initHoldingReg();
	initStatusConvertor();
	initCntrlConvertor();

	presetPwmTime(/*3125, */5000);
	initMessureHD();
	onBufferEn();
	initCoolerCntrl();
	initMessure();
	InitOSC(1);
	ADD_DELAY(SLOW_DATA_DL, 0);
	cntSlDtDl = 0;
	initAverageMsr();


	initOsPid(1); //05.08.21 ��������

	setPwmPriod(getPeriodPWM());
	presetPwmTime(/*3125, */5000);
	initFlow485Cntrl();
	initModbusAndShell();
	initModbusAndShell232();
	initRtClock();
	initAllAnalogInput();
	initDiscrPinOut();
	initMotoresource();
//	readSinglTrack();

#if (defined __HIGH_POWER)

//	initSinchroTest();
	SET_DELAY(SLOW_DATA_DL, c200ms);
//	while(WHILE_DELAY(SLOW_DATA_DL)){
//		waitSinchroPulse();
//	}
	testSinchroPulse();

//	initRectifire();
	setConer(_MaxConer);
#endif

#if (defined __MIDDLE_POWER)
	initSinchroTest();
#endif
	 SetRegister(_OS_PID_TASK, 0);
	 SetRegister(_F_MAX_OS, 500);
	while(1)		// Dummy loop.  Wait for an interrupt.
	{
	    /*
	    if(getStatusConvertor(_RUN_CONVERTOR))
	{
	        SetRegister(_RUN_PULT, 0x0FF);
	}
	    if(!getStatusConvertor(_RUN_CONVERTOR))
	        {
	                SetRegister(_RUN_PULT, 0x00);
	        }
*/
	    if(costyl_control())
{
	    incButton(presc_button,3200);
	    decButton(presc_button,3200);
}
		cntrlLedMode();
		I2C_Control();
		cntrlConvertor();
		cntrlAdcMessure();
		transmiteOscMsg();
		cntrlSlowData();
		cntrlOsPid();
		cntrFlow232();//UartFlowControl();
		cntrFlow485();
		cntrlMotoResource();
	}

	//return 0;
}
