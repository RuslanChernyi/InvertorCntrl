/* ******************************************************************
 * ���������� �������� ������-�������� ����� UART
 * ******************************************************************/
#ifndef ___CNTRL_UART_TRANSFER_H___
#define ___CNTRL_UART_TRANSFER_H___

char getCharUart(char *rc);
char putRcBuf(char rc);
char getTxBuf(char *tx);
char putCharUart (char tx);
char testTxBuf(void);
char testRcBuf(void);
char testRcBufCnt(void);
char putRcBufFromInt(char rc);
char getTxBufForInt(char *tx);
void initUartBufer(void);

/* ******************************************************************
 * ���������� �������� ������-�������� ����� UART added
 * ******************************************************************/

void initUartBBufer(void);
char getCharUartB(char *rc);
char putRcBufB(char rc);
char putRcBufBFromInt(char rc);
char getTxBufBForInt(char *tx);
char getTxBufB(char *tx);
char putCharUartB (char tx);
char testTxBufB(void);
char testRcBufB(void);
char testRcBufBCnt(void);

/*
 * cntrlTransferUart_485.c
 */

void initUart485Bufer(void);
char getCharUart485(char *rc);
char putRcBuf485(char rc);
char putRcBuf485FromInt(char rc);
char getTxBuf485ForInt(char *tx);
char getTxBuf485(char *tx);
char putCharUart485 (char tx);
char testTxBuf485(void);
char testRcBuf485(void);
char testRcBuf485Cnt(void);

void testSCI1_Sts(void);
void testSCI2_Sts(void);
void testSCI3_Sts(void);

#endif
