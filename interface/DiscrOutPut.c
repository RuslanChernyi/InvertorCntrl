/*
 * �������� ���������� �������
 * */

#include "ConvertorCntrl/ReleCntrl.h"
#include "ModBus/modbus.h"
#include "ConvertorCntrl/statusConvertor.h"
#include "ConvertorCntrl/CntrlConvertor.h"
#include "ConvertorCntrl/ReleCntrl.h"
#include "CntrlLimit/cntrlLimit.h"
#include "OsCntrl/OsCntrl.h"
#include "AdcCntrl/AdcCntrl.h"
#include "DiscrOutPut.h"

#define _BYPASS_MODE 7

UINT16 flagDiscrOut;

UINT16 setDiscrOutFlag(UINT16 flg){
    flagDiscrOut |= flg;
    return flagDiscrOut;
}

UINT16 clrDiscrOutFlag(UINT16 flg){
    flagDiscrOut &= ~flg;
    return flagDiscrOut;
}

UINT16 getDiscrOutFlag(void){
    return flagDiscrOut;
}

void initDiscrPinOut(void){
	offDistRele();
	offDistOK1();
	flagDiscrOut = 0;
}

void cntrlOutPin(char mode, void (*onPin)(void),
		                    void (*offPin)(void)){
	switch(mode){
		default:{
			(*offPin)();
			break;
		}
		case 1:{
			if(testErrorInv()){
				(*onPin)();
			}else{
				(*offPin)();
			}

			break;
		}
		case 2:{
			if(testErrorInv()){
				(*offPin)();
			}else{
				if(getModeConvertor() == _off_convertor){
					(*offPin)();
				}else{
					(*onPin)();
				}
			}
			break;
		}
		case 3:{
			if(getModeConvertor() == _run_convertor){
				(*onPin)();
			}else{
				(*offPin)();
			}
			break;
		}
		case 4:{
			if(testOvercurrent()){
				(*onPin)();
			}else{
				(*offPin)();
			}
			break;
		}
		case 5:{
		    INT8 tmp;   //доработка 07.12.18
		    if(getModeConvertor() != _run_convertor){
                (*offPin)();
            }
		    tmp = testOsKHN();
			if(tmp == 1){
				(*onPin)();
			}else{
                (*offPin)();
			}
			break;
		}
		case 6:{
			if(getStatusConvertor(_ErrorConturRer) == _CurentByTimeLimit){
				(*onPin)();
			}else{
				(*offPin)();
			}
			break;
		}
		case 7:{
			if(getStatusConvertor(_ErrorConturRer) == _CurentByTimeLimit){
			    if(!(flagDiscrOut & _BYPASS_DISCR_OUT_1)){
                    onDistRele();
                    flagDiscrOut |= _BYPASS_DISCR_OUT_1;
			    }else if(!(flagDiscrOut & _BYPASS_DISCR_OUT_2)){
			        float current, lim;
			        lim = getCurrentLimit();
			        lim = lim / 4.0f;
			        current = getMomentaryCurrentA();
			        if(current < 0)
			            current = -current;

			        if(current < lim){
			            current = getMomentaryCurrentB();
                        if(current < 0)
                            current = -current;
                        if(current < lim){
                            onDistOK1();
                            flagDiscrOut |= _BYPASS_DISCR_OUT_2;
                        }
			        }
			    }
			}
			break;
		}
	}
}

void cntrlDiscrPinOut(void){
	char tmp;
	if(!(flagDiscrOut & _BLOCK_DISCR_OUT_1)){
	    tmp = getRegister(_DISCR_OUT_VAL1);
	    if(tmp == _BYPASS_MODE)
	        flagDiscrOut = _BLOCK_DISCR_OUT_2;
	    cntrlOutPin(tmp, &onDistRele, &offDistRele);
	}
	if(!(flagDiscrOut & _BLOCK_DISCR_OUT_2)){
	    if(tmp == _BYPASS_MODE)
            flagDiscrOut = _BLOCK_DISCR_OUT_1;
        tmp = getRegister(_DISCR_OUT_VAL2);
        cntrlOutPin(tmp, &onDistOK1, &offDistOK1);
	}
}

void onTermoRele(void){
	onPwrRele();
}

void offTermoRele(void){
	offPwrRele();
}

