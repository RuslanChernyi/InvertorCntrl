/*
 * MotoTimeCntrl.c
 *
 *  Created on: 6 нояб. 2019 г.
 *      Author: test
 */

#include "added.h"
#include "AdcCntrl/OutCurrentCalc.h"
#include "ConvertorCntrl/CntrlConvertor.h"
#include "ModBus/Registers.h"
#include "SysTimer/sysTime.h"
#include "RtcCntrl/RtcCntrl.h"

#define _MOTORESOURSE_PERIOD (((UINT32)10))
#define _TIME_DELTA 0.01
#define _PERIOD_MR_CNT ((UINT16)100*60*10)/*10 minuts*/

UINT16 MotoTimeResourse, PeriodMR, MinutsMR;
float OutputEnergy;
UINT32 timerMR;

void initMotoresource(void){
    UINT16 tmp;
    UINT32 lTmp;

    getNVRam(_HOUR_TIME_MR, &MotoTimeResourse);

    getNVRam(_OUT_PWR_CNT_H, &tmp);
    lTmp = ((UINT32)tmp) & 0x0ffff;
    lTmp = lTmp << 16;
    getNVRam(_OUT_PWR_CNT_L, &tmp);
    lTmp = lTmp | (((UINT32)tmp)&0x0ffff);
    OutputEnergy = lTmp;
    OutputEnergy = OutputEnergy / 100;

    getNVRam(_MINUTS_TIME_MR, &MinutsMR);

    timerMR = getCountTimer();
    PeriodMR = 0;
}

void saveMinutsMR(UINT16 val){
    setNVRam(_MINUTS_TIME_MR, val);
}

void saveMotoResourse(UINT16 val){
    setNVRam(_HOUR_TIME_MR, val);
}

void saveMotroPwrCnt(float val){
    UINT32 tmp;
    UINT16 data;
    val *= 100;
    tmp = val;
    data = tmp >> 16;
    setNVRam(_OUT_PWR_CNT_H, data);
    data = tmp & 0x0ffff;
    setNVRam(_OUT_PWR_CNT_L, data);
}

void cntrlMotoResource(void){
    UINT32 tmp;
    float pwr;
    tmp = getCountTimer();
    if((tmp - timerMR) > _MOTORESOURSE_PERIOD){
        pwr = getIdVal();
        pwr *= getCurrentVinv();
        pwr *= _TIME_DELTA;
        OutputEnergy += pwr;
        PeriodMR++;
        timerMR = tmp;

    }
    if(!(PeriodMR & 0x03fff)){
        saveMotroPwrCnt(OutputEnergy);
    }
    if(PeriodMR > _PERIOD_MR_CNT){
        PeriodMR = 0;
        MinutsMR++;
        if(MinutsMR > 5/*60 minuts*/){
            MotoTimeResourse++;
            saveMotoResourse(MotoTimeResourse);
        }
        saveMinutsMR(MinutsMR);
    }
}

UINT32 getMotorPwrOut(void){
    UINT32 tmp;
    tmp = (UINT32)(OutputEnergy*100);
    return tmp;
}

UINT16 getTimeMotoResourse(void){
    return MotoTimeResourse;
}
