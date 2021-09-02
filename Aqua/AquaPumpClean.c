/*
 * AquaPumpClean.c
 *
 *  Created on: 22 нояб. 2019 г.
 *      Author: test
 */


#include "added.h"
#include "aqua.h"
#include "OsCntrl/OsCntrl.h"
#include "SysTimer/sysTime.h"
#include "ModBus/modbus.h"
#include "ConvertorCntrl/statusConvertor.h"
#include "ConvertorCntrl/motorCntrl.h"

#define _TIME_STOP_RUN 2

INT8 cntrlAquaPumpClean(UINT8 *state){
    float tmp;
    static UINT16 num, time;
    UINT16 t;
    switch(*state){
        default:{
            *state = _aqua_clean_test;
            break;
        }
        case _aqua_clean_test:{
            t = getRegister(_CLEAN_REV_NUM);
            if(t > 0){
                tmp = getRegister(_CLEAN_REV_TASK);
                if(tmp > 0){
                    t = getRegister(_CLEAN_REV_ACCEL);
                    if(t > 0){
                        time = t / 5;
                        setTimeAccel(time);
                        setTimeDecel(time);
                        num = getRegister(_CLEAN_REV_NUM);
                        setAtomPidDelay(c250ms);
                        *state = _aqua_clean_decel;
                        break;
                    }
                }
            }
            return -1;
        }
        case _aqua_clean_decel:{
            setFtaskOs(0);
            tmp = getInvertorFreq();
            if(tmp >= 0){
                *state = _aqua_clean_stop;
                time = _TIME_STOP_RUN;
                setAtomPidDelay(c1sec);
            }
            break;
        }
        case _aqua_clean_stop:{
            if(!time){
                if(!num)
                    return 0;
                num--;
                tmp = getRegister(_CLEAN_REV_TASK);
                tmp *= 0.1;
                setFtaskOs(-tmp);
                setAtomPidDelay(c250ms);
                *state = _aqua_clean_accel;
            }else
                time--;
            break;
        }
        case _aqua_clean_accel:{
            tmp = getRegister(_CLEAN_REV_TASK);
            tmp *= -0.1;
            if(getInvertorFreq() <= tmp){
                time = _TIME_STOP_RUN;
                setAtomPidDelay(c1sec);
                *state = _aqua_clean_run;
            }
            break;
        }
        case _aqua_clean_run:{
            if(!time){
                tmp = getRegister(_CLEAN_REV_TASK);
                tmp *= 0.1;
                setFtaskOs(-tmp);
                setAtomPidDelay(c250ms);
                *state = _aqua_clean_decel;
            }else
                time--;
            break;
        }
    }
    return 1;
}
