/* ********************************************************
 * ”правление аппаратным модулем UART
 * ******************************************************** */

#ifndef ___CNTRL_UART_H___
#define ___CNTRL_UART_H___

#include "added.h"

void sci_fifo_init(void);
void initGPIO(void);
void initUART(void);
void initGPOIUart(void);

char getUART_RC(char* rc);
char testUART_RC(void);
char testUART_TX(void);
char setUART_TX(char tx);

void initUartInt(void);

void initCoreUART_A(void);
void initCoreUART_C(void);

void enableSciTxInt(void);
void disableSciTxInt(void);
void enableSciRcInt(void);
void disableSciRcInt(void);

void initUART_B(void);
interrupt void scibTxFifoIsr(void);
interrupt void scibRxFifoIsr(void);
void initUartBInt(void);
void initCoreUART_B(void);
void scib_fifo_init(void);
void InitScibGpio();
void enableScibTxInt(void);
void disableScibTxInt(void);
void enableScibRcInt(void);
void disableScibRcInt(void);

void initUART_485(void);
char getUART_RC_485(char* rc);
char testUART_RC_485(void);
char testUART_TX_485(void);
char setUART_TX_485(char tx);
interrupt void sciTxFifoIsr_485(void);
interrupt void sciRxFifoIsr_485(void);
void initUartInt_485(void);
void enableSciTxInt_485(void);
void disableSciTxInt_485(void);
void enableSciRcInt_485(void);
void disableSciRcInt_485(void);

void initUARTAddRS(void);
void scib_fifo_init(void);
void InitScibGpio(void);
void initGPOIUartAddRS(void);
void sci_fifo_initAddRS(void);
char getUART_RCAddRS(char* rc);
char testUART_RCAddRS(void);
char testUART_TXAddRS(void);
char setUART_TXAddRS(char tx);
interrupt void scibTxFifoIsr(void);
interrupt void scibRxFifoIsr(void);
void initUartIntAddRS(void);
void enableSciTxIntAddRS(void);
void disableSciTxIntAddRS(void);
void enableSciRcIntAddRS(void);
void disableSciRcIntAddRS(void);

void runTxDelay(UINT8 number);
void initRS485Timer(void);

void cntrlModbuAndShellRecive(void);
void initModbusAndShell(void);

#define CPU_FRQ_100MHZ /*CPU CLK*/1

#endif
