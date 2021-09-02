/* ******************************************************************
 * ���������� ������� ePWM
 * ******************************************************************/

#include "DSP2833x_Device.h"
#include "DSP2833x_EPwm_defines.h"
#include "cntrlPwm.h"
#include "PWM.h"
#include "ConvertorCntrl/statusConvertor.h"
#include "ModBus/modbus.h"
#include "added.h"

#define _ERROR_4_PIN ((UINT32)1<<15)
#define _ERROR_3_PIN ((UINT32)1<<14)
#define _ERROR_2_PIN ((UINT32)1<<13)
#define _ERROR_1_PIN ((UINT32)1<<12)

UINT16 timePwm[3];

UINT16 getTimePwm(UINT16 phase){
	return timePwm[phase&0x03];
}

interrupt void cpu_pwm_prd_isr(void){
	//UINT16 time[3];

	getPhaseTimePWM(timePwm);
	setPwmDuty(timePwm);

	// Clear INT flag for this timer
	EPwm1Regs.ETCLR.bit.INT = 1;
	// Acknowledge this interrupt to receive more interrupts from group 3
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}

char testErrorPin(UINT32 pin){
	UINT32 i;
	i = 10;
	while(i--){
		if(GpioDataRegs.GPADAT.all & pin){
			return 1;
		}
	}
	return 0;
}

char testAllDriverError(void){
	return testErrorPin(_ERROR_1_PIN | _ERROR_2_PIN | _ERROR_3_PIN | _ERROR_4_PIN);
}

interrupt void cpu_pwm_err1_isr(void){
	// Clear INT flag
	EALLOW;
	EPwm1Regs.TZCLR.bit.CBC = 1;// OST = 1;
	EPwm1Regs.TZCLR.bit.INT = 1;
	EDIS;

	if(!testErrorPin(_ERROR_1_PIN)){
		setStatusConvertor(_ALARM_INV | _PHASE_A);
		rstPwmCntrl();
	}

	// Acknowledge this interrupt to receive more interrupts from group 3
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}

interrupt void cpu_pwm_err2_isr(void){
	// Clear INT flag
	EALLOW;
	EPwm2Regs.TZCLR.bit.CBC = 1;//OST = 1;
	EPwm2Regs.TZCLR.bit.INT = 1;
	EDIS;

	if(!testErrorPin(_ERROR_2_PIN)){
		setStatusConvertor(_ALARM_INV | _PHASE_B);
		rstPwmCntrl();
	}

	// Acknowledge this interrupt to receive more interrupts from group 3
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}

interrupt void cpu_pwm_err3_isr(void){
	// Clear INT flag
	EALLOW;
	EPwm3Regs.TZCLR.bit.CBC = 1;//OST = 1;
	EPwm3Regs.TZCLR.bit.INT = 1;
	EDIS;

	if(!testErrorPin(_ERROR_3_PIN)){
		setStatusConvertor(_ALARM_INV | _PHASE_C);
		rstPwmCntrl();
	}

	// Acknowledge this interrupt to receive more interrupts from group 3
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}

interrupt void cpu_pwm_err4_isr(void){
	// Clear INT flag
	EALLOW;
	EPwm4Regs.TZCLR.bit.CBC = 1;//OST = 1;
	EPwm4Regs.TZCLR.bit.INT = 1;
	EDIS;

	if(!testErrorPin(_ERROR_4_PIN)){
		setStatusConvertor(_ALARM_INV | _BreakErr);
		rstPwmCntrl();
	}

	// Acknowledge this interrupt to receive more interrupts from group 3
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}

void InitEPwmGpio(void)
{
   EALLOW;

/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAPUD.bit.GPIO0 = 0;    // Enable pull-up on GPIO0 (EPWM1A)
    GpioCtrlRegs.GPAPUD.bit.GPIO1 = 0;    // Enable pull-up on GPIO1 (EPWM1B)

/* Configure ePWM-1 pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be ePWM1 functional pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;   // Configure GPIO0 as EPWM1A
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;   // Configure GPIO1 as EPWM1B

/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

	GpioCtrlRegs.GPAPUD.bit.GPIO2 = 0;    // Enable pull-up on GPIO2 (EPWM2A)
	GpioCtrlRegs.GPAPUD.bit.GPIO3 = 0;    // Enable pull-up on GPIO3 (EPWM3B)

/* Configure ePWM-2 pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be ePWM2 functional pins.
// Comment out other unwanted lines.

	GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;   // Configure GPIO2 as EPWM2A
	GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;   // Configure GPIO3 as EPWM2B

/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

	GpioCtrlRegs.GPAPUD.bit.GPIO4 = 0;    // Enable pull-up on GPIO4 (EPWM3A)
	GpioCtrlRegs.GPAPUD.bit.GPIO5 = 0;    // Enable pull-up on GPIO5 (EPWM3B)

/* Configure ePWM-3 pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be ePWM3 functional pins.
// Comment out other unwanted lines.

	GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;   // Configure GPIO4 as EPWM3A
	GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;   // Configure GPIO5 as EPWM3B

	GpioCtrlRegs.GPAPUD.bit.GPIO12 = 1;
	GpioCtrlRegs.GPAPUD.bit.GPIO13 = 1;
	GpioCtrlRegs.GPAPUD.bit.GPIO14 = 1;
	GpioCtrlRegs.GPAPUD.bit.GPIO15 = 1;

	GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 1;
	GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 1;
	GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 1;
	GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 1;

	GpioCtrlRegs.GPADIR.bit.GPIO12 = 0;
	GpioCtrlRegs.GPADIR.bit.GPIO13 = 0;
	GpioCtrlRegs.GPADIR.bit.GPIO14 = 0;
	GpioCtrlRegs.GPADIR.bit.GPIO15 = 0;
	GpioCtrlRegs.GPADIR.all |= 0x03f;

    EDIS;
#ifdef __HI_ACKT
    GpioDataRegs.GPACLEAR.all = 0x03f;
#else
    GpioDataRegs.GPASET.all = 0x03f;
#endif
}

void cntrlIO_PIN(char state){

#ifdef __HI_ACKT
    GpioDataRegs.GPACLEAR.all = 0x03f;
#else
    GpioDataRegs.GPASET.all = 0x03f;
#endif

	if(state){
		EALLOW;

		GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;   // Configure GPIO4 as EPWM1A
		GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;   // Configure GPIO5 as EPWM1B
		GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;   // Configure GPIO4 as EPWM2A
		GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;   // Configure GPIO5 as EPWM2B
		GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;   // Configure GPIO4 as EPWM3A
		GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;   // Configure GPIO5 as EPWM3B

		EDIS;
	}else{
		EALLOW;

		GpioCtrlRegs.GPAMUX1.all &= ~((UINT32)0x0fff);   // Configure GPIO5 as GPIOx
		EDIS;
	}
//#ifdef __HI_ACKT
//    GpioDataRegs.GPACLEAR.all = 0x03f;
//#else
//    GpioDataRegs.GPASET.all = 0x03f;
//#endif

}

UINT16 getPeriodPWM(void){
	UINT16 tmp;
	tmp = getRegister(_PWM_FRC);
	switch(tmp){
		default:
		case 0:{
			return getFixPeriodPWM();	//3000 Hz
		}
		case 1:{
			return getPwm_1500Hz();
		}
		case 2:{
			return getPwm_4500Hz();
		}
		case 3:{
			return getPwm_6000Hz();
		}
		case 4:{
			return getPwm_8000Hz();
		}
	}
//	return getFixPeriodPWM();
}

void initPWM(void){

	UINT16 PwmDuty[3];
	InitEPwmGpio();

	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
	EDIS;

	//=====================================================================
	// Configuration
	//=====================================================================
	// Initialization Time
	//========================// EPWM Module 1 config
	EPwm1Regs.TBPRD = getPeriodPWM(); // Period = X TBCLK counts
	EPwm1Regs.TBPHS.half.TBPHS = 0; // Set Phase register to zero
	EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;    // Symmetrical mode
	EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE; // Master module
	EPwm1Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;
	EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV2;
	EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;       // Sync down-stream module
	EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;     // load on CTR=Zero
	EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;     // load on CTR=Zero
	EPwm1Regs.AQCTLA.bit.CAU = AQ_SET; // set actions for EPWM1A
	EPwm1Regs.AQCTLA.bit.CAD = AQ_CLEAR;
	EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;        // enable Dead-band module
#ifdef __HI_ACKT
	EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;         // Active Hi complementary
#endif
#ifdef __LOW_ACKT
	EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_LOC;         // Active LOWi complementary
#endif
	EPwm1Regs.DBFED = getDeadTime(); // FED = x TBCLKs
	EPwm1Regs.DBRED = getDeadTime(); // RED = x TBCLKs

	//EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
	EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_PRD;     // Select INT on Zero event
	EPwm1Regs.ETSEL.bit.INTEN = 1;                // Enable INT
	EPwm1Regs.ETPS.bit.INTPRD = ET_1ST;           // Generate INT on 1st event
	EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_ZERO;
	EPwm1Regs.ETSEL.bit.SOCAEN = 1;
	EPwm1Regs.ETPS.bit.SOCAPRD = ET_1ST;

	// EPWM Module 2 config
	EPwm2Regs.TBPRD = getPeriodPWM(); // Period = 1600 TBCLK counts
	EPwm2Regs.TBPHS.half.TBPHS = 0; // Set Phase register to zero
	EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;    // Symmetrical mode
	EPwm2Regs.TBCTL.bit.PHSEN = TB_ENABLE; // Slave module
	EPwm2Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;        // sync flow-through
	EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV1;
	EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV2;
	EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;     // load on CTR=Zero
	EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;     // load on CTR=Zero
	EPwm2Regs.AQCTLA.bit.CAU = AQ_SET; // set actions for EPWM2A
	EPwm2Regs.AQCTLA.bit.CAD = AQ_CLEAR;
	EPwm2Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;        // enable Dead-band module
#ifdef __HI_ACKT
	EPwm2Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;         // Active Hi complementary
#endif
#ifdef __LOW_ACKT
	EPwm2Regs.DBCTL.bit.POLSEL = DB_ACTV_LOC;         // Active LOW complementary
#endif
	EPwm2Regs.DBFED = getDeadTime(); // FED = 50 TBCLKs
	EPwm2Regs.DBRED = getDeadTime(); // RED = 50 TBCLKs

	//EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
	EPwm2Regs.ETSEL.bit.INTSEL = ET_CTR_PRD;     // Select INT on Zero event
	EPwm2Regs.ETSEL.bit.INTEN = 0;                // Disable INT
	EPwm2Regs.ETPS.bit.INTPRD = ET_1ST;           // Generate INT on 1st event
	EPwm2Regs.ETSEL.bit.SOCASEL = ET_CTR_PRD;
	EPwm2Regs.ETSEL.bit.SOCAEN = 1;
	EPwm2Regs.ETPS.bit.SOCAPRD = ET_1ST;

	// EPWM Module 3 config
	EPwm3Regs.TBPRD = getPeriodPWM(); // Period = 1600 TBCLK counts
	EPwm3Regs.TBPHS.half.TBPHS = 0; // Set Phase register to zero
	EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;    // Symmetrical mode
	EPwm3Regs.TBCTL.bit.PHSEN = TB_ENABLE; // Slave module
	EPwm3Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	EPwm3Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;        // sync flow-through
	EPwm3Regs.TBCTL.bit.CLKDIV = TB_DIV1;
	EPwm3Regs.TBCTL.bit.HSPCLKDIV = TB_DIV2;
	EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm3Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;     // load on CTR=Zero
	EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;     // load on CTR=Zero
	EPwm3Regs.AQCTLA.bit.CAU = AQ_SET; // set actions for EPWM3A
	EPwm3Regs.AQCTLA.bit.CAD = AQ_CLEAR;
	EPwm3Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;        // enable Dead-band module
#ifdef __HI_ACKT
	EPwm3Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;         // Active Hi complementary
#endif
#ifdef __LOW_ACKT
	EPwm3Regs.DBCTL.bit.POLSEL = DB_ACTV_LOC;         // Active LOW complementary
#endif
	EPwm3Regs.DBFED = getDeadTime(); // FED = 50 TBCLKs
	EPwm3Regs.DBRED = getDeadTime(); // RED = 50 TBCLKs
	// Run Time (Note: Example execution of one run-time instant)
	getPhaseTimePWM(PwmDuty);
	setPwmDuty(PwmDuty);

#ifndef __BLOCK_PWM_ERR
	EALLOW;
	EPwm1Regs.TZSEL.bit.CBC1 = 1;
	EPwm2Regs.TZSEL.bit.CBC2 = 1;
	EPwm3Regs.TZSEL.bit.CBC3 = 1;
	EPwm4Regs.TZSEL.bit.CBC4 = 1;
#ifdef __HI_ACKT
	EPwm1Regs.TZCTL.bit.TZA = TZ_FORCE_LO;
	EPwm2Regs.TZCTL.bit.TZA = TZ_FORCE_LO;
	EPwm3Regs.TZCTL.bit.TZA = TZ_FORCE_LO;
	EPwm4Regs.TZCTL.bit.TZA = TZ_FORCE_LO;
	EPwm1Regs.TZCTL.bit.TZB = TZ_FORCE_LO;
	EPwm2Regs.TZCTL.bit.TZB = TZ_FORCE_LO;
	EPwm3Regs.TZCTL.bit.TZB = TZ_FORCE_LO;
	EPwm4Regs.TZCTL.bit.TZB = TZ_FORCE_LO;
#endif
#ifdef __LOW_ACKT
	EPwm1Regs.TZCTL.bit.TZA = TZ_FORCE_HI;
	EPwm2Regs.TZCTL.bit.TZA = TZ_FORCE_HI;
	EPwm3Regs.TZCTL.bit.TZA = TZ_FORCE_HI;
	EPwm4Regs.TZCTL.bit.TZA = TZ_FORCE_HI;
	EPwm1Regs.TZCTL.bit.TZB = TZ_FORCE_HI;
	EPwm2Regs.TZCTL.bit.TZB = TZ_FORCE_HI;
	EPwm3Regs.TZCTL.bit.TZB = TZ_FORCE_HI;
	EPwm4Regs.TZCTL.bit.TZB = TZ_FORCE_HI;
         // Active LOW complementary
#endif

	EPwm1Regs.TZEINT.bit.CBC = 1;// OST = 1;
	EPwm2Regs.TZEINT.bit.CBC = 1;// OST = 1;
	EPwm3Regs.TZEINT.bit.CBC = 1;// OST = 1;
	EPwm4Regs.TZEINT.bit.CBC = 1;// OST = 1;
	EDIS;
#endif

	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
	EDIS;

	EALLOW;  // This is needed to write to EALLOW protected registers
	PieVectTable.EPWM1_INT = &cpu_pwm_prd_isr;
	PieVectTable.EPWM1_TZINT = &cpu_pwm_err1_isr;
	PieVectTable.EPWM2_TZINT = &cpu_pwm_err2_isr;
	PieVectTable.EPWM3_TZINT = &cpu_pwm_err3_isr;
	PieVectTable.EPWM4_TZINT = &cpu_pwm_err4_isr;
	EDIS;    // This is needed to disable write to EALLOW protected registers
#ifndef __BLOCK_PWM_ERR
	IER |= M_INT2;
#endif
	IER |= M_INT3;
#ifndef __BLOCK_PWM_ERR
	PieCtrlRegs.PIEIER2.bit.INTx1 = 1;	// ������ ��������. 1
	PieCtrlRegs.PIEIER2.bit.INTx2 = 1;	// ������ ��������. 2
	PieCtrlRegs.PIEIER2.bit.INTx3 = 1;	// ������ ��������. 3
	PieCtrlRegs.PIEIER2.bit.INTx4 = 1;	// ������ ��������. BREAK
#endif
	PieCtrlRegs.PIEIER3.bit.INTx1 = 1;	// ���������� ����������

	// ����������� PWM

	cntrlIO_PIN(0);
}

void setPwmDuty(UINT16 *PwmDuty){
//	EPwm1Regs.CMPA.half.CMPA = PwmDuty[1]; // adjust duty for output EPWM1A
//	//PwmDuty++;
//	EPwm2Regs.CMPA.half.CMPA = PwmDuty[0]; // adjust duty for output EPWM2A
//	//PwmDuty++;
//	EPwm3Regs.CMPA.half.CMPA = PwmDuty[2]; // adjust duty for output EPWM3A
	EPwm1Regs.CMPA.half.CMPA = PwmDuty[0]; // adjust duty for output EPWM1A
	EPwm2Regs.CMPA.half.CMPA = PwmDuty[1]; // adjust duty for output EPWM2A
	EPwm3Regs.CMPA.half.CMPA = PwmDuty[2];
}

void setPwmPriod(UINT16 prd){
	EPwm1Regs.TBPRD = prd; // Period = X TBCLK counts
	EPwm2Regs.TBPRD = prd; // Period = X TBCLK counts
	EPwm3Regs.TBPRD = prd; // Period = X TBCLK counts
	EPwm1Regs.CMPA.half.CMPA = prd >> 1;
	EPwm2Regs.CMPA.half.CMPA = prd >> 1;
	EPwm3Regs.CMPA.half.CMPA = prd >> 1;
}

void enablePwmInt(void){
	EPwm1Regs.ETSEL.bit.INTEN = 1;
}

void disablePwmInt(void){
	EPwm1Regs.ETSEL.bit.INTEN = 0;
}
