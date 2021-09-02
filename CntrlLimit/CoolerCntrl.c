/************************************************
 * Управление охлаждением
 * **/

#include "added.h"
#include "AdcCntrl/AdcCntrl.h"
#include "ModBus/Registers.h"
#include "interface/DiscrOutPut.h"
#include "ConvertorCntrl/CntrlConvertor.h"


#define _onTemperatureLim ((float)60.0)
#define _offTemperatureLim ((float)40.0)

void (*cntrlCoolerFn)(void);

void cntrlCoolerBySensor(void){
	if(testRunState()){
		if(getTemperature() > _onTemperatureLim){
			onTermoRele();
		}
		if(getTemperature() < _offTemperatureLim){
			offTermoRele();
		}
	}else{
		offTermoRele();
	}
}


void cntrlCoolerOn(void){
	onTermoRele();
}

void cntrlCoolerByRun(void){
	if(testRunState()){
		onTermoRele();
	}else{
		offTermoRele();
	}
}

void cntrlCooler(void){
	(*cntrlCoolerFn)();
}

void initCoolerCntrl(void){
	switch(getRegister(_COOLER_CNTRL_METHOB)){
		default:
		case 0:{
			cntrlCoolerFn = &cntrlCoolerOn;
		}
		case 1:{
			cntrlCoolerFn = &cntrlCoolerBySensor;
		}
		case 2:{
			cntrlCoolerFn = &cntrlCoolerByRun;
		}
	}
}
