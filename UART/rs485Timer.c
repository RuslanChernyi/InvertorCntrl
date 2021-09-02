/*
 * rs485Timer.c
 *
 *  Created on: 9 ����. 2014
 *      Author: Tester
 *
 *  ���������� ������� �����-�������� rs485
 *
 */

#include "DSP2833x_Device.h"
#include "added.h"

/*��� SYSCLK 100 MHz*/
#define _period485 ((UINT32)(105000)) /*����� �������� �����*/

#define pinTx485En() GpioDataRegs.GPBSET.bit.GPIO39 = 1;
#define pinRc485En() GpioDataRegs.GPBCLEAR.bit.GPIO39 = 1;

interrupt void cpu_timer485_isr(void)
{	// Acknowledge this interrupt to receive more interrupts from group 1
	ECap5Regs.ECCLR.all = 0x0ff;
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;
	ECap5Regs.ECCTL2.bit.TSCTRSTOP = 0;
	ECap5Regs.TSCTR = 0;
	pinRc485En();
}

void runTxDelay(UINT8 number){
	UINT32 tmp;
	pinTx485En();
	tmp = _period485;
	tmp += ECap5Regs.TSCTR;
	tmp *= number;
//	ECap5Regs.CAP3 = tmp;
//	ECap5Regs.ECCTL2.bit.SWSYNC = 1;
	ECap5Regs.ECCTL2.bit.TSCTRSTOP = 0;
	ECap5Regs.TSCTR = 0;
	ECap5Regs.CAP1 = tmp;
	ECap5Regs.ECCTL2.bit.TSCTRSTOP = 1;
}

void initRS485Timer(void){
	EALLOW;  // This is needed to write to EALLOW protected registers
	PieVectTable.ECAP5_INT = &cpu_timer485_isr;
	EDIS;    // This is needed to disable write to EALLOW protected registers

	EALLOW;

	// Enable an GPIO output on GPIOx, set it high
	GpioCtrlRegs.GPBPUD.bit.GPIO39 = 0;   // Enable pullup on GPIOx
	GpioDataRegs.GPBCLEAR.bit.GPIO39 = 1;   // Load output latch
	GpioCtrlRegs.GPBMUX1.bit.GPIO39 = 0;  // GPIOx = GPIOx
	GpioCtrlRegs.GPBDIR.bit.GPIO39 = 1;   // GPIOx = output

	EDIS;

	// Make sure timer is stopped:
	ECap5Regs.ECCTL2.bit.TSCTRSTOP = 0;
	ECap5Regs.TSCTR = 0;
	// Code snippet for APWM mode Example 1
	// Initialization Time
	//=======================
	// ECAP module 1 config
	ECap5Regs.CTRPHS = 0x0; // make phase zero
	ECap5Regs.ECCTL2.bit.CAP_APWM = 1/*EC_APWM_MODE*/;
	ECap5Regs.ECCTL2.bit.APWMPOL = 1/*EC_ACTV_HI*/;     // Active high
	ECap5Regs.ECCTL2.bit.SYNCI_EN = 0/*EC_DISABLE*/;    // Synch not used
	ECap5Regs.ECCTL2.bit.SYNCO_SEL = 0x03/*EC_SYNCO_DIS*/; // Synch not used
	// Run Time (Instant 1, e.g. ISR call)
	//======================
	ECap5Regs.CAP1 = _period485+1; // Set period value
	ECap5Regs.CAP3 = _period485+1; // Set period value
	ECap5Regs.CAP2 = _period485-1;      // Set Duty cycle i.e. compare value
	ECap5Regs.CAP4 = _period485-1;      // Set Duty cycle i.e. compare value
	// Run Time (Instant 2, e.g. another ISR call)
	ECap5Regs.ECCTL2.bit.TSCTRSTOP = 1;       // Allow TSCTR to run

	ECap5Regs.ECCLR.all = 0x0ff;
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;
	ECap5Regs.ECEINT.bit./*CTR_EQ_CMP*/CTR_EQ_PRD = 1;
	PieCtrlRegs.PIEIER4.bit.INTx5 = 1;
    IER |= M_INT4;
}
