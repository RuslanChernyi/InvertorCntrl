/*
 * KaskadLimit.c
 *
 *  Created on: 30 окт. 2017 г.
 *      Author: test
 */

#include "added.h"
#include "ModBus/modbus.h"
#include "AnalogInput/AnalogInput.h"
#include "OsCntrl.h"
#include "ConvertorCntrl/CntrlConvertor.h"

INT8 testKaskadLimit(void);

INT8 testOsKHN(void){
    return getStateKaskadAdd();//testKaskadLimit();
}

INT8 testKaskadLimit(void){
    float task, lim;
    lim = getRegister(_F_MAX_OS);
    lim = lim * 0.095;
    task = getCurrentFreq();//getFtaskOs();//
    if(task > lim){
//        lim = getNormaOs() * 0.9;
        lim = getNormaOs() - getDeltaKaskad();
        task = getAnalogOs();
        if(task < lim)
            return 1;
        else
            return 0;
    }else{
        lim = getRegister(_F_MIN_OS);
        lim *= 0.105;
        if(task < lim){
//            lim = getNormaOs() * 1.1;
            lim = getNormaOs() + getDeltaKaskad();
            task = getAnalogOs();
            if(task > lim)
                return -1;
            else
                return 0;
        }else
            return 0;
    }
}
