/*
 * AquaEconom.c
 *
 *  Created on: 21 нояб. 2019 г.
 *      Author: test
 *
 *  выключение двигателя при отсутсвии расхода воды
 *
 */


#include "aqua.h"
#include "SysTimer/sysTime.h"
#include "AnalogInput/AnalogInput.h"
#include "ModBus/modbus.h"
#include "OsCntrl/OsCntrl.h"
#include "ConvertorCntrl/motorCntrl.h"

UINT8 EconomState;

INT8 testAquaEconom(void){
    if(getRegister(_AQUA_ECONOMISER)){
        if(getRegister(_AQUA_EKONOM_P_STOP)){
            if(getRegister(_AQUA_EKONOM_P_WORK))
                return 1;
        }
    }
    return 0;
}

INT8 testAquaEconomMax(float level){
    float tmp;
    tmp = getRegister(_AQUA_EKONOM_P_STOP);
    tmp = tmp / 10;
    tmp += getNormaOs();
    if(level > tmp)
        return 1;
    else
        return 0;
}

INT8 testAquaEconomMin(float level){
    float tmp;
    tmp = getRegister(_AQUA_EKONOM_P_WORK);
    tmp = tmp / 10;
    tmp += getNormaOs();
    if(level < tmp){
        level = getInvertorFreq();
        tmp = getRegister(_F_MAX_OS);
        if(tmp >= level)
            return 1;
        else
            return -1;
    }else
        return 0;
}

void cntrlAquaEconom(UINT8 *state){
    INT8 r;
    float tmp = getAnalogOs();

    switch(*state){
        default: {
            *state = _aqua_econom_test;
            break;
        }
        case _aqua_econom_test:{
            if(testAquaEconomMax(tmp) == 1){
                *state = _aqua_econom_stop;
                EconomState = 0;
            }
            break;
        }
        case _aqua_econom_stop:{
            r = cntrlSlowStop(&EconomState);
            if(r == 0){
                *state = _aqua_econom_wait;
            }
            break;
        }
        case _aqua_econom_wait:{
            if(testAquaEconomMin(tmp))
                *state = _aqua_econom_restart;
            break;
        }
        case _aqua_econom_restart:{
            r = cntrlStartAqua(&EconomState);
            if(r == 0)
                *state = _aqua_econom_test;
            break;
        }
    }
}
