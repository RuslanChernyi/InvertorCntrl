/*
 * cntrlTransferUart_485.c
 *
 *  Created on: 18 лип. 2014
 *      Author: Tester
 */

/* ******************************************************************
 * управление очередью приеме-передачи через UART 485
 * ******************************************************************/

#include "cntrlUART.h"

#define _NUM_BUFER_RC 64
#define _NUM_BUFER_TX 64

#define protectUartTxBufer() disableSciTxInt_485()
#define unprotectUartTxBufer() enableSciTxInt_485()
#define protectUartRcBufer() disableSciRcInt_485()
#define unprotectUartRcBufer() enableSciRcInt_485()

char Bufer485RC[_NUM_BUFER_RC];
char Bufer485TX[_NUM_BUFER_TX];
int pntr485RC, pntr485TX;

void initUart485Bufer(void)
{
	pntr485RC = 0;
	pntr485TX = 0;
}

char getCharUart485(char *rc)
{
	int pntr;
	if(pntr485RC){
		protectUartRcBufer();
		*rc = Bufer485RC[0];

		for(pntr = 0; pntr < pntr485RC; pntr++){
			Bufer485RC[pntr] = Bufer485RC[pntr+1];
		}
		pntr485RC--;

		unprotectUartRcBufer();
		return 1;
	}else
		return 0;
}

char putRcBuf485(char rc)
{
	if(pntr485RC < _NUM_BUFER_RC)
		{
			protectUartRcBufer();
			Bufer485RC[pntr485RC] = rc;
			pntr485RC++;
			unprotectUartRcBufer();
			return 1;
		}
		else
			return 0;
}

char putRcBuf485FromInt(char rc)
{
	if(pntr485RC < _NUM_BUFER_RC)
		{
			Bufer485RC[pntr485RC] = rc;
			pntr485RC++;
			return 1;
		}
		else
			return 0;
}

char getTxBuf485ForInt(char *tx)
{
	int pntr;
	if(pntr485TX){
		*tx = Bufer485TX[0];

		for(pntr = 0; pntr < pntr485TX; pntr++){
			Bufer485TX[pntr] = Bufer485TX[pntr+1];
		}
		pntr485TX--;

		return 1;
	}else
		return 0;
}

char getTxBuf485(char *tx)
{
	int pntr;
	if(pntr485TX){
		protectUartTxBufer();
		*tx = Bufer485TX[0];

		for(pntr = 0; pntr < pntr485TX; pntr++){
			Bufer485TX[pntr] = Bufer485TX[pntr+1];
		}
		pntr485TX--;

		unprotectUartTxBufer();
		return 1;
	}else
		return 0;
}

char putCharUart485 (char tx)
{
	if(pntr485TX < _NUM_BUFER_TX)
	{
		protectUartTxBufer();
		Bufer485TX[pntr485TX] = tx;
		pntr485TX++;
		unprotectUartTxBufer();
		return 1;
	}
	else
		return 0;
}

char testTxBuf485(void)
{return pntr485TX;}

char testRcBuf485(void)
{return (_NUM_BUFER_RC-pntr485RC);}

char testRcBuf485Cnt(void)
{return (pntr485RC);}

