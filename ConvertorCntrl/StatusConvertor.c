/* ******************************************************************
 * модуль контроля статуса инвертора
 * ******************************************************************/

#include "statusConvertor.h"

UINT16 statusConvertor;

void initStatusConvertor(void){
	statusConvertor = 0;
}

UINT16 setStatusConvertor(UINT16 stat){
	statusConvertor |= stat;
	return statusConvertor;
}

UINT16 clrStatusConvertor(UINT16 stat){
	statusConvertor &= ~stat;
	return statusConvertor;
}

UINT16 getStatusConvertor(UINT16 stat){
	return statusConvertor&stat;
}

UINT8 testStatusConvertor(UINT16 msk){
	if(statusConvertor & msk)
		return 1;
	else
		return 0;
}
