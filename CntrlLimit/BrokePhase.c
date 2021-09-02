/*
 * BrokePhase.c
 *
 *  Created on: 1 нояб. 2019 г.
 *      Author: test
 */

#include "added.h"
#include "AdcCntrl/OutCurrentCalc.h"
#include "ConvertorCntrl/statusConvertor.h"
#include "ModBus/Registers.h"

UINT16 cntCntrlBPh;

void cntrlBrokePhase(void){
    float current, coner, lim;
    coner = getAlfaVal();
    current = getImVal();
    lim = getRegister(_NOMINAL_CURRENT);
    lim *= 0.01;
    if(lim > current){
        if(coner < 0){
            cntCntrlBPh++;
            if(cntCntrlBPh > 3)
                setStatusConvertor(_BROKEN_PHASE);
        }else{
            if(cntCntrlBPh)
                cntCntrlBPh--;
        }
    }else{
        cntCntrlBPh = 0;
    }
}

void initBrokenPhaseCntrl(void){
    clrStatusConvertor(_BROKEN_PHASE);
    cntCntrlBPh = 0;
}
