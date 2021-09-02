/*
 * UartAdded.c
 *
 *  Created on: 17 ���. 2014
 *      Author: Tester
 */

/* ******************************************************************
 * ��������� � ���������� UART SCI
 * ******************************************************************/

#include <DSP2833x_Device.h>
#include "core/initSys.h"
#include "added.h"
#include "cntrlUART.h"
#include "cntrlUartTransfer.h"

#define __UART_B_USE /*��� ���-�*/
//#define __UART0_USE /*��� .053*/

//#define _9600_ /*9600 B/sek*/1
//#define _38400_ /*38400 B/sek*/1
#define _115200_ /* B/sek*/1

#if (_9600_) && (CPU_FRQ_100MHZ)
#define _H_BAUD_VAL 1
#define _L_BAUD_VAL 0x0044
#endif

#if (_38400_) && (CPU_FRQ_100MHZ)
#define _H_BAUD_VAL 0
#define _L_BAUD_VAL 80
#endif

#if (_115200_) && (CPU_FRQ_100MHZ)
#define _H_BAUD_VAL 0
#define _L_BAUD_VAL 26
#endif

#define _MaxBuf 0x07

void initUARTAddRS(void)
{
	initUartBBufer();
	initGPOIUartAddRS();
	scib_fifo_init();
#ifdef __UART_B_USE
	initCoreUART_B();
#endif
#ifdef __UART0_USE
	initCoreUART_A();
#endif
	initUartIntAddRS();
}

// Initalize the SCI FIFO
void scib_fifo_init(void)
{
    ScibRegs.SCIFFTX.all=0xE040;
    ScibRegs.SCIFFTX.bit.TXFFIENA = 1;
    ScibRegs.SCIFFRX.all=0x2041;
    ScibRegs.SCIFFRX.bit.RXFFIENA = 1;
    ScibRegs.SCIFFCT.all=0x0;
}

void InitScibGpio()
{
   EALLOW;

/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled disabled by the user.
// This will enable the pullups for the specified pins.

	GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;    // Enable pull-up for GPIO62 (SCIRXDC)
	GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;	   // Enable pull-up for GPIO63 (SCITXDC)

/* Set qualification for selected pins to asynch only */
// Inputs are synchronized to SYSCLKOUT by default.
// This will select asynch (no qualification) for the selected pins.

	GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3;  // Asynch input GPIO62 (SCIRXDC)

/* Configure SCI-C pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be SCI functional pins.

	GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 2;   // Configure GPIO62 for SCIRXDC operation
	GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 2;   // Configure GPIO63 for SCITXDC operation

    EDIS;
}

void initCoreUART_B(void)
{
    // Note: Clocks were turned on to the SCIA peripheral
    // in the InitSysCtrl() function

	ScibRegs.SCICCR.all =0x0007;   // 1 stop bit,  No loopback
                                   // No parity,8 char bits,
                                   // async mode, idle-line protocol
	ScibRegs.SCICTL1.all =0x0003;  // enable TX, RX, internal ScibLK,
                                   // Disable RX ERR, SLEEP, TXWAKE
	ScibRegs.SCICTL2.all =0x0003;
//	ScibRegs.SCICTL2.bit.TXINTENA =1;
//	ScibRegs.SCICTL2.bit.RXBKINTENA =1;
	ScibRegs.SCIHBAUD    =_H_BAUD_VAL;  // 9600 baud @LSPCLK = 20MHz.
	ScibRegs.SCILBAUD    =_L_BAUD_VAL;
	ScibRegs.SCICTL1.all =0x0023;  // Relinquish SCI from Reset
}

void initGPOIUartAddRS(void)
{
#ifdef __UART_B_USE
	InitScibGpio();
#endif

#ifdef __UART0_USE
	InitSciaGpioAddRS();
#endif
}

void sci_fifo_initAddRS(void)
{
#ifdef __UART_B_USE
	scib_fifo_init();
#endif

#ifdef __UART0_USE
	Scia_fifo_init();
#endif

}

char getUART_RCAddRS(char* rc)
{
#ifdef __UART_B_USE
	if(ScibRegs.SCIFFRX.bit.RXFFST > 0){
		*rc = ScibRegs.SCIRXBUF.all;
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

char testUART_RCAddRS(void)
{
#ifdef __UART_B_USE
	return (char)ScibRegs.SCIFFRX.bit.RXFFST;
#endif

#ifdef __UART0_USE
	return (char)SciaRegs.SCIFFRX.bit.RXFFST;
#endif
}

char testUART_TXAddRS(void)
{
#ifdef __UART_B_USE
	return (char)ScibRegs.SCIFFTX.bit.TXFFST;
#endif

#ifdef __UART0_USE
	return (char)SciaRegs.SCIFFTX.bit.TXFFST;
#endif
}

char setUART_TXAddRS(char tx)
{
#ifdef __UART_B_USE
    if(ScibRegs.SCIFFTX.bit.TXFFST < _MaxBuf) {
    	ScibRegs.SCITXBUF=tx;
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

interrupt void scibTxFifoIsr(void)
{
	char tmp;
#ifdef __UART_B_USE
	while(ScibRegs.SCIFFTX.bit.TXFFST<_MaxBuf){
		if(getTxBufBForInt(&tmp)){
			ScibRegs.SCITXBUF=tmp;     // Send data
			ScibRegs.SCIFFTX.bit.TXFFINTCLR=1;	// Clear SCI Interrupt flag
		}
		else{
			ScibRegs.SCIFFTX.bit.TXFFIENA = 0;
			break;
		}
	}
	PieCtrlRegs.PIEACK.all|= PIEACK_GROUP9;      // Issue PIE ACK
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

interrupt void scibRxFifoIsr(void)
{
	char tmp;
#ifdef __UART_B_USE
	while(ScibRegs.SCIFFRX.bit.RXFFST){
			tmp = ScibRegs.SCIRXBUF.all;

		if(testRcBufB()){
			putRcBufBFromInt(tmp);
			//ScibRegs.SCIFFRX.bit.RXFFINTCLR=1;	// Clear SCI Interrupt flag
		}
//		else{
//			ScibRegs.SCIFFRX.bit.RXFFIENA = 0;
//			break;
//		}
	}
	ScibRegs.SCIFFRX.bit.RXFFINTCLR=1;   // Clear Interrupt flag
	ScibRegs.SCIFFRX.bit.RXFFOVRCLR=1;   // Clear Overflow flag
	PieCtrlRegs.PIEACK.all|= PIEACK_GROUP9;      // Issue PIE ACK
#endif
#ifdef __UART0_USE
	while(SciaRegs.SCIFFRX.bit.RXFFST){
		tmp = SciaRegs.SCIRXBUF.all;

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
	PieCtrlRegs.PIEACK.all|= PIEACK_GROUP9;      // Issue PIE ACK
	SciaRegs.SCIFFRX.bit.RXFFOVRCLR=1;   // Clear Overflow flag
#endif
}

void initUartIntAddRS(void)
{
#ifdef __UART_B_USE
	EALLOW;
	PieVectTable.SCIRXINTB = &scibRxFifoIsr;
	PieVectTable.SCITXINTB = &scibTxFifoIsr;
	EDIS;
	PieCtrlRegs.PIEIER9.bit.INTx3 = 1;
	PieCtrlRegs.PIEIER9.bit.INTx4 = 1;
	ScibRegs.SCIFFTX.bit.TXFIFOXRESET=1;
	ScibRegs.SCIFFRX.bit.RXFIFORESET=1;
	IER |= M_INT9;
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

void enableSciTxIntAddRS(void)
{
#ifdef __UART_B_USE
    ScibRegs.SCIFFTX.bit.TXFFIENA = 1;
//	PieCtrlRegs.PIEIER8.bit.INTx6 = 1;
#endif
#ifdef __UART0_USE
    SciaRegs.SCIFFTX.bit.TXFFIENA = 1;
    //PieCtrlRegs.PIEIER9.bit.INTx2 = 1;
    //PieCtrlRegs.PIEACK.all|= PIEACK_GROUP9;
//	enINT(9,INTx2);
#endif
}

void disableSciTxIntAddRS(void)
{
#ifdef __UART2_USE
    ScibRegs.SCIFFTX.bit.TXFFIENA = 0;
//	PieCtrlRegs.PIEIER8.bit.INTx6 = 0;
#endif
#ifdef __UART0_USE
    SciaRegs.SCIFFTX.bit.TXFFIENA = 0;
    //PieCtrlRegs.PIEIER9.bit.INTx2 = 0;
//	disINT(9,INTx2);
#endif
}

void enableSciRcIntAddRS(void)
{
#ifdef __UART_B_USE
    ScibRegs.SCIFFRX.bit.RXFFIENA = 1;
//	PieCtrlRegs.PIEIER8.bit.INTx5 = 1;
#endif
#ifdef __UART0_USE
    SciaRegs.SCIFFRX.bit.RXFFIENA = 1;
//    enINT(9,INTx1);
//	PieCtrlRegs.PIEIER9.bit.INTx1 = 1;
//    PieCtrlRegs.PIEACK.all|= PIEACK_GROUP9;
#endif
}

void disableSciRcIntAddRS(void)
{
#ifdef __UART_B_USE
	ScibRegs.SCIFFRX.bit.RXFFIENA = 0;
//	PieCtrlRegs.PIEIER8.bit.INTx5 = 0;
#endif
#ifdef __UART0_USE
	SciaRegs.SCIFFRX.bit.RXFFIENA = 0;
//	disINT(9,INTx1);
//	PieCtrlRegs.PIEIER9.bit.INTx1 = 0;
#endif
}

void testSCI2_Sts(void){
#ifdef __UART_B_USE
	if(ScibRegs.SCIRXST.bit.RXERROR){
		ScibRegs.SCICTL1.bit.SWRESET = 0;
		usDelay(10);
		ScibRegs.SCICTL1.bit.SWRESET = 1;
	}
#endif
}
