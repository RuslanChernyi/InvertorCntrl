/* ******************************************************************
 * инициализация системы
 * ******************************************************************/

#include "DSP2833x_Device.h"
#include "initSys.h"

void InitSysCtrl(void)
{
	asm(" EALLOW");						// Enable EALLOW protected register access

//--- Call the ADC_cal() function located in reserved OTP.
//    ADC_cal_func_ptr is a macro defined in the file example_nonBIOS.h or
//    example_BIOS.h (as may be the case for the example being used).  This
//    macro simply defines ADC_cal_func_ptr to be a function pointer to
//    the correct address in the reserved OTP.  Note that the device cal function
//    is automatically called by the bootloader.  A call to this function is
//    included here just in case the bootloader is bypassed during development.

	SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;	// Enable ADC peripheral clock
	(*ADC_cal_func_ptr)();
	SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 0; 	// Disable ADC peripheral clock

//--- Memory Protection Configuration
	DevEmuRegs.PROTSTART = 0x0100;		// Write default value to protection start register
	DevEmuRegs.PROTRANGE = 0x00FF;		// Write default value to protection range register

//--- Configure the PLL
// Note: The DSP/BIOS configuration tool can also be used to initialize the PLL
// instead of doing the initialization here.

	// Make sure the PLL is not running in limp mode
	if (SysCtrlRegs.PLLSTS.bit.MCLKSTS != 1)
	{													// PLL is not running in limp mode
		SysCtrlRegs.PLLSTS.bit.MCLKOFF = 1;				// Turn off missing clock detect before changing PLLCR
		SysCtrlRegs.PLLSTS.bit.DIVSEL = 0;				// DIVSEL must be 0 or 1  (/4 CLKIN mode) before changing PLLCR
		SysCtrlRegs.PLLCR.bit.DIV = 0x000A;				// PLLx10/4 (because DIVSEL is /4)

		// Wait for PLL to lock.
		// During this time the CPU will run at OSCCLK/4 until the PLL is stable.
		// Once the PLL is stable the CPU will automatically switch to the new PLL value.
		// Code is not required to sit and wait for the PLL to lock.  However,
		// if the code does anything that is timing critical (e.g. something that
		// relies on the CPU clock frequency to be at speed), then it is best to wait
		// until PLL lock is complete.  The watchdog should be disabled before this loop
		// (e.g., as was done above), or fed within the loop.
		while(SysCtrlRegs.PLLSTS.bit.PLLLOCKS != 1)		// Wait for PLLLOCKS bit to set
		{
			SysCtrlRegs.WDKEY = 0x0055;					// Service the watchdog while waiting
			SysCtrlRegs.WDKEY = 0x00AA;					//   in case the user enabled it.
		}

		// After the PLL has locked, we are running in PLLx10/4 mode (since DIVSEL is /4).
		// We can now enable the missing clock detect circuitry, and also change DIVSEL
		// to /2.  In this example, I will wait a bit of time to let inrush currents settle,
		// and then change DIVSEL from /4 to /2.  This is only an example.  The amount of
		// time you need to wait depends on the power supply feeding the DSP (i.e., how much
		// voltage droop occurs due to the inrush currents, and how long it takes the
		// voltage regulators to recover).
		SysCtrlRegs.PLLSTS.bit.MCLKOFF = 0;				// Enable missing clock detect circuitry
		usDelay(20/2);									// Wait 20 us (just an example).  Remember we're running
														// at half-speed here, so divide function argument by 2.
		SysCtrlRegs.PLLSTS.bit.DIVSEL = 0x2;			// Change to /2 mode
	}
	else
	{													// PLL is running in limp mode
	// User should replace the below with a call to an appropriate function,
	// for example shutdown the system (since something is very wrong!).
		asm(" ESTOP0");
	}

//--- Configure the clocks
	SysCtrlRegs.HISPCP.all = 0x0000;		// Hi-speed periph clock prescaler, HSPCLK=SYSCLKOUT/1
	SysCtrlRegs.LOSPCP.all = 0x0002;		// Lo-speed periph clock prescaler, LSPCLK=SYSCLKOUT/4

	SysCtrlRegs.PCLKCR3.bit.GPIOINENCLK = 1;	// GPIO input module is clocked
	SysCtrlRegs.PCLKCR3.bit.XINTFENCLK = 1;		// XINTF module is clocked
	SysCtrlRegs.PCLKCR3.bit.DMAENCLK = 1;		// SYSCLKOUT to DMA enabled
	SysCtrlRegs.PCLKCR3.bit.CPUTIMER2ENCLK = 1;	// SYSCLKOUT to CPU Timer2 enabled
	SysCtrlRegs.PCLKCR3.bit.CPUTIMER1ENCLK = 1;	// SYSCLKOUT to CPU Timer1 enabled
	SysCtrlRegs.PCLKCR3.bit.CPUTIMER0ENCLK = 1;	// SYSCLKOUT to CPU Timer0 enabled

	SysCtrlRegs.PCLKCR1.bit.EQEP2ENCLK = 1;		// SYSCLKOUT to eQEP2 enabled
	SysCtrlRegs.PCLKCR1.bit.EQEP1ENCLK = 1;		// SYSCLKOUT to eQEP1 enabled
	SysCtrlRegs.PCLKCR1.bit.ECAP6ENCLK = 1;		// SYSCLKOUT to eCAP6 enabled
	SysCtrlRegs.PCLKCR1.bit.ECAP5ENCLK = 1;		// SYSCLKOUT to eCAP5 enabled
	SysCtrlRegs.PCLKCR1.bit.ECAP4ENCLK = 1;		// SYSCLKOUT to eCAP4 enabled
	SysCtrlRegs.PCLKCR1.bit.ECAP3ENCLK = 1;		// SYSCLKOUT to eCAP3 enabled
	SysCtrlRegs.PCLKCR1.bit.ECAP2ENCLK = 1;		// SYSCLKOUT to eCAP2 enabled
	SysCtrlRegs.PCLKCR1.bit.ECAP1ENCLK = 1;		// SYSCLKOUT to eCAP1 enabled
	SysCtrlRegs.PCLKCR1.bit.EPWM6ENCLK = 1;		// SYSCLKOUT to ePWM6 enabled
	SysCtrlRegs.PCLKCR1.bit.EPWM5ENCLK = 1;		// SYSCLKOUT to ePWM5 enabled
	SysCtrlRegs.PCLKCR1.bit.EPWM4ENCLK = 1;		// SYSCLKOUT to ePWM4 enabled
	SysCtrlRegs.PCLKCR1.bit.EPWM3ENCLK = 1;		// SYSCLKOUT to ePWM3 enabled
	SysCtrlRegs.PCLKCR1.bit.EPWM2ENCLK = 1;		// SYSCLKOUT to ePWM2 enabled
	SysCtrlRegs.PCLKCR1.bit.EPWM1ENCLK = 1;		// SYSCLKOUT to ePWM1 enabled

	SysCtrlRegs.PCLKCR0.bit.ECANBENCLK = 1;		// SYSCLKOUT/2 to eCAN-B enabled
	SysCtrlRegs.PCLKCR0.bit.ECANAENCLK = 1;		// SYSCLKOUT/2 to eCAN-A enabled
	SysCtrlRegs.PCLKCR0.bit.MCBSPBENCLK = 1;	// LSPCLK to McBSP-B enabled
	SysCtrlRegs.PCLKCR0.bit.MCBSPAENCLK = 1;	// LSPCLK to McBSP-A enabled
	SysCtrlRegs.PCLKCR0.bit.SCIBENCLK = 1;		// LSPCLK to SCI-B enabled
	SysCtrlRegs.PCLKCR0.bit.SCIAENCLK = 1;		// LSPCLK to SCI-A enabled
	SysCtrlRegs.PCLKCR0.bit.SPIAENCLK = 1;		// LSPCLK to SPI-A enabled
	SysCtrlRegs.PCLKCR0.bit.SCICENCLK = 1;		// LSPCLK to SCI-C enabled
	SysCtrlRegs.PCLKCR0.bit.I2CAENCLK = 1;		// SYSCLKOUT to I2C-A enabled
	SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;		// HSPCLK to ADC enabled
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;		// TBCLKSYNC bit is handled separately in InitEPwm()

//--- Configure the low-power modes
	SysCtrlRegs.LPMCR0.all = 0x00FC;		// LPMCR0 set to default value

//--- Finish up
	asm(" EDIS");						// Disable EALLOW protected register access

} // end InitSysCtrl()

//---------------------------------------------------------------------------
// Example: DisableDog:
//---------------------------------------------------------------------------
// This function disables the watchdog timer.

void DisableDog(void)
{
    EALLOW;
    SysCtrlRegs.WDCR= 0x0068;
    EDIS;
}

//---------------------------------------------------------------------------
// InitPieCtrl:
//---------------------------------------------------------------------------
// This function initializes the PIE control registers to a known state.
//
void InitPieCtrl(void)
{

	//--- Disable interrupts
		asm(" SETC INTM, DBGM");			// Disable global interrupts

	//--- Initialize the PIE_RAM

		PieCtrlRegs.PIECTRL.bit.ENPIE = 0;	// Disable the PIE
		asm(" EALLOW");						// Enable EALLOW protected register access
		memcpy(&PieVectTable, &PieVectTableInit, 256);
		asm(" EDIS");						// Disable EALLOW protected register access

	//--- Disable all PIE interrupts
		PieCtrlRegs.PIEIER1.all =  0x0000;
		PieCtrlRegs.PIEIER2.all =  0x0000;
		PieCtrlRegs.PIEIER3.all =  0x0000;
		PieCtrlRegs.PIEIER4.all =  0x0000;
		PieCtrlRegs.PIEIER5.all =  0x0000;
		PieCtrlRegs.PIEIER6.all =  0x0000;
		PieCtrlRegs.PIEIER7.all =  0x0000;
		PieCtrlRegs.PIEIER8.all =  0x0000;
		PieCtrlRegs.PIEIER9.all =  0x0000;
		PieCtrlRegs.PIEIER10.all = 0x0000;
		PieCtrlRegs.PIEIER11.all = 0x0000;
		PieCtrlRegs.PIEIER12.all = 0x0000;

	//--- Clear any potentially pending PIEIFR flags
		PieCtrlRegs.PIEIFR1.all  = 0x0000;
		PieCtrlRegs.PIEIFR2.all  = 0x0000;
		PieCtrlRegs.PIEIFR3.all  = 0x0000;
		PieCtrlRegs.PIEIFR4.all  = 0x0000;
		PieCtrlRegs.PIEIFR5.all  = 0x0000;
		PieCtrlRegs.PIEIFR6.all  = 0x0000;
		PieCtrlRegs.PIEIFR7.all  = 0x0000;
		PieCtrlRegs.PIEIFR8.all  = 0x0000;
		PieCtrlRegs.PIEIFR9.all  = 0x0000;
		PieCtrlRegs.PIEIFR10.all = 0x0000;
		PieCtrlRegs.PIEIFR11.all = 0x0000;
		PieCtrlRegs.PIEIFR12.all = 0x0000;

	//--- Acknowlege all PIE interrupt groups
		PieCtrlRegs.PIEACK.all = 0xFFFF;

	//--- Enable the PIE
		PieCtrlRegs.PIECTRL.bit.ENPIE = 1;		// Enable the PIE

} // end of InitPieCtrl()

#pragma CODE_SECTION(InitFlash, "secureRamFuncs")
void InitFlash(void)
{
	asm(" EALLOW");									// Enable EALLOW protected register access
	FlashRegs.FOPT.bit.ENPIPE = 1;					// Enable the flash pipeline
	FlashRegs.FPWR.bit.PWR = 3;						// Pump and bank set to active mode
	FlashRegs.FSTATUS.bit.V3STAT = 1;				// Clear the 3VSTAT bit
	FlashRegs.FSTDBYWAIT.bit.STDBYWAIT = 0x01FF;	// Sleep to standby transition cycles
	FlashRegs.FACTIVEWAIT.bit.ACTIVEWAIT = 0x01FF;	// Standby to active transition cycles

#if CPU_FRQ_100MHZ
	FlashRegs.FBANKWAIT.bit.RANDWAIT = 3;			// Random access waitstates
	FlashRegs.FBANKWAIT.bit.PAGEWAIT = 3;			// Paged access waitstates
	FlashRegs.FOTPWAIT.bit.OTPWAIT = 5;				// OTP waitstates
#endif

#if CPU_FRQ_150MHZ
	FlashRegs.FBANKWAIT.bit.RANDWAIT = 5;			// Random access waitstates
	FlashRegs.FBANKWAIT.bit.PAGEWAIT = 5;			// Paged access waitstates
	FlashRegs.FOTPWAIT.bit.OTPWAIT = 8;				// OTP waitstates
#endif
	asm(" EDIS");									// Disable EALLOW protected register access

// Force a complete pipeline flush to ensure that the write to the last register
// configured occurs before returning.  Safest thing is to wait 8 full cycles.

    asm(" RPT #6 || NOP");

} // end of InitFlash()

