/* ******************************************************************
 * сонтроль подсчета системмного времени
 * ******************************************************************/

#include "stddef.h"
#include "DSP2833x_Device.h"
#include "core/initSys.h"
#include "sysTime.h"

unsigned int looseBeltCounter = 0;

#if (CPU_FRQ_100MHZ)

#define _1ms_priod 5000
#define _1ms_prescale 20

#endif

long SystemTime;

pTimeTic listTimeTic;

interrupt void cpu_timer0_isr(void)
{
	pTimeTic pntr;
	pntr = listTimeTic;
	SystemTime++;
	while(pntr != NULL){
		if(pntr->tic)
			pntr->tic--;
		pntr = pntr->next;
	}

	looseBeltCounter++;//17.08.2021 Ruslan Chernyi

	// Acknowledge this interrupt to receive more interrupts from group 1]
	CpuTimer0Regs.TCR.bit.TIF = 1;
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

void initCountTime(void)
{

    // Make sure timer is stopped:
    CpuTimer1Regs.TCR.bit.TSS = 1;
    // CPU Timer 0
    // Initialize pre-scale counter to divide by 1 (SYSCLKOUT):
    CpuTimer1Regs.TPR.all  = 0x0;
    CpuTimer1Regs.TPRH.all = 0x0;

// Configure CPU-Timer 0 to interrupt every 1 milliseconds:
// 100MHz CPU Freq, 50 millisecond Period (in uSeconds)
	CpuTimer1Regs.PRD.all = 0xffffffff;
	CpuTimer1Regs.TPR.all = 100;
    // Reload all counter register with period value:
    CpuTimer1Regs.TCR.bit.TRB = 1;
    CpuTimer1Regs.TCR.bit.FREE = 1;

	CpuTimer1Regs.TCR.bit.TIE = 0;
	// Make sure timer is stopped:
    CpuTimer1Regs.TCR.bit.TSS = 0;
}

UINT32 getCountTimer(void){
	return SystemTime;	//CpuTimer1Regs.TIM.all;
}

void initSysTime(void)
{
	listTimeTic = NULL;

	SystemTime = 0;
	EALLOW;  // This is needed to write to EALLOW protected registers
	PieVectTable.TINT0 = &cpu_timer0_isr;
	EDIS;    // This is needed to disable write to EALLOW protected registers

    // Make sure timer is stopped:
    CpuTimer0Regs.TCR.bit.TSS = 1;
    // CPU Timer 0
    // Initialize pre-scale counter to divide by 1 (SYSCLKOUT):
    CpuTimer0Regs.TPR.all  = 0;
    CpuTimer0Regs.TPRH.all = 0;

// Configure CPU-Timer 0 to interrupt every 1 milliseconds:
// 100MHz CPU Freq, 50 millisecond Period (in uSeconds)
	CpuTimer0Regs.PRD.all = _1ms_priod;
	CpuTimer0Regs.TPR.all = _1ms_prescale;
    // Reload all counter register with period value:
    CpuTimer0Regs.TCR.bit.TRB = 1;
    CpuTimer0Regs.TCR.bit.FREE = 1;

	CpuTimer0Regs.TCR.bit.TIE = 1;
	IER |= M_INT1;
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
    // Make sure timer is stopped:
    CpuTimer0Regs.TCR.bit.TSS = 0;
    initCountTime();
}

void protectSysTimer(void)
{
//	PieCtrlRegs.PIEIER1.bit.INTx7 = 0;
//	disINT(1,INTx7);
	CpuTimer0Regs.TCR.bit.TIE = 1;
}

void unprotectSysTimer(void)
{
//	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
//	enINT(1,INTx7);
	CpuTimer0Regs.TCR.bit.TIE = 1;
}

char addedTimeTic(pTimeTic tic)
{
	pTimeTic pntr;
	tic->next = NULL;
	protectSysTimer();

	if(listTimeTic == NULL)
	{
		listTimeTic = tic;
	}
	else
	{
		pntr = listTimeTic;
		while(pntr->next != NULL)
			pntr = pntr->next;

		pntr->next = tic;
	}
	unprotectSysTimer();
	return 1;
}

char removeTimeTic(pTimeTic tic)
{
	pTimeTic pntr;

	protectSysTimer();
	if(listTimeTic == NULL)
	{
		return 0;
	}
	else
	{
		pntr = listTimeTic;
		if(pntr == tic)
			listTimeTic = pntr->next;
		else
		{
			while(pntr != NULL){
				if(pntr->next == tic){
					pntr->next = tic->next;
					return 1;
				}
				pntr = pntr->next;
			}
			return -1;
		}
	}
	unprotectSysTimer();
	return 1;
}
