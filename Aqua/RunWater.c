/*
 * RunWater.c
 *
 *  Created on: 20 нояб. 2019 г.
 *      Author: test
 */

#include "aqua.h"
#include "OsCntrl/OsCntrl.h"
#include "AnalogInput/AnalogInput.h"
#include "ModBus/Registers.h"
#include "ConvertorCntrl/statusConvertor.h"
#include "ConvertorCntrl/motorCntrl.h"
UINT8 testLooseCurrent(void);
void reinitAquaLimOs(void){
    reinitKaskadOs();
}


UINT16 testEndOfRange(float Ftask, float Fmax){
    /*конец характеристики насоса*/
    if(Ftask > (0.9*Fmax)){
        return 1;
    }
    return 0;
}

UINT16 testDryRun(float Ftask, float Fmax){
    /*контроль сухого хода*/
    float p;
    if(Ftask > (0.8*Fmax)){
        p = getAnalogOs();
        if(p < (0.7 * getNormaOs())){
            return 1;
        }
    }
    return 0;
}


INT8 cntrlRunWater(UINT8 *state){
    float tmp, tmp1;

    cntrlPIDregulator();
    tmp = getPidOs();

    setFtaskOs(tmp);
    switch(*state){
        default:{
            *state = _no_cntrl_alrm;
            break;
        }
        case _no_cntrl_alrm:{
            *state = _cntrl_KHN;
            break;
        }
        case _cntrl_KHN:{
            *state = _cntrl_dry_run;
            tmp1 = getRegister(_F_MAX_OS);
            tmp1 /= 10;
            if(testEndOfRange(tmp, tmp1))
                return _cntrl_KHN;
            break;
        }
        case _cntrl_dry_run:{
            tmp1 = getRegister(_F_MAX_OS);
            tmp1 /= 10;
            *state = _no_cntrl_alrm;
            if(testDryRun(tmp, tmp1))
                return _cntrl_dry_run;
            break;
        }

    }
    return 0;
}
