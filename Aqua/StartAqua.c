/*
 * StartAqua.c
 *
 *  Created on: 18 нояб. 2019 г.
 *      Author: test
 */


#include "aqua.h"
#include "SysTimer/sysTime.h"
#include "AnalogInput/AnalogInput.h"
#include "ModBus/modbus.h"
#include "OsCntrl/OsCntrl.h"
#include "ConvertorCntrl/motorCntrl.h"

INT8 cntrlStartAqua(UINT8 *state){
    UINT16 val;
    float freq, fInv;;
    switch(*state){
        default: ;
        case _bgn_start:{
            val = getRegister(_SLOW_START_AQUA);
            val = val / 5;
            setTimeAccel(val);
            freq = getRegister(_F_MIN_OS);
            freq *= 0.1;
            setFtaskOs(freq);
            *state = _slow_start_aqua;
            break;
        }
        case _slow_start_aqua:{
            freq = getRegister(_F_MIN_OS);
            freq *= 0.1;
            fInv = getInvertorFreq();
            if(fInv >= freq){
                val = getRegister(_FILL_TUBE_ENABLE);
                if(!val){
                    *state = _finish_slow_accel;
                }
                else
                    return 1;
            }
            break;
        }
        case _finish_slow_accel:{
            freq = getRegister(_F_MIN_OS);
            freq *= 0.1;
            presetAccelDecel();
            setPidOs(freq);
            return 0;
        }
    }
    return -1;
}
