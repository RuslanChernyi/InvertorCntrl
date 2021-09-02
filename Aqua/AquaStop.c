/*
 *
 */

#include "aqua.h"
#include "OsCntrl/OsCntrl.h"
#include "ConvertorCntrl/motorCntrl.h"
#include "ModBus/modbus.h"

INT8 cntrlSlowStop(UINT8 *state){
    float f, f_inv;
    int time;

    switch(*state){
        default:
        case _bgn_stop_water:{
            time = getRegister(_AQUA_MIDDLE_STOP);
            time /= 5;
            setTimeAccel(time);
            f = getRegister(_AQUA_F_MIDDLE_STOP);
            f *= 0.1;
            setFtaskOs(f);
            *state = _wait_fmiddle_water;
            return _stop_water;
        }
        case _wait_fmiddle_water:{
            f = getRegister(_AQUA_F_MIDDLE_STOP);
            f *= 0.1;
            f_inv = getInvertorFreq();
            if(f_inv <= f){
                *state = _middle_stop_water;
            }
            break;
        }
        case _middle_stop_water:{
            time = getRegister(_AQUA_SLOW_STOP);
            time /= 5;
            setTimeAccel(time);
            f = getRegister(_FREQ_SLOW_STOP);
            f *= 0.1;
            setFtaskOs(f);
            *state = _wait_fmin_water;
            break;
        }
        case _wait_fmin_water:{
            f = getRegister(_FREQ_SLOW_STOP);
            f *= 0.1;
            f_inv = getInvertorFreq();
            if(f_inv <= f){
                *state = _begin_fast_stop;
            }
            break;
        }
        case _begin_fast_stop:{
            time = getRegister(_AQUA_FAST_STOP);
            time /= 5;
            setTimeAccel(time);
            setFtaskOs(0);
            *state = _wait_stop_fast;
            break;
        }
        case _wait_stop_fast:{
            f = getInvertorFreq();
            if(f <= 0){
                return 0;
            }
            break;
        }
    }
    return -1;
}
