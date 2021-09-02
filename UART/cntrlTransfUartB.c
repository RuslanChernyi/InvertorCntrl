/* ******************************************************************
 * управление очередью приеме-передачи через UART
 * ******************************************************************/

#include "cntrlUART.h"

#define _NUM_BUFER_RC 64
#define _NUM_BUFER_TX 64

#define protectUartTxBufer() disableSciTxIntAddRS()
#define unprotectUartTxBufer() enableSciTxIntAddRS()
#define protectUartRcBufer() disableSciRcIntAddRS()
#define unprotectUartRcBufer() enableSciRcIntAddRS()

char BuferBRC[_NUM_BUFER_RC];
char BuferBTX[_NUM_BUFER_TX];
int pntrBRC, pntrBTX;

void initUartBBufer(void)
{
	pntrBRC = 0;
	pntrBTX = 0;
}

char getCharUartB(char *rc)
{
	int pntr;
	if(pntrBRC){
		protectUartRcBufer();
		*rc = BuferBRC[0];

		for(pntr = 0; pntr < pntrBRC; pntr++){
			BuferBRC[pntr] = BuferBRC[pntr+1];
		}
		pntrBRC--;

		unprotectUartRcBufer();
		return 1;
	}else
		return 0;
}

char putRcBufB(char rc)
{
	if(pntrBRC < _NUM_BUFER_RC)
		{
			protectUartRcBufer();
			BuferBRC[pntrBRC] = rc;
			pntrBRC++;
			unprotectUartRcBufer();
			return 1;
		}
		else
			return 0;
}

char putRcBufBFromInt(char rc)
{
	if(pntrBRC < _NUM_BUFER_RC)
		{
			BuferBRC[pntrBRC] = rc;
			pntrBRC++;
			return 1;
		}
		else
			return 0;
}

char getTxBufBForInt(char *tx)
{
	int pntr;
	if(pntrBTX){
		*tx = BuferBTX[0];

		for(pntr = 0; pntr < pntrBTX; pntr++){
			BuferBTX[pntr] = BuferBTX[pntr+1];
		}
		pntrBTX--;

		return 1;
	}else
		return 0;
}

char getTxBufB(char *tx)
{
	int pntr;
	if(pntrBTX){
		protectUartTxBufer();
		*tx = BuferBTX[0];

		for(pntr = 0; pntr < pntrBTX; pntr++){
			BuferBTX[pntr] = BuferBTX[pntr+1];
		}
		pntrBTX--;

		unprotectUartTxBufer();
		return 1;
	}else
		return 0;
}

char putCharUartB (char tx)
{
	if(pntrBTX < _NUM_BUFER_TX)
	{
		protectUartTxBufer();
		BuferBTX[pntrBTX] = tx;
		pntrBTX++;
		unprotectUartTxBufer();
		return 1;
	}
	else
		return 0;
}

char testTxBufB(void)
{return pntrBTX;}

char testRcBufB(void)
{return (_NUM_BUFER_RC-pntrBRC);}

char testRcBufBCnt(void)
{return (pntrBRC);}
