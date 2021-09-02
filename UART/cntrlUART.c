/* ******************************************************************
 * ��������� � ���������� UART SCI
 * ******************************************************************/

#include <DSP2833x_Device.h>
#include "core/initSys.h"
#include "added.h"
#include "cntrlUART.h"
#include "cntrlUartTransfer.h"

//#define __UART2_USE /*��� ���-�*/
#define __UART0_USE /*��� .053*/

//#define _9600_ /*9600 B/sek*/1
#define _38400_ /*38400 B/sek*/1

#if (_9600_) && (CPU_FRQ_100MHZ)
#define _H_BAUD_VAL 1
#define _L_BAUD_VAL 0x0044
#endif

#if (_38400_) && (CPU_FRQ_100MHZ)
#define _H_BAUD_VAL 0
#define _L_BAUD_VAL 80
#endif

#define _MaxBuf 0x07

void initCoreUART_232(void){
	// Note: Clocks were turned on to the SCIA peripheral
	// in the InitSysCtrl() function

	SciaRegs.SCICCR.all =0x0007;   // 1 stop bit,  No loopback
								   // No parity,8 char bits,
								   // async mode, idle-line protocol
	SciaRegs.SCICTL1.all =0x0003;  // enable TX, RX, internal SCICLK,
								   // Disable RX ERR, SLEEP, TXWAKE
	SciaRegs.SCICTL2.all =0x0003;
	//SciaRegs.SCICTL2.bit.TXINTENA =1;
	//SciaRegs.SCICTL2.bit.RXBKINTENA =1;
	#if (CPU_FRQ_150MHZ)
		  SciaRegs.SCIHBAUD    =_H_BAUD_VAL;  // 9600 baud @LSPCLK = 37.5MHz.
		  SciaRegs.SCILBAUD    =_L_BAUD_VAL;
	#endif
    #if (CPU_FRQ_100MHZ)
		  ;  // 9600 baud @LSPCLK = 20MHz.
	  SciaRegs.SCIHBAUD    =_H_BAUD_VAL;  // 9600 baud @LSPCLK = 20MHz.
	  SciaRegs.SCILBAUD    =_L_BAUD_VAL;
	#endif
	SciaRegs.SCICTL1.all =0x0023;  // Relinquish SCI from Reset
}

void initUART(void)
{
	initUartBufer();
	initGPOIUart();
	sci_fifo_init();
	initCoreUART_232();
	initUartInt();
}

// Initalize the SCI FIFO
//void scia_fifo_init(void)
//{
//    SciaRegs.SCIFFTX.all=0xE040;
//    SciaRegs.SCIFFTX.bit.TXFFIENA = 1;
//    SciaRegs.SCIFFRX.all=0x2041;
//    SciaRegs.SCIFFRX.bit.RXFFIENA = 1;
//    SciaRegs.SCIFFCT.all=0x0;
//}


void initCoreUART_C(void)
{
    // Note: Clocks were turned on to the SCIA peripheral
    // in the InitSysCtrl() function

 	ScicRegs.SCICCR.all =0x0007;   // 1 stop bit,  No loopback
                                   // No parity,8 char bits,
                                   // async mode, idle-line protocol
	ScicRegs.SCICTL1.all =0x0003;  // enable TX, RX, internal SCICLK,
                                   // Disable RX ERR, SLEEP, TXWAKE
	ScicRegs.SCICTL2.all =0x0003;
//	ScicRegs.SCICTL2.bit.TXINTENA =1;
//	ScicRegs.SCICTL2.bit.RXBKINTENA =1;
	#if (CPU_FRQ_150MHZ)
	      ScicRegs.SCIHBAUD    =_H_BAUD_VAL;  // 9600 baud @LSPCLK = 37.5MHz.
	      ScicRegs.SCILBAUD    =_H_BAUD_VAL;
	#endif
	#if (CPU_FRQ_100MHZ)
      ScicRegs.SCIHBAUD    =_H_BAUD_VAL;  // 9600 baud @LSPCLK = 20MHz.
      ScicRegs.SCILBAUD    =_L_BAUD_VAL;
	#endif
	ScicRegs.SCICTL1.all =0x0023;  // Relinquish SCI from Reset
}


void initGPOIUart(void)
{
#ifdef __UART2_USE

#endif

#ifdef __UART0_USE

	   EALLOW;

	/* Enable internal pull-up for the selected pins */
	// Pull-ups can be enabled or disabled disabled by the user.
	// This will enable the pullups for the specified pins.

		GpioCtrlRegs.GPAPUD.bit.GPIO28 = 0;    // Enable pull-up for GPIO62 (SCIRXDC)
		GpioCtrlRegs.GPAPUD.bit.GPIO29 = 0;	   // Enable pull-up for GPIO63 (SCITXDC)

	/* Set qualification for selected pins to asynch only */
	// Inputs are synchronized to SYSCLKOUT by default.
	// This will select asynch (no qualification) for the selected pins.

		GpioCtrlRegs.GPAQSEL2.bit.GPIO28 = 3;  // Asynch input GPIO62 (SCIRXDC)

	/* Configure SCI-C pins using GPIO regs*/
	// This specifies which of the possible GPIO pins will be SCI functional pins.

		GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 1;   // Configure GPIO62 for SCIRXDC operation
		GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 1;   // Configure GPIO63 for SCITXDC operation

	    EDIS;

#endif
}

void sci_fifo_init(void)
{
#ifdef __UART2_USE
    ScicRegs.SCIFFTX.all=0xE040;
    ScicRegs.SCIFFTX.bit.TXFFIENA = 1;
    ScicRegs.SCIFFRX.all=0x2041;
    ScicRegs.SCIFFRX.bit.RXFFIENA = 1;
    ScicRegs.SCIFFCT.all=0x0;
#endif

#ifdef __UART0_USE
    SciaRegs.SCIFFTX.all=0xE040;
    SciaRegs.SCIFFTX.bit.TXFFIENA = 1;
    SciaRegs.SCIFFRX.all=0x2041;
    SciaRegs.SCIFFRX.bit.RXFFIENA = 1;
    SciaRegs.SCIFFCT.all=0x0;
#endif
}

char getUART_RC(char* rc)
{
#ifdef __UART2_USE
	if(ScicRegs.SCIFFRX.bit.RXFFST > 0){
		*rc = ScicRegs.SCIRXBUF.all;
		return 1;
	}else
		return 0;
#endif

#ifdef __UART0_USE
	if(SciaRegs.SCIFFRX.bit.RXFFST > 0){
		*rc = SciaRegs.SCIRXBUF.all;
		return 1;
	}else
		return 0;
#endif
}

char testUART_RC(void)
{
#ifdef __UART2_USE
	return (char)ScicRegs.SCIFFRX.bit.RXFFST;
#endif

#ifdef __UART0_USE
	return (char)SciaRegs.SCIFFRX.bit.RXFFST;
#endif
}

char testUART_TX(void)
{
#ifdef __UART2_USE
	return (char)ScicRegs.SCIFFTX.bit.TXFFST;
#endif

#ifdef __UART0_USE
	return (char)SciaRegs.SCIFFTX.bit.TXFFST;
#endif
}

char setUART_TX(char tx)
{
#ifdef __UART2_USE
    if(ScicRegs.SCIFFTX.bit.TXFFST < _MaxBuf) {
    	ScicRegs.SCITXBUF=tx;
    	return 1;
    }
    else
    	return 0;
#endif

#ifdef __UART0_USE
    if(SciaRegs.SCIFFTX.bit.TXFFST < _MaxBuf) {
    	SciaRegs.SCITXBUF=tx;
    	return 1;
    }
    else
    	return 0;
#endif

}

interrupt void sciTxFifoIsr(void)
{
	char tmp;
#ifdef __UART2_USE
	while(ScicRegs.SCIFFTX.bit.TXFFST<_MaxBuf){
		if(getTxBufForInt(&tmp)){
			ScicRegs.SCITXBUF=tmp;     // Send data
			ScicRegs.SCIFFTX.bit.TXFFINTCLR=1;	// Clear SCI Interrupt flag
		}
		else{
			ScicRegs.SCIFFTX.bit.TXFFIENA = 0;
			break;
		}
	}
	PieCtrlRegs.PIEACK.all|= PIEACK_GROUP8;      // Issue PIE ACK
#endif

#ifdef __UART0_USE
	while(SciaRegs.SCIFFTX.bit.TXFFST<_MaxBuf){
		if(getTxBufForInt(&tmp)){
			SciaRegs.SCITXBUF=tmp;     // Send data
			SciaRegs.SCIFFTX.bit.TXFFINTCLR=1;	// Clear SCI Interrupt flag
		}
		else{
			//disINT(9,INTx2)
			//PieCtrlRegs.PIEIER9.bit.INTx2 = 0;;
			SciaRegs.SCIFFTX.bit.TXFFIENA = 0;
			break;
		}
	}
	PieCtrlRegs.PIEACK.all|= PIEACK_GROUP9;      // Issue PIE ACK
#endif
}

interrupt void sciRxFifoIsr(void)
{
	char tmp;
#ifdef __UART2_USE
	while(ScicRegs.SCIFFRX.bit.RXFFST){
			tmp = ScicRegs.SCIRXBUF.all;

		if(testRcBuf()){
			putRcBufFromInt(tmp);
			//ScicRegs.SCIFFRX.bit.RXFFINTCLR=1;	// Clear SCI Interrupt flag
		}
//		else{
//			ScicRegs.SCIFFRX.bit.RXFFIENA = 0;
//			break;
//		}
	}
	ScicRegs.SCIFFRX.bit.RXFFINTCLR=1;   // Clear Interrupt flag
	ScicRegs.SCIFFRX.bit.RXFFOVRCLR=1;   // Clear Overflow flag
	PieCtrlRegs.PIEACK.all|= PIEACK_GROUP8;      // Issue PIE ACK
#endif
#ifdef __UART0_USE
	while(SciaRegs.SCIFFRX.bit.RXFFST){
		tmp = SciaRegs.SCIRXBUF.bit.RXDT;

		if(testRcBuf()){
			putRcBufFromInt(tmp);
			SciaRegs.SCIFFRX.bit.RXFFINTCLR=1;	// Clear SCI Interrupt flag
		}
//		else{
//			SciaRegs.SCIFFRX.bit.RXFFIENA = 0;
//			break;
//		}
	}
	SciaRegs.SCIFFRX.bit.RXFFINTCLR=1;   // Clear Interrupt flag
	SciaRegs.SCIFFRX.bit.RXFFOVRCLR=1;   // Clear Overflow flag
	PieCtrlRegs.PIEACK.all|= PIEACK_GROUP9;      // Issue PIE ACK
#endif
}

void initUartInt(void)
{
#ifdef __UART2_USE
	EALLOW;
	PieVectTable.SCIRXINTC = &sciRxFifoIsr;
	PieVectTable.SCITXINTC = &sciTxFifoIsr;
	EDIS;
	PieCtrlRegs.PIEIER8.bit.INTx6 = 1;
	PieCtrlRegs.PIEIER8.bit.INTx5 = 1;
	ScicRegs.SCIFFTX.bit.TXFIFOXRESET=1;
	ScicRegs.SCIFFRX.bit.RXFIFORESET=1;
	IER |= M_INT8;
#endif
#ifdef __UART0_USE
	EALLOW;
	PieVectTable.SCIRXINTA = &sciRxFifoIsr;
	PieVectTable.SCITXINTA = &sciTxFifoIsr;
	EDIS;
	PieCtrlRegs.PIEIER9.bit.INTx1 = 1;
	PieCtrlRegs.PIEIER9.bit.INTx2 = 1;
	SciaRegs.SCIFFTX.bit.TXFIFOXRESET=1;
	SciaRegs.SCIFFRX.bit.RXFIFORESET=1;
	IER |= M_INT9;
#endif
}

void enableSciTxInt(void)
{
#ifdef __UART2_USE
    ScicRegs.SCIFFTX.bit.TXFFIENA = 1;
//	PieCtrlRegs.PIEIER8.bit.INTx6 = 1;
#endif
#ifdef __UART0_USE
    SciaRegs.SCIFFTX.bit.TXFFIENA = 1;
    //PieCtrlRegs.PIEIER9.bit.INTx2 = 1;
    //PieCtrlRegs.PIEACK.all|= PIEACK_GROUP9;
//	enINT(9,INTx2);
#endif
}

void disableSciTxInt(void)
{
#ifdef __UART2_USE
    ScicRegs.SCIFFTX.bit.TXFFIENA = 0;
//	PieCtrlRegs.PIEIER8.bit.INTx6 = 0;
#endif
#ifdef __UART0_USE
    SciaRegs.SCIFFTX.bit.TXFFIENA = 0;
    //PieCtrlRegs.PIEIER9.bit.INTx2 = 0;
//	disINT(9,INTx2);
#endif
}

void enableSciRcInt(void)
{
#ifdef __UART2_USE
    ScicRegs.SCIFFRX.bit.RXFFIENA = 1;
//	PieCtrlRegs.PIEIER8.bit.INTx5 = 1;
#endif
#ifdef __UART0_USE
    SciaRegs.SCIFFRX.bit.RXFFIENA = 1;
//    enINT(9,INTx1);
//	PieCtrlRegs.PIEIER9.bit.INTx1 = 1;
//    PieCtrlRegs.PIEACK.all|= PIEACK_GROUP9;
#endif
}

void disableSciRcInt(void)
{
#ifdef __UART2_USE
	ScicRegs.SCIFFRX.bit.RXFFIENA = 0;
//	PieCtrlRegs.PIEIER8.bit.INTx5 = 0;
#endif
#ifdef __UART0_USE
	SciaRegs.SCIFFRX.bit.RXFFIENA = 0;
//	disINT(9,INTx1);
//	PieCtrlRegs.PIEIER9.bit.INTx1 = 0;
#endif
}

void testSCI1_Sts(void){
#ifdef __UART0_USE
	if(SciaRegs.SCIRXST.bit.RXERROR){
		SciaRegs.SCICTL1.bit.SWRESET = 0;
		usDelay(10);
		SciaRegs.SCICTL1.bit.SWRESET = 1;
	}
#endif
}
