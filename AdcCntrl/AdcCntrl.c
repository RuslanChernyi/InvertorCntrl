/*
 * ���������� ������� ���
 */

#include "added.h"
#include "DSP2833x_Device.h"     // Device Headerfile and Examples Include File
#include "AdcCntrl.h"
#include "core/initSys.h"
#include "blink/blink.h"

#define __NUMBER_OF_CONVERTION 0x07/*0x033*/
#define _NumberResult 8
#define _NUM_INTEGR 128

#define ADC_Cal (void (*) (void)) 0x380080

#if (CPU_FRQ_150MHZ)     // Default - 150 MHz SYSCLKOUT
  #define ADC_MODCLK 0x3 // HSPCLK = SYSCLKOUT/2*ADC_MODCLK2 = 150/(2*3)   = 25.0 MHz
#endif
#if (CPU_FRQ_100MHZ)
  #define ADC_MODCLK 0x2 // HSPCLK = SYSCLKOUT/2*ADC_MODCLK2 = 100/(2*2)   = 25.0 MHz
#endif

#define ADC_CKPS   0x3   // ADC module clock = HSPCLK/2*ADC_CKPS   = 25.0MHz/(1*2) = 12.5MHz
#define ADC_SHCLK  0x1   // S/H width in ADC module periods                        = 16 ADC clocks
#define AVG        1000  // Average sample limit
#define ZOFFSET    0x00  // Average Zero offset

#define ADCINA0 0
#define ADCINA1 1
#define ADCINA2 2
#define ADCINA3 3
#define ADCINA4 4
#define ADCINA5 5
#define ADCINA6 6
#define ADCINA7 7
#define ADCINB0 8
#define ADCINB1 9
#define ADCINB2 10
#define ADCINB3 11
#define ADCINB4 12
#define ADCINB5 0x0d
#define ADCINB6 14
#define ADCINB7 15

#define _CURRENT_A_CH ADCINA0
#define _CURRENT_B_CH ADCINB0
#define _DC_VOLTAG_CH ADCINA1
#define _ANALOG_TASK_CH ADCINA2
#define _ANALOG_OS_CH ADCINA3
#define _TERMO_SENSOR_CH ADCINB3
#define _CURRENT_ADD_CH ADCINB2
#define _ADDED_AN_CH ADCINB1

UINT16 AdcRes[_NumberResult], AdcIntegr[_NumberResult];
//05.07.17
UINT32 AdcResZr[_NumberResult];
//05.07.17
//UINT16 AdcResBak[_NumberResult];
UINT8 cntIntgrADC;
UINT8 edgeADC = 0;
UINT32 adcIntegrSumm[_NumberResult][_NUM_INTEGR];

interrupt void  adc_isr(void)
{
	setPin1();
  //05.07.17
    if(edgeADC){
        edgeADC = 0;
        cntrlAdcModulOne();
    }else{
        edgeADC = 1;
        cntrlAdcModulZerro();
    }
  //05.07.17
    //05.07.17	cntrlAdcModul();    //05.07.17
	clrPin1();
//	setFlagPwmSet();
  // Reinitialize for next ADC sequence
  AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;         // Reset SEQ1
  AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;       // Clear INT SEQ1 bit
  PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;   // Acknowledge interrupt to PIE

  return;
}


void InitAdcHD(void)
{
    extern void DSP28x_usDelay(Uint32 Count);
    volatile UINT16 dtApd;

    // *IMPORTANT*
	// The ADC_cal function, which  copies the ADC calibration values from TI reserved
	// OTP into the ADCREFSEL and ADCOFFTRIM registers, occurs automatically in the
	// Boot ROM. If the boot ROM code is bypassed during the debug process, the
	// following function MUST be called for the ADC to function according
	// to specification. The clocks to the ADC MUST be enabled before calling this
	// function.
	// See the device data manual and/or the ADC Reference
	// Manual for more information.

	    EALLOW;
		SysCtrlRegs.HISPCP.all = ADC_MODCLK;

		SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;
		//ADC_cal();#define ADC_Cal (void (*) (void)) 0x380080

		dtApd = 40000;
		while(dtApd) dtApd--;

		(*ADC_Cal) ();
		EDIS;

    // To powerup the ADC the ADCENCLK bit should be set first to enable
    // clocks, followed by powering up the bandgap, reference circuitry, and ADC core.
    // Before the first conversion is performed a 5ms delay must be observed
	// after power up to give all analog circuits time to power up and settle

    // Please note that for the delay function below to operate correctly the
	// CPU_RATE define statement in the DSP2833x_Examples.h file must
	// contain the correct CPU clock period in nanoseconds.

    AdcRegs.ADCTRL3.all = 0x00E0;  // Power up bandgap/reference/ADC circuits
}


void initADC(void){

	UINT8 cnt;

	InitAdcHD();

	for(cnt = 0; cnt<_NumberResult; cnt++)
	{
		for(cntIntgrADC = 0; cntIntgrADC <_NUM_INTEGR; cntIntgrADC++)
			adcIntegrSumm[cnt][cntIntgrADC]=0;

		AdcRes[cnt] = 0;
//		AdcResBak[cnt] = 0;
	}

	cntIntgrADC = 0;

	AdcRegs.ADCTRL1.bit.ACQ_PS = ADC_SHCLK;
	AdcRegs.ADCTRL3.bit.ADCCLKPS = ADC_CKPS;
	AdcRegs.ADCTRL1.bit.SEQ_CASC = 1;        // 1 Cascaded Mode
	AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1 = 1;// Enable SOCA from ePWM to start SEQ1
	AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 0x1;  // Enable SEQ1 interrupt (every EOS)
	AdcRegs.ADCTRL2.bit.RST_SEQ1 = 0x1;
	AdcRegs.ADCTRL1.bit.CONT_RUN = 0;	//

	AdcRegs.ADCTRL3.bit.SMODE_SEL = 0;

	AdcRegs.ADCMAXCONV.all = __NUMBER_OF_CONVERTION;

	AdcRegs.ADCCHSELSEQ1.bit.CONV00 = _CURRENT_A_CH;
	AdcRegs.ADCCHSELSEQ1.bit.CONV01 = _CURRENT_B_CH;
	AdcRegs.ADCCHSELSEQ1.bit.CONV02 = _DC_VOLTAG_CH;
	AdcRegs.ADCCHSELSEQ1.bit.CONV03 = _ANALOG_TASK_CH;
	AdcRegs.ADCCHSELSEQ2.bit.CONV04 = _ANALOG_OS_CH;
	AdcRegs.ADCCHSELSEQ2.bit.CONV05 = _TERMO_SENSOR_CH;
	AdcRegs.ADCCHSELSEQ2.bit.CONV06 = _CURRENT_ADD_CH;
	AdcRegs.ADCCHSELSEQ2.bit.CONV07 = _ADDED_AN_CH;

	// Interrupts that are used in this example are re-mapped to
	// ISR functions found within this file.
	   EALLOW;  // This is needed to write to EALLOW protected register
	   PieVectTable.ADCINT = &adc_isr;
	   EDIS;    // This is needed to disable write to EALLOW protected registers

	// Enable ADCINT in PIE
	   PieCtrlRegs.PIEIER1.bit.INTx6 = 1;
	   IER |= M_INT1; // Enable CPU Interrupt 1
}

//void cntrlAdcModul(void)
//{
//	/* ************************************
//	 * ���������� �� ��������� ��������� ������
//	 * ���������� ��������
//	 * ************************************ */
////	UINT16 i;
////	UINT16 tmp[8];
////	UINT32 tmpL;
////	tmp[0] = AdcRegs.ADCRESULT0;
////	tmp[1] = AdcRegs.ADCRESULT1;
////	tmp[2] = AdcRegs.ADCRESULT2;
////	tmp[3] = AdcRegs.ADCRESULT3;
////	tmp[4] = AdcRegs.ADCRESULT4;
////	tmp[5] = AdcRegs.ADCRESULT5;
////	tmp[6] = AdcRegs.ADCRESULT6;
////	tmp[7] = AdcRegs.ADCRESULT7;
////	for(i=0;i<8;i++){
////		tmpL = AdcResBak[i];
////		tmpL += tmp[i];
//////		AdcRes[i]=(AdcResBak[1] + tmp[i])>>1;
////		AdcRes[i] = (UINT16)(tmpL>>1);
////		AdcResBak[i] = tmp[i];
////	}
//
//	AdcRes[0] = AdcRegs.ADCRESULT0;
//	AdcRes[1] = AdcRegs.ADCRESULT1;
//	AdcRes[2] = AdcRegs.ADCRESULT2;
//	AdcRes[3] = AdcRegs.ADCRESULT3;
//	AdcRes[4] = AdcRegs.ADCRESULT4;
//	AdcRes[5] = AdcRegs.ADCRESULT5;
//	AdcRes[6] = AdcRegs.ADCRESULT6;
//	AdcRes[7] = AdcRegs.ADCRESULT7;
//}

void cntrlAdcModulZerro(void)
{
    AdcResZr[0] = AdcRegs.ADCRESULT0;
    AdcResZr[1] = AdcRegs.ADCRESULT1;
    AdcResZr[2] = AdcRegs.ADCRESULT2;
    AdcResZr[3] = AdcRegs.ADCRESULT3;
    AdcResZr[4] = AdcRegs.ADCRESULT4;
    AdcResZr[5] = AdcRegs.ADCRESULT5;
    AdcResZr[6] = AdcRegs.ADCRESULT6;
    AdcResZr[7] = AdcRegs.ADCRESULT7;
}

void cntrlAdcModulOne(void)
{
    AdcResZr[0] += AdcRegs.ADCRESULT0;
    AdcResZr[1] += AdcRegs.ADCRESULT1;
    AdcResZr[2] += AdcRegs.ADCRESULT2;
    AdcResZr[3] += AdcRegs.ADCRESULT3;
    AdcResZr[4] += AdcRegs.ADCRESULT4;
    AdcResZr[5] += AdcRegs.ADCRESULT5;
    AdcResZr[6] += AdcRegs.ADCRESULT6;
    AdcResZr[7] += AdcRegs.ADCRESULT7;

    AdcRes[0] = AdcResZr[0]>>1;
    AdcRes[1] = AdcResZr[1]>>1;
    AdcRes[2] = AdcResZr[2]>>1;
    AdcRes[3] = AdcResZr[3]>>1;
    AdcRes[4] = AdcResZr[4]>>1;
    AdcRes[5] = AdcResZr[5]>>1;
    AdcRes[6] = AdcResZr[6]>>1;
    AdcRes[7] = AdcResZr[7]>>1;
}

void cntrlIntegreteADC( void )
{
	char cnt;

	if(cntIntgrADC > (_NUM_INTEGR-1))
		cntIntgrADC = 0;
	else{
		for (cnt=0; cnt<8; cnt++ )
			adcIntegrSumm[cnt][cntIntgrADC]=AdcRes[cnt];
		cntIntgrADC++;
	}
}

UINT16 getMomenteryResult(UINT8 res)
{
	return AdcRes[res];
}

UINT16 getAveregResult(UINT8 res)
{
	UINT16 val;
	UINT32 tmp;
	tmp = 0;
	val = _NUM_INTEGR-1;
	do{
		tmp += adcIntegrSumm[res][val--];
	}while(val);
	tmp = tmp / _NUM_INTEGR;
	return (UINT16)tmp;
}
