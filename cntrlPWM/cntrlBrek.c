/*
 * cntrlBrek.c
 *
 *  Created on: 16 ���. 2015
 *      Author: Tester
 *
 *  ���������� ��������� ������������
 */

#include "added.h"
#include "ModBus/modbus.h"
#include "PWM.h"
#include "AdcCntrl/AdcCntrl.h"

float levelBreak;

void initBreak(void){
	EALLOW;

	// Enable an GPIO output on GPIOx, set it high
	GpioCtrlRegs.GPAPUD.bit.GPIO24 = 0;   // Enable pullup on GPIOx
#ifdef __HI_ACKT
	GpioDataRegs.GPACLEAR.bit.GPIO24 = 1;   // Load output latch
#else
	GpioDataRegs.GPASET.bit.GPIO24 = 1;   // Load output latch
#endif
	GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 0;  // GPIOx = GPIOx
	GpioCtrlRegs.GPADIR.bit.GPIO24 = 1;   // GPIOx = output

	EDIS;

	levelBreak = getRegister(_LEVEL_BREAK_LIM);
}

void rstBreak(void){
#ifdef __HI_ACKT
	GpioDataRegs.GPACLEAR.bit.GPIO24 = 1;   // Load output latch
#else
	GpioDataRegs.GPASET.bit.GPIO24 = 1;   // Load output latch
#endif
}

void cntrlBreak(void){
	if(getVoltagDC() > levelBreak){
		#ifdef __HI_ACKT
			GpioDataRegs.GPASET.bit.GPIO24 = 1;   // Load output latch
		#else
			GpioDataRegs.GPACLEAR.bit.GPIO24 = 1;   // Load output latch
		#endif
	}else{
		#ifdef __HI_ACKT
			GpioDataRegs.GPACLEAR.bit.GPIO24 = 1;   // Load output latch
		#else
			GpioDataRegs.GPASET.bit.GPIO24 = 1;   // Load output latch
		#endif
	}
}
