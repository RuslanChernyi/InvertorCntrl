/*
 * 
 * пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ "пїЅпїЅпїЅпїЅ"
 * 
 * */

#include "added.h"
#include "aqua.h"
#include "ModBus/modbus.h"
#include "OsCntrl/OsCntrl.h"
#include "ConvertorCntrl/statusConvertor.h"

UINT16 stateWater;
UINT16 subStateWater;
float AquaFmax, AquaFmin;

float Fwater, Pmin;

void cntrlStateWater(void){
    float tmp;
    INT8 r;
    if(testErrorInv()){
            stateWater = _error_water;
    }
    switch(stateWater){
        default:{
            stateWater = _off_all_state;
        }
        case _off_all_state:{
            Fwater = 0;
            setFtaskOs(Fwater);
            stateWater = _off_water;
            setAtomPidDelay(c100ms);
            subStateWater = 0;
            break;
        }
        case _off_water:{
            if(!testRunMode()){
                Fwater = 0;
                setFtaskOs(0);
                tmp = getInvertorFreq();
                if(tmp == 0)
                    stateWater = _off_all_state;
            }else{
                stateWater = _clean_pump_water;
            }
            break;
        }
        case _clean_pump_water:{
            r = cntrlAquaPumpClean(&subStateWater);
            if(testRunMode()){
                if((r == 0) || (r == -1)){
                    stateWater = _start_water;
                }
            }else
                stateWater = _off_all_state;
            break;
        }
        case _start_water:{
            if(testRunMode()){
                r = cntrlStartAqua(&subStateWater);
                if(r == 0){
                    stateWater = _run_water;
                    setAtomPidDelay(c10ms);
                }else if(r == 1){
                    stateWater = _fill_tube;
                    initFillTube();
                }
            }
            break;
        }
        case _fill_tube:{
            UINT8 tmp;
            tmp = cntrlFillTube();
            if(tmp & 0x080)
                stateWater = _error_water;;
            if(tmp == 1){
                stateWater = _run_water;;
                setAtomPidDelay(c10ms);
            }
            stateWater = _run_water;
            break;
        }
        case _run_water:{
            if(testRunMode()){
                r = cntrlRunWater(&subStateWater);
              //изменено 05.08.21 обрыв
                if(r==34)
                {
                    stateWater =  _error_water;
                }
                //
            }else
                stateWater = _stop_water;
            break;
        }
        case _stop_water:{
            r = cntrlSlowStop(&subStateWater);
            if(r == 0)
                stateWater = _off_all_state;
            break;
        }
        case _error_water:{
            Fwater = 0;
            setFtaskOs(Fwater);
            stateWater = _off_water;
            break;
        }
    }
}

void initAqua(void){
	stateWater = 0;
}
