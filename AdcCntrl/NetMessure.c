/*
 * NetMessure.c
 *
 *  Created on: 06 апр. 2017 г.
 *      Author: test
 */

#include "added.h"
#include "AdcCntrl.h"
#include "SysTimer/sysTime.h"
#include "ModBus/Registers.h"
#include "ConvertorCntrl/statusConvertor.h"

INIT_DELAY(netCntrl)

INT32 NetSumm, netRectSumm, averVal;
UINT16 cntNC;
float NetVoltag, mshtNet;

void cntrlNetMessure(void){
	INT32 tmp;
	float fTmp;
	if(TEST_DELAY(netCntrl)){
		SET_DELAY(netCntrl, c1ms);
		if(cntNC){
			cntNC--;
			tmp = ((INT32)getNetAdc()) & 0x0ffff;
			NetSumm += tmp;
			tmp = tmp - averVal;
			if(tmp < 0) tmp = -tmp;
			netRectSumm += tmp;
		}else{
			cntNC = 100;
			fTmp = netRectSumm;
			fTmp = fTmp / 100.0;
			NetVoltag = fTmp * mshtNet;
			averVal = NetSumm / 100;
			NetSumm = 0;
			netRectSumm = 0;
			if(!testStatusConvertor(_AllError)){
				if(testNetMin()){
					setStatusConvertor(_NetMinLimit);
				}
				if(testNetMax()){
					setStatusConvertor(_NetMaxLimit);
				}
			}
		}
	}
}

float getNetVoltag(void){
	return NetVoltag;
}

void initNetMessure(void){
	NetVoltag = 380.0;
	mshtNet = getRegister(_NET_MSHT);
	mshtNet *= 380;
	mshtNet = mshtNet / (32767.0);
	mshtNet = mshtNet / (100.0);
}

void initNetCntrl(void){
	initNetMessure();
	ADD_DELAY(netCntrl, c1ms);
	cntNC = 100;
	NetSumm = 0;
	netRectSumm = 0;
}

UINT8 testNetMax(void){
	float tmp;
	tmp = getRegister(_NET_MAX);
	if(tmp){
		if(tmp < NetVoltag)
			return 1;
	}
	return 0;
}


UINT8 testNetMin(void){
	float tmp;
	tmp = getRegister(_NET_MIN);
	if(tmp){
		if(tmp > NetVoltag)
			return 1;
	}
	return 0;
}
