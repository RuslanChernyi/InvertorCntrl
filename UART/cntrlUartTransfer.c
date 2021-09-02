/* ******************************************************************
 * ���������� �������� ������-�������� ����� UART
 * ******************************************************************/

#include "cntrlUART.h"

#define _NUM_BUFER_RC 64
#define _NUM_BUFER_TX 64

#define protectUartTxBufer() disableSciTxInt()
#define unprotectUartTxBufer() enableSciTxInt()
#define protectUartRcBufer() disableSciRcInt()
#define unprotectUartRcBufer() enableSciRcInt()

char BuferRC[_NUM_BUFER_RC];
char BuferTX[_NUM_BUFER_TX];
int pntrRC, pntrTX=0;

void initUartBufer(void)
{
	pntrRC = 0;
//	pntrTX = 0;
}

char getCharUart(char *rc)
{
	int pntr;
	if(pntrRC){
		protectUartRcBufer();
		*rc = BuferRC[0];

		for(pntr = 0; pntr < pntrRC; pntr++){
			BuferRC[pntr] = BuferRC[pntr+1];
		}
		pntrRC--;

		unprotectUartRcBufer();
		return 1;
	}else
		return 0;
}

char putRcBuf(char rc)
{
	if(pntrRC < _NUM_BUFER_RC)
		{
			protectUartRcBufer();
			BuferRC[pntrRC] = rc;
			pntrRC++;
			unprotectUartRcBufer();
			return 1;
		}
		else
			return 0;
}

char putRcBufFromInt(char rc)
{
	if(pntrRC < _NUM_BUFER_RC)
		{
			BuferRC[pntrRC] = rc;
			pntrRC++;
			return 1;
		}
		else
			return 0;
}

char getTxBufForInt(char *tx)
{
	int pntr;
	if(pntrTX){
		*tx = BuferTX[0];

		for(pntr = 0; pntr < pntrTX; pntr++){
			BuferTX[pntr] = BuferTX[pntr+1];
		}
		pntrTX--;

		return 1;
	}else
		return 0;
}

char getTxBuf(char *tx)
{
	int pntr;
	if(pntrTX){
		protectUartTxBufer();
		*tx = BuferTX[0];

		for(pntr = 0; pntr < pntrTX; pntr++){
			BuferTX[pntr] = BuferTX[pntr+1];
		}
		pntrTX--;

		unprotectUartTxBufer();
		return 1;
	}else
		return 0;
}

char putCharUart (char tx)
{
	if(pntrTX < _NUM_BUFER_TX)
	{
		protectUartTxBufer();
		BuferTX[pntrTX] = tx;
		pntrTX++;
		unprotectUartTxBufer();
		return 1;
	}
	else
		return 0;
}

char testTxBuf(void)
{return pntrTX;}

char testRcBuf(void)
{return (_NUM_BUFER_RC-pntrRC);}

char testRcBufCnt(void)
{return (pntrRC);}
