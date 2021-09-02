/*
 * KaskadOs.c
 *
 *  Created on: 23 окт. 2017 г.
 *      Author: test
 */

#include "added.h"
#include "SysTimer/sysTime.h"
#include "OsCntrl.h"
#include "KaskadOs.h"
#include "ConvertorCntrl/CntrlConvertor.h"
#include "ConvertorCntrl/statusConvertor.h"
#include "ModBus/modbus.h"
#include "Aqua/aqua.h"

UINT8 stateKaskadOS = 0;
UINT8 stateKaskadAdd;
UINT32 DelayPause, DelaySwitch;
float DeltaKaskad = 0;

INT8 getStateKaskadAdd(void){
    return stateKaskadAdd;
}

float getDeltaKaskad(void){
    return DeltaKaskad;
}

void reinitKaskadOs(void){
    float tmp1, tmp;
    tmp = getRegister(_F_MIN_OS);
    tmp1 = getRegister(_F_MAX_OS);
    tmp /= 10;
    tmp1 /= 10;
    setLimitOs(tmp1, tmp);
    tmp = getRegister(_DELTA_KASKAD);
    tmp *= 0.1;
    DeltaKaskad = tmp;
}

void initAddedKaskad(void){
    stateKaskadAdd = 0;
}

UINT8 upAddKaskad(void){
    if(stateKaskadAdd < getRegister(_NUM_KASKAD)){
        stateKaskadAdd++;
        return 0;
    }
    return 1;
}

UINT8 downAddKaskad(void){
    if(stateKaskadAdd > 0){
        stateKaskadAdd--;
        return 0;
    }
    return 1;
}

INT8 runKaskadOs(void){
    INT8 stsKaskad;
    stsKaskad = testKaskadLimit();
//    if(stsKaskad > 0){
//        if(!upAddKaskad())
//            return 1;
//        else
//            return 2;
//    }else if(stsKaskad < 0){
//        if(!downAddKaskad())
//            return -1;
//        else
//            return -2;
//    }
//    return 0;
    return stsKaskad;
}

void KaskadOsPidCntrl(void){
    float tmp;
    UINT32 time;

    if(testErrorInv()){
        stateKaskadOS = _ALARM_KASKAD_OS;
    }

    switch(stateKaskadOS){
        default:
        case _INIT_KASKAD_OS:{
            reinitKaskadOs();
            stateKaskadOS = _OFF_KASKAD_OS;
            setAtomPidDelay(c10ms);
            initPIDregulator();
            initAddedKaskad();
            cntrlAddedKaskadStage(stateKaskadAdd);
            stateKaskadOS = _START_KASKAD_OS;
            break;
        }
        case _START_KASKAD_OS:{
            if(testRunMode()){
                tmp = getRegister(_F_MIN_OS);
                tmp *= 0.1;
                setFtaskOs(tmp);
                tmp = getInvertorFreq();
                if(tmp >= getFtaskOs()){
                    setPidOs(tmp);
                    DelayPause = getCountTimer();
                    if(getInpReg(_FILL_TUBE_ENABLE)){
                        stateKaskadOS = _TUBE_FILL;
                        setAtomPidDelay(c1sec);
                        initFillTube();
                    }
                    else
                        stateKaskadOS = _RUN_KASKAD_OS;
                }
            }else
                stateKaskadOS = _STOP_KASKAD_OS;
            break;
        }
        case _STEP_DOWN_KASKAD_OS:{
            if(getRegister(_MODE_KSKD_SWTCH)){
                tmp = getRegister(_F_MAX_OS);
                tmp *= 0.1;
                setFtaskOs(tmp);
                tmp = getInvertorFreq();
                if(tmp >= getFtaskOs()){
                    stateKaskadOS = _RUN_KASKAD_OS;
                    setPidOs(tmp);
                }
            }else{
                stateKaskadOS = _RUN_KASKAD_OS;
                UINT32 tmp = getRegister(_KASKAD_PAUSE_DOWN);
                tmp *= 100;
                DelayPause = tmp + getCountTimer();
            }
            break;
        }
        case _STEP_UP_KASKAD_OS:{
            if(getRegister(_MODE_KSKD_SWTCH)){
                tmp = getRegister(_F_MIN_OS);
                tmp *= 0.1;
                setFtaskOs(tmp);
                tmp = getInvertorFreq();
                if(tmp <= getFtaskOs()){
                    stateKaskadOS = _RUN_KASKAD_OS;
                    setPidOs(tmp);
                }
            }else{
                UINT32 tmp = getRegister(_KASKAD_PAUSE_UP);

                stateKaskadOS = _RUN_KASKAD_OS;
                if(getRegister(_NUM_KASKAD) > 1){
                    tmp *= 100;
                }else
                    tmp = 1000;
                DelayPause = tmp + getCountTimer();
            }
            break;
        }
        case _RUN_KASKAD_OS:{
            if(testRunMode()){
                cntrlPIDregulator();
                setFtaskOs(getPidOs());
                if(DelayPause < getCountTimer()){
                    switch(runKaskadOs()){
                        default:{
                            DelaySwitch = getCountTimer();
                            break;
                        }
                        case -1:{
                            if(getRegister(_NUM_KASKAD) > 1){
                                time = getRegister(_KASKAD_PAUSE_DOWN);
                                time *= 100;
                                if((DelaySwitch + time) > getCountTimer()){
                                    break;
                                }
                            }

                            if(downAddKaskad())
                                break;

                            cntrlAddedKaskadStage(stateKaskadAdd);
                            stateKaskadOS = _STEP_DOWN_KASKAD_OS;
                            DelayPause = getCountTimer();
                            break;
                        }
                        case 1:{
                            time = getRegister(_KASKAD_PAUSE_UP);
                            time *= 100;
                            if((DelaySwitch + time) > getCountTimer()){
                                break;
                            }
                            if(upAddKaskad())
                                 break;
                            cntrlAddedKaskadStage(stateKaskadAdd);
                            stateKaskadOS = _STEP_UP_KASKAD_OS;
                            DelayPause = getCountTimer();
                            break;
                        }
                    }
                }
            }else{
                stateKaskadOS = _STOP_KASKAD_OS;
            }
            break;
        }
        case _STOP_KASKAD_OS:{
            if(!testRunMode()){
                setFtaskOs(0);
                initAddedKaskad();
                cntrlAddedKaskadStage(stateKaskadAdd);
                tmp = getInvertorFreq();
                if(tmp == 0)
                    stateKaskadOS = _OFF_KASKAD_OS;
            }else{
                stateKaskadOS = _INIT_KASKAD_OS;
            }
            break;
        }
        case _OFF_KASKAD_OS:{
            if(testRunMode()){
                stateKaskadOS = _INIT_KASKAD_OS;
            }
            break;
        }
        case _ALARM_KASKAD_OS:{
            if(!testErrorInv()){
                stateKaskadOS = _OFF_KASKAD_OS;
            }
            initAddedKaskad();
            cntrlAddedKaskadStage(stateKaskadAdd);
            break;
        }
        case _TUBE_FILL:{
            UINT8 tmp;
            tmp = cntrlFillTube();
            if(tmp & 0x080)
                stateKaskadOS = _ALARM_KASKAD_OS;
            if(tmp == 1){
                stateKaskadOS = _RUN_KASKAD_OS;
                setAtomPidDelay(c10ms);
            }
            break;
        }
    }
}
