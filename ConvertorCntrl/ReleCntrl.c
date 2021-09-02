/* ******************************************************************
 * ������ � ���� � ���������� �������
 * ******************************************************************/

#include "added.h"
#include "DSP2833x_Device.h"

void initDiscrOut(void){

    EALLOW;

    // Enable an GPIO output on GPIOx, set it high
    GpioCtrlRegs.GPBPUD.bit.GPIO46 = 0;   // Enable pullup on GPIOx
    GpioDataRegs.GPBCLEAR.bit.GPIO46 = 1;   // Load output latch
    GpioCtrlRegs.GPBMUX1.bit.GPIO46 = 0;  // GPIOx = GPIOx
#ifndef __BLOCK_PWR
    GpioCtrlRegs.GPBDIR.bit.GPIO46 = 1;   // GPIOx = output
#else
    GpioCtrlRegs.GPBDIR.bit.GPIO46 = 0;   // GPIOx = output
#endif

    // Enable an GPIO output on GPIOx, set it high
    GpioCtrlRegs.GPBPUD.bit.GPIO47 = 0;   // Enable pullup on GPIOx
#if !(defined __USE_EXTERN_SIFU)
    GpioDataRegs.GPBCLEAR.bit.GPIO47 = 1;   // Load output latch
#else
    GpioDataRegs.GPBSET.bit.GPIO47 = 1;   // Load output latch
#endif
    GpioCtrlRegs.GPBMUX1.bit.GPIO47 = 0;  // GPIOx = GPIOx
    GpioCtrlRegs.GPBDIR.bit.GPIO47 = 1;   // GPIOx = output

    // Enable an GPIO output on GPIOx, set it high
    GpioCtrlRegs.GPCPUD.bit.GPIO64 = 1;   // disable pullup on GPIOx
    GpioDataRegs.GPCCLEAR.bit.GPIO64 = 1;   // Load output latch
    GpioCtrlRegs.GPCMUX1.bit.GPIO64 = 0;  // GPIOx = GPIOx
    GpioCtrlRegs.GPCDIR.bit.GPIO64 = 1;   // GPIOx = output

    // Enable an GPIO output on GPIOx, set it high
       GpioCtrlRegs.GPBPUD.bit.GPIO60 = 1;   // disable pullup on GPIOx
       GpioDataRegs.GPBCLEAR.bit.GPIO60 = 1;   // Load output latch
       GpioCtrlRegs.GPBMUX2.bit.GPIO60 = 0;  // GPIOx = GPIOx
       GpioCtrlRegs.GPBDIR.bit.GPIO60 = 1;   // GPIOx = output

    // Enable an GPIO output on GPIOx, set it high
    GpioCtrlRegs.GPCPUD.bit.GPIO70 = 1;   // disable pullup on GPIOx
    GpioDataRegs.GPCCLEAR.bit.GPIO70 = 1;   // Load output latch
    GpioCtrlRegs.GPCMUX1.bit.GPIO70 = 0;  // GPIOx = GPIOx
    GpioCtrlRegs.GPCDIR.bit.GPIO70 = 1;   // GPIOx = output

    // Enable an GPIO output on GPIOx, set it high
    GpioCtrlRegs.GPCPUD.bit.GPIO71 = 1;   // disable pullup on GPIOx
    GpioDataRegs.GPCCLEAR.bit.GPIO71 = 1;   // Load output latch
    GpioCtrlRegs.GPCMUX1.bit.GPIO71 = 0;  // GPIOx = GPIOx
    GpioCtrlRegs.GPCDIR.bit.GPIO71 = 1;   // GPIOx = output

	/*output buffer enable control pin*/
    GpioCtrlRegs.GPBPUD.bit.GPIO45 = 0;   // Enable pullup on GPIOx
	GpioDataRegs.GPBCLEAR.bit.GPIO45 = 1;   // Load output latch
	GpioCtrlRegs.GPBMUX1.bit.GPIO45 = 0;  // GPIOx = GPIOx
	GpioCtrlRegs.GPBDIR.bit.GPIO45 = 1;   // GPIOx = output


    EDIS;

}

void onBufferEn(void){
	GpioDataRegs.GPBCLEAR.bit.GPIO45 = 1;   // Load output latch
}

void offBufferEn(void){
	GpioDataRegs.GPBSET.bit.GPIO45 = 1;   // Load output latch
}

#if !(defined __USE_EXTERN_SIFU)
void onChargeRele(void){
	GpioDataRegs.GPBSET.bit.GPIO47 = 1;   // Load output latch
}

void offChargeRele(void){
	GpioDataRegs.GPBCLEAR.bit.GPIO47 = 1;   // Load output latch
}
#else
void onChargeRele(void){
	GpioDataRegs.GPBCLEAR.bit.GPIO47 = 1;   // Load output latch
}

void offChargeRele(void){
	GpioDataRegs.GPBSET.bit.GPIO47 = 1;   // Load output latch
}
#endif

void onPwrRele(void){
	GpioDataRegs.GPBSET.bit.GPIO46 = 1;   // Load output latch
}

void offPwrRele(void){
	GpioDataRegs.GPBCLEAR.bit.GPIO46 = 1;   // Load output latch
}

void onDistRele(void){
	GpioDataRegs.GPCSET.bit.GPIO71 = 1;   // Load output latch
    GpioDataRegs.GPBSET.bit.GPIO60 = 1;   // Load output latch
}

void offDistRele(void){
	GpioDataRegs.GPCCLEAR.bit.GPIO71 = 1;   // Load output latch
	GpioDataRegs.GPBCLEAR.bit.GPIO60 = 1;   // Load output latch
}

//void onDistOK2(void){
//	GpioDataRegs.GPCSET.bit.GPIO64 = 1;   // Load output latch
//}
//
//void offDistOK2(void){
//	GpioDataRegs.GPCCLEAR.bit.GPIO64 = 1;   // Load output latch
//}

void onDistOK1(void){
	GpioDataRegs.GPCSET.bit.GPIO70 = 1;   // Load output latch
    GpioDataRegs.GPCSET.bit.GPIO64= 1;   // Load output latch
}

void offDistOK1(void){
	GpioDataRegs.GPCCLEAR.bit.GPIO70 = 1;   // Load output latch
	GpioDataRegs.GPCCLEAR.bit.GPIO64 = 1;   // Load output latch
}
