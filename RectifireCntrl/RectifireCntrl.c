/*
 * RectifireCntrl.c
 *
 *  Created on: 19 дек. 2016 г.
 *      Author: test
 */

#include "added.h"
#include "DSP2833x_Device.h"
#include "cntrlPWM/DSP2833x_EPwm_defines.h"
#include "RectifireCntrl.h"

#define _MinTestPulse 7

#define getFixPeriodSifu() 9000
#define getDeadTimeSifu() 255

//#define _periodSifu	((UINT32)200000)
#define _periodSifu	((UINT32)2000000)
#define _halfPeriod	(_periodSifu/2)
#define _60dgrPeriod	(_periodSifu/6)
#define _30dgrPeriod	(_periodSifu/12)
#define _minConer	(100)

#define clrSifuPulse(pin)	\
	EALLOW;					\
	GpioDataRegs.GPACLEAR.bit.pin = 1;\
	GpioCtrlRegs.GPAMUX1.bit.pin = 0;\
	EDIS;					\

#define setSifuPulse(pin)	\
	EALLOW;					\
	GpioCtrlRegs.GPAMUX1.bit.pin = 1;\
	EDIS;					\

#define _SufyAP	0x04
#define _SufyAM	0x020
#define _SufyBP	0x010
#define _SufyBM	0x02
#define _SufyCP	0x01
#define _SufyCM	0x08

#define _ApinP	GPIO6
#define _ApinM	GPIO7
#define _BpinP	GPIO8
#define _BpinM	GPIO9
#define _CpinP	GPIO10
#define _CpinM	GPIO11

#define setSifuPulse(pin)	\
	EALLOW;					\
	GpioCtrlRegs.GPAMUX1.bit.pin = 1;\
	EDIS;					\

char filtrCntrl, filtrState;
char pulseState, pulseCntrl;
char SinchroState;
char stateSinchro;
UINT32 ConerTime, shiftTime;

void resetFiltrTimer(void){
	ECap6Regs.ECCTL2.bit.TSCTRSTOP = 0;
	ECap6Regs.TSCTR = 0;
	ECap6Regs.ECCTL2.bit.TSCTRSTOP = 1;
}

void resetPulseTimer(void){
	ECap2Regs.ECCTL2.bit.TSCTRSTOP = 0;
	ECap2Regs.TSCTR = 0;
	ECap2Regs.ECCTL2.bit.TSCTRSTOP = 1;
}

interrupt void sifuAlfa(void){
//	if(!SinchroState){
//		GpioDataRegs.GPASET.bit.GPIO7 = 1;
//		ECap1Regs.ECCTL2.bit.TSCTRSTOP = 0;
//		ECap1Regs.CAP2 = ConerTime;
//		ECap1Regs.ECCLR.bit.CTR_EQ_CMP = 1;
//		ECap1Regs.ECCTL2.bit.TSCTRSTOP = 1;
//		SinchroState = 1;
//		filtrCntrl = 0;
//		cntrlFiitrSifu();
//		resetFiltrTimer();
////		ECap1Regs.ECEINT.bit.CTR_EQ_CMP = 0;
//	}else{
//		GpioDataRegs.GPACLEAR.bit.GPIO7 = 1;
////		ECap1Regs.ECCTL2.bit.TSCTRSTOP = 0;
//		ECap1Regs.ECCLR.bit.CTR_EQ_CMP = 1;
//		pulseCntrl = 0;
//		cntrlPulseSifu();
//		resetPulseTimer();
//
////		ECap1Regs.ECCLR.bit.CTR_EQ_PRD = 1;
////		ECap1Regs.ECEINT.bit.CTR_EQ_CMP = 0;
////		ECap1Regs.ECEINT.bit.CTR_EQ_PRD = 0;
//	}
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;
}

//	static long time;
void startAlfaTimer(void){
	ECap1Regs.ECCTL2.bit.TSCTRSTOP = 0;
//	time = ECap1Regs.TSCTR;
	ECap1Regs.TSCTR = 0;
	ECap1Regs.CAP1 = _periodSifu*2;
//	ECap1Regs.CAP3 = ConerTime;
	ECap1Regs.CAP2 = shiftTime;
//	ECap1Regs.CAP4 = shiftTime;
	ECap1Regs.ECCLR.all = 0xff;
//	ECap1Regs.ECEINT.bit.CTR_EQ_CMP = 1;
//	ECap1Regs.ECEINT.bit.CTR_EQ_PRD = 1;
	ECap1Regs.ECCTL2.bit.TSCTRSTOP = 1;
	GpioDataRegs.GPACLEAR.bit.GPIO7 = 1;
}

interrupt void sifuSinchro(void) {
//	if(GpioDataRegs.GPBDAT.bit.GPIO44){
//		startAlfaTimer();
//		SinchroState = 0;
//		GpioDataRegs.GPASET.bit.GPIO6 = 1;
//	}else{
//		GpioDataRegs.GPACLEAR.bit.GPIO6 = 1;
//	}

	PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;
}

const char sifuStateArray[] = {
	0b000000111,
	0b000001110,
	0b000011100,
	0b000111000,
	0b000110001,
	0b000100011,
	0x0,
	0x0
};

void cntrlFiitrSifu(void){
	if(filtrCntrl > 5){
		cntrlIO_PIN_Sifu(0);
		filtrCntrl = 6;
	}else{
		filtrState = sifuStateArray[filtrCntrl];
		++filtrCntrl;

		if(!(filtrState & _SufyAP)){
			clrSifuPulse(_ApinP);
		}
		if(!(filtrState & _SufyAM)){
			clrSifuPulse(_ApinM);
		}
		if(!(filtrState & _SufyBP)){
			clrSifuPulse(_BpinP);
		}
		if(!(filtrState & _SufyBM)){
			clrSifuPulse(_BpinM);
		}
		if(!(filtrState & _SufyCP)){
			clrSifuPulse(_CpinP);
		}
		if(!(filtrState & _SufyCM)){
			clrSifuPulse(_CpinM);
		}
	}
}

interrupt void sifuFiltr(void) {
//	cntrlFiitrSifu();

	ECap6Regs.ECCLR.all = 0x0ff;
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;
}

void cntrlPulseSifu(void){
	if(pulseCntrl > 5){
		cntrlIO_PIN_Sifu(0);
		pulseCntrl = 6;
	}else{
		pulseState = sifuStateArray[pulseCntrl];
		pulseState &= filtrState;
		++pulseCntrl;

		if(!(pulseState & _SufyAP)){
			setSifuPulse(_ApinP);
		}else{
			clrSifuPulse(_ApinP);
		}
		if(!(pulseState & _SufyAM)){
			setSifuPulse(_ApinM);
		}else{
			clrSifuPulse(_ApinM);
		}
		if(!(pulseState & _SufyBP)){
			setSifuPulse(_BpinP);
		}else{
			clrSifuPulse(_BpinP);
		}
		if(!(pulseState & _SufyBM)){
			setSifuPulse(_BpinM);
		}else{
			clrSifuPulse(_BpinM);
		}
		if(!(pulseState & _SufyCP)){
			setSifuPulse(_CpinP);
		}else{
			clrSifuPulse(_CpinP);
		}
		if(!(pulseState & _SufyCM)){
			setSifuPulse(_CpinM);
		}else{
			clrSifuPulse(_CpinM);
		}
	}
}

interrupt void sifuPulse(void) {
//	cntrlPulseSifu();

	ECap2Regs.ECCLR.all = 0x0ff;
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;
}

void cntrlIO_PIN_Sifu(char state){

    GpioDataRegs.GPACLEAR.all = ((0x01<<6)|(0x01<<7)|(0x01<<8)|
    		                     (0x01<<9)|(0x01<<10)|(0x01<<11));

	if(state){
		EALLOW;

		GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 1;   // Configure GPIO5 as EPWM1B
		GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 1;   // Configure GPIO4 as EPWM2A
		GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 1;   // Configure GPIO5 as EPWM2B
		GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 1;   // Configure GPIO4 as EPWM3A
		GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 1;   // Configure GPIO5 as EPWM3B
		GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 1;   // Configure GPIO4 as EPWM1A

		EDIS;
	}else{
		GpioDataRegs.GPACLEAR.all = (((UINT32)1)<<6) | (((UINT32)1)<<7) |
									(((UINT32)1)<<8) | (((UINT32)1)<<9) |
									(((UINT32)1)<<10) | (((UINT32)1)<<11);
		EALLOW;
		GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 0;   // Configure GPIO5 as IOP_IN
		GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 0;   // Configure GPIO4 as IOP_IN
		GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 0;   // Configure GPIO5 as IOP_IN
		GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 0;   // Configure GPIO4 as IOP_IN
		GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 0;   // Configure GPIO5 as IOP_IN
		GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 0;   // Configure GPIO4 as IOP_IN
		EDIS;
	}
}

void initRectifire(void) {

	filtrCntrl = 6;
	filtrState = 0;
	pulseState = 0;
	pulseCntrl = 6;
	shiftTime = _30dgrPeriod;
	ConerTime = _halfPeriod;

	EALLOW;

	PieVectTable.ECAP1_INT = &sifuAlfa;
	PieVectTable.ECAP2_INT = &sifuPulse;
	PieVectTable.ECAP6_INT = &sifuFiltr;
	PieVectTable.XINT3 = &sifuSinchro;

	// Enable an GPIO output on GPIOx, set it high
	GpioCtrlRegs.GPBPUD.bit.GPIO44 = 0;   // Enable pullup on GPIOx
	GpioDataRegs.GPBSET.bit.GPIO44 = 1;   // Load output latch
	GpioCtrlRegs.GPBMUX1.bit.GPIO44 = 0;  // GPIOx = GPIOx
	GpioCtrlRegs.GPBDIR.bit.GPIO44 = 0;   // GPIOx = input

	// Enable an GPIO output on GPIOx, set it high
	GpioCtrlRegs.GPBPUD.bit.GPIO43 = 0;   // Enable pullup on GPIOx
	GpioDataRegs.GPBSET.bit.GPIO43 = 1;   // Load output latch
	GpioCtrlRegs.GPBMUX1.bit.GPIO43 = 0;  // GPIOx = GPIOx
	GpioCtrlRegs.GPBDIR.bit.GPIO43 = 0;   // GPIOx = input

//	GpioIntRegs.GPIOXINT3SEL.bit.GPIOSEL = 12; // XINT3 to GPIO44
//	XIntruptRegs.XINT3CR.bit.POLARITY = 0x02;	// 0 - falling adge 3 - F&R adge
//	XIntruptRegs.XINT3CR.bit.ENABLE = 1;

//	PieVectTable.XINT3 = &sifuSinchro;

//	PieCtrlRegs.PIEIER12.bit.INTx1 = 1;	// XINT 3
//	IER |= M_INT12;

	GpioCtrlRegs.GPAPUD.bit.GPIO6 = 0;    // Enable pull-up on GPIO0 (EPWM1A)
	GpioCtrlRegs.GPAPUD.bit.GPIO7 = 0;    // Enable pull-up on GPIO1 (EPWM1B)

/* Configure ePWM-1 pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be ePWM1 functional pins.
// Comment out other unwanted lines.

	GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 1;   // Configure GPIO0 as EPWM1A
	GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 1;   // Configure GPIO1 as EPWM1B

/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

	GpioCtrlRegs.GPAPUD.bit.GPIO8 = 0;    // Enable pull-up on GPIO2 (EPWM2A)
	GpioCtrlRegs.GPAPUD.bit.GPIO8 = 0;    // Enable pull-up on GPIO3 (EPWM3B)

/* Configure ePWM-2 pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be ePWM2 functional pins.
// Comment out other unwanted lines.

	GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 1;   // Configure GPIO2 as EPWM2A
	GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 1;   // Configure GPIO3 as EPWM2B

/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

	GpioCtrlRegs.GPAPUD.bit.GPIO10 = 0;    // Enable pull-up on GPIO4 (EPWM3A)
	GpioCtrlRegs.GPAPUD.bit.GPIO11 = 0;    // Enable pull-up on GPIO5 (EPWM3B)

/* Configure ePWM-3 pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be ePWM3 functional pins.
// Comment out other unwanted lines.

	GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 1;   // Configure GPIO4 as EPWM3A
	GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 1;   // Configure GPIO5 as EPWM3B

	GpioCtrlRegs.GPADIR.bit.GPIO11 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO10 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO9 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO8 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO7 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO6 = 1;

	GpioDataRegs.GPACLEAR.bit.GPIO6 = 0x0;
	GpioDataRegs.GPACLEAR.bit.GPIO7 = 0x0;
	GpioDataRegs.GPACLEAR.bit.GPIO8 = 0x0;
	GpioDataRegs.GPACLEAR.bit.GPIO9 = 0x0;
	GpioDataRegs.GPACLEAR.bit.GPIO10 = 0x0;
	GpioDataRegs.GPACLEAR.bit.GPIO11 = 0x0;

	//=====================================================================
	// Configuration
	//=====================================================================
	// Initialization Time
	//========================// EPWM Module 1 config
	EPwm4Regs.TBPRD = getFixPeriodSifu(); // Period = X TBCLK counts
	EPwm4Regs.TBPHS.half.TBPHS = 0; // Set Phase register to zero
	EPwm4Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;    // Symmetrical mode
	EPwm4Regs.TBCTL.bit.PHSEN = TB_DISABLE; // Master module
	EPwm4Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm4Regs.TBCTL.bit.CLKDIV = TB_DIV1;
	EPwm4Regs.TBCTL.bit.HSPCLKDIV = TB_DIV2;
	EPwm4Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;       // Sync down-stream module
	EPwm4Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm4Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm4Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;     // load on CTR=Zero
	EPwm4Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;     // load on CTR=Zero
	EPwm4Regs.AQCTLA.bit.CAU = AQ_SET; // set actions for EPWM1A
	EPwm4Regs.AQCTLA.bit.CAD = AQ_CLEAR;
	EPwm4Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;        // enable Dead-band module
	EPwm4Regs.DBCTL.bit.POLSEL = DB_ACTV_HI;//DB_ACTV_HIC;         // Active Hi complementary
	EPwm4Regs.DBFED = getDeadTimeSifu(); // FED = x TBCLKs
	EPwm4Regs.DBRED = getDeadTimeSifu(); // RED = x TBCLKs
//========================// EPWM Module 1 config
	EPwm5Regs.TBPRD = getFixPeriodSifu(); // Period = X TBCLK counts
	EPwm5Regs.TBPHS.half.TBPHS = 0; // Set Phase register to zero
	EPwm5Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;    // Symmetrical mode
	EPwm5Regs.TBCTL.bit.PHSEN = TB_DISABLE; // Master module
	EPwm5Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm5Regs.TBCTL.bit.CLKDIV = TB_DIV1;
	EPwm5Regs.TBCTL.bit.HSPCLKDIV = TB_DIV2;
	EPwm5Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;       // Sync down-stream module
	EPwm5Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm5Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm5Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;     // load on CTR=Zero
	EPwm5Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;     // load on CTR=Zero
	EPwm5Regs.AQCTLA.bit.CAU = AQ_SET; // set actions for EPWM1A
	EPwm5Regs.AQCTLA.bit.CAD = AQ_CLEAR;
	EPwm5Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;        // enable Dead-band module
	EPwm5Regs.DBCTL.bit.POLSEL = DB_ACTV_HI;//DB_ACTV_HIC;         // Active Hi complementary
	EPwm5Regs.DBFED = getDeadTimeSifu(); // FED = x TBCLKs
	EPwm5Regs.DBRED = getDeadTimeSifu(); // RED = x TBCLKs
//========================// EPWM Module 1 config
	EPwm6Regs.TBPRD = getFixPeriodSifu(); // Period = X TBCLK counts
	EPwm6Regs.TBPHS.half.TBPHS = 0; // Set Phase register to zero
	EPwm6Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;    // Symmetrical mode
	EPwm6Regs.TBCTL.bit.PHSEN = TB_DISABLE; // Master module
	EPwm6Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm6Regs.TBCTL.bit.CLKDIV = TB_DIV1;
	EPwm6Regs.TBCTL.bit.HSPCLKDIV = TB_DIV2;
	EPwm6Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;       // Sync down-stream module
	EPwm6Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm6Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm6Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;     // load on CTR=Zero
	EPwm6Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;     // load on CTR=Zero
	EPwm6Regs.AQCTLA.bit.CAU = AQ_SET; // set actions for EPWM1A
	EPwm6Regs.AQCTLA.bit.CAD = AQ_CLEAR;
	EPwm6Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;        // enable Dead-band module
	EPwm6Regs.DBCTL.bit.POLSEL = DB_ACTV_HI;//DB_ACTV_HIC;         // Active Hi complementary
	EPwm6Regs.DBFED = getDeadTimeSifu(); // FED = x TBCLKs
	EPwm6Regs.DBRED = getDeadTimeSifu(); // RED = x TBCLKs

//	XIntruptRegs.XINT3CR.bit.ENABLE = 1;
//
//	// Make sure timer is stopped:
//	ECap1Regs.ECCTL2.bit.TSCTRSTOP = 0;
//	ECap1Regs.TSCTR = 0;
//	// Code snippet for APWM mode Example 1
//	// Initialization Time
//	//=======================
//	// ECAP module 1 config
//	ECap1Regs.CTRPHS = 0x0; // make phase zero
//	ECap1Regs.ECCTL2.bit.CAP_APWM = 1/*EC_APWM_MODE*/;
//	ECap1Regs.ECCTL2.bit.APWMPOL = 1/*EC_ACTV_HI*/;     // Active high
//	ECap1Regs.ECCTL2.bit.SYNCI_EN = 0/*EC_DISABLE*/;    // Synch not used
//	ECap1Regs.ECCTL2.bit.SYNCO_SEL = 0x03/*EC_SYNCO_DIS*/; // Synch not used
//	// Run Time (Instant 1, e.g. ISR call)
//	//======================
//	ECap1Regs.CAP1 = _halfPeriod+1; // Set period value
//	ECap1Regs.CAP3 = _halfPeriod+1; // Set period value
//	ECap1Regs.CAP2 = _halfPeriod-1;      // Set Duty cycle i.e. compare value
//	ECap1Regs.CAP4 = _halfPeriod-1;      // Set Duty cycle i.e. compare value
//	// Run Time (Instant 2, e.g. another ISR call)
////	ECap1Regs.ECCTL2.bit.TSCTRSTOP = 1;       // Allow TSCTR to run
//
//	// Make sure timer is stopped:
//	ECap2Regs.ECCTL2.bit.TSCTRSTOP = 0;
//	ECap2Regs.TSCTR = 0;
//	// Code snippet for APWM mode Example 1
//	// Initialization Time
//	//=======================
//	// ECAP module 1 config
//	ECap2Regs.CTRPHS = 0x0; // make phase zero
//	ECap2Regs.ECCTL2.bit.CAP_APWM = 1/*EC_APWM_MODE*/;
//	ECap2Regs.ECCTL2.bit.APWMPOL = 1/*EC_ACTV_HI*/;     // Active high
//	ECap2Regs.ECCTL2.bit.SYNCI_EN = 0/*EC_DISABLE*/;    // Synch not used
//	ECap2Regs.ECCTL2.bit.SYNCO_SEL = 0x03/*EC_SYNCO_DIS*/; // Synch not used
//	// Run Time (Instant 1, e.g. ISR call)
//	//======================
//	ECap2Regs.CAP1 = _60dgrPeriod+1; // Set period value
//	ECap2Regs.CAP3 = _60dgrPeriod+1; // Set period value
//	ECap2Regs.CAP2 = _60dgrPeriod-1;      // Set Duty cycle i.e. compare value
//	ECap2Regs.CAP4 = _60dgrPeriod-1;      // Set Duty cycle i.e. compare value
//	// Run Time (Instant 2, e.g. another ISR call)
//	ECap2Regs.ECCTL2.bit.TSCTRSTOP = 1;       // Allow TSCTR to run
//
//	// Make sure timer is stopped:
//	ECap6Regs.ECCTL2.bit.TSCTRSTOP = 0;
//	ECap6Regs.TSCTR = 0;
//	// Code snippet for APWM mode Example 1
//	// Initialization Time
//	//=======================
//	// ECAP module 1 config
//	ECap6Regs.CTRPHS = 0x0; // make phase zero
//	ECap6Regs.ECCTL2.bit.CAP_APWM = 1/*EC_APWM_MODE*/;
//	ECap6Regs.ECCTL2.bit.APWMPOL = 1/*EC_ACTV_HI*/;     // Active high
//	ECap6Regs.ECCTL2.bit.SYNCI_EN = 0/*EC_DISABLE*/;    // Synch not used
//	ECap6Regs.ECCTL2.bit.SYNCO_SEL = 0x03/*EC_SYNCO_DIS*/; // Synch not used
//	// Run Time (Instant 1, e.g. ISR call)
//	//======================
//	ECap6Regs.CAP1 = _60dgrPeriod+1; // Set period value
//	ECap6Regs.CAP3 = _60dgrPeriod+1; // Set period value
//	ECap6Regs.CAP2 = _60dgrPeriod-1;      // Set Duty cycle i.e. compare value
//	ECap6Regs.CAP4 = _60dgrPeriod-1;      // Set Duty cycle i.e. compare value
//	// Run Time (Instant 2, e.g. another ISR call)
//	ECap6Regs.ECCTL2.bit.TSCTRSTOP = 1;       // Allow TSCTR to run

//	ECap1Regs.ECCLR.all = 0x0ff;
//	ECap1Regs.ECEINT.bit.CTR_EQ_PRD = 0;
//	ECap1Regs.ECEINT.bit.CTR_EQ_CMP = 1;
//	PieCtrlRegs.PIEIER4.bit.INTx1 = 1;
//
//	ECap2Regs.ECCLR.all = 0x0ff;
//	ECap2Regs.ECEINT.bit.CTR_EQ_PRD = 0;
//	ECap2Regs.ECEINT.bit.CTR_EQ_CMP = 1;
//	PieCtrlRegs.PIEIER4.bit.INTx2 = 1;
//
//	ECap6Regs.ECCLR.all = 0x0ff;
//	ECap6Regs.ECEINT.bit.CTR_EQ_PRD = 0;
//	ECap6Regs.ECEINT.bit.CTR_EQ_CMP = 1;
//	PieCtrlRegs.PIEIER4.bit.INTx6 = 1;
//
//    PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;
//	IER |= M_INT4;

		EPwm4Regs.CMPA.half.CMPA = getFixPeriodSifu() - getFixPeriodSifu()/5; // adjust duty for output EPWM1A
		EPwm5Regs.CMPA.half.CMPA = getFixPeriodSifu() - getFixPeriodSifu()/5; // adjust duty for output EPWM2A
		EPwm6Regs.CMPA.half.CMPA = getFixPeriodSifu() - getFixPeriodSifu()/5;

	EDIS;
	cntrlIO_PIN_Sifu(0);
}

void setConer(int coner) {
	UINT32 tmp/* = _MaxConer*/;
	tmp = /*tmp - */coner;
	tmp *= (_halfPeriod - shiftTime);
	tmp >>= 8;
	if(tmp < _minConer)
		tmp = _minConer;
	tmp += shiftTime;
	ConerTime = tmp;
}

void offRectifire(void) {
	setConer(_MaxConer);
	cntrlIO_PIN_Sifu(0);
}

void initSinchroTest(void){
	EALLOW;

		// Enable an GPIO output on GPIOx, set it high
		GpioCtrlRegs.GPBPUD.bit.GPIO44 = 0;   // Enable pullup on GPIOx
		GpioDataRegs.GPBSET.bit.GPIO44 = 1;   // Load output latch
		GpioCtrlRegs.GPBMUX1.bit.GPIO44 = 0;  // GPIOx = GPIOx
		GpioCtrlRegs.GPBDIR.bit.GPIO44 = 0;   // GPIOx = input

		// Enable an GPIO output on GPIOx, set it high
		GpioCtrlRegs.GPBPUD.bit.GPIO43 = 0;   // Enable pullup on GPIOx
		GpioDataRegs.GPBSET.bit.GPIO43 = 1;   // Load output latch
		GpioCtrlRegs.GPBMUX1.bit.GPIO43 = 0;  // GPIOx = GPIOx
		GpioCtrlRegs.GPBDIR.bit.GPIO43 = 0;   // GPIOx = input

	EDIS;
	pulseState = 0;
	pulseCntrl = 0;
	filtrState = 0;
	filtrCntrl = 0;
	stateSinchro = 0;
}

void waitSinchroPulse(void){
	if(GpioDataRegs.GPBDAT.bit.GPIO44)
		pulseCntrl = 0;
	else{
		if(!pulseCntrl){
			pulseCntrl = 1;
			pulseState++;
			if(GpioDataRegs.GPBDAT.bit.GPIO43){
				filtrState++;
			}
		}
	}
	if(GpioDataRegs.GPBDAT.bit.GPIO43)
		filtrCntrl &= 0x0f;
	else{
		if(!(filtrCntrl & 0x0f0)){
			filtrCntrl |= 0x010;
			filtrCntrl++;
		}
	}
}

void testSinchroPulse(void){
	stateSinchro = 0;
//	if(pulseState > _MinTestPulse)
		stateSinchro |= _NORMA_SINCHRO;

//	if((filtrState > _MinTestPulse) &&
//	   ((filtrCntrl & 0x0f) > _MinTestPulse))
		stateSinchro |= _NORMA_PHASE;
}

char getTestSinchro(void){
	return stateSinchro;
}

