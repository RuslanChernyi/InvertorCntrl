/*
 * пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅ пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ MODBUS
 */

#include "ModBus/modbus.h"
#include "CntrlConvertor.h"
#include "statusConvertor.h"
#include "AdcCntrl/AdcCntrl.h"
extern unsigned int tmp_stat;
UINT16 refrErrorBits(void){
    if(testErrorInv()){
        if(getStatusConvertor(_AllError) == _MaxCurrentLimit){
            SetInpBit(_I_MAX_ALARM, 1);
            return _I_MAX_ALARM;
        }else if(getStatusConvertor(_BROLEN_RECT) == _BROLEN_RECT){
            SetInpBit(_BROKEN_SINCHRO, 1);
            return _BROKEN_SINCHRO;
        }else if(getStatusConvertor(_AllError) == _CurentByTimeLimit){
            SetInpBit(_I_BY_T_ALARM, 1);
            return _I_BY_T_ALARM;
        }else if(getStatusConvertor(_AllError) == _VoltagMaxLimit){
            SetInpBit(_UD_MAX_ALARM, 1);
            return _UD_MAX_ALARM;
        }else if(getStatusConvertor(_AllError) == _VoltagMinLimit){
            SetInpBit(_UD_MIN_ALARM, 1);
            return _UD_MIN_ALARM;
        }else if(getStatusConvertor(_AllError)==_NetMaxLimit){
            SetInpBit(_NET_PWR_ALARM, 1);
            return _NET_PWR_ALARM;
        }else if(getStatusConvertor(_AllError)==_NetMinLimit){
            SetInpBit(_NET_PWR_ALARM, 1);
            return _NET_PWR_ALARM;
        }else if(testStatusConvertor(_ALARM_INV)){
            SetInpBit(_DRIVER_ALARM, 1);
#ifdef __ERROR_PHASE_IND
            if(getStatusConvertor(_DriverError)==_PHASE_A ){
                SetInpBit(_Driver_A_Alrm, 1);
                return _Driver_A_Alrm;
            }else if(getStatusConvertor(_DriverError)==_PHASE_B ){
                SetInpBit(_Driver_B_Alrm, 1);
                return _Driver_B_Alrm;
            }else if(getStatusConvertor(_DriverError)==_PHASE_C ){
                SetInpBit(_Driver_C_Alrm, 1);
                return _Driver_C_Alrm;
            }else if(getStatusConvertor(_DriverError)==_BreakErr ){
                SetInpBit(_Driver_Br_Alrm, 1);
                return _Driver_Br_Alrm;
            }
#else
            return _DRIVER_ALARM;
#endif
        }else if(testStatusConvertor(_BROLEN_PHASE)){
            SetInpBit(_BROKEN_PHASE, 1);
            return _BROKEN_PHASE;
        }
    }else{
        SetInpBit(_DRIVER_ALARM, 0);
        SetInpBit(_I_BY_T_ALARM, 0);
        SetInpBit(_I_MAX_ALARM, 0);
        SetInpBit(_UD_MAX_ALARM, 0);
        SetInpBit(_UD_MIN_ALARM, 0);
        SetInpBit(_NET_PWR_ALARM, 0);
        SetInpBit(_NET_PWR_ALARM, 0);
#ifdef __ERROR_PHASE_IND

        SetInpBit(_Driver_A_Alrm, 0);
        SetInpBit(_Driver_C_Alrm, 0);
        SetInpBit(_Driver_B_Alrm, 0);
        SetInpBit(_Driver_Br_Alrm, 0);

#endif

        SetInpBit(_BROKEN_PHASE, 0);
    }
    return 0;
}

void refreshRegisterMbus(void){
	float tmp;
	tmp = getCurrentFreq();//getInvertorFreq();
	tmp *= 100;
	SetInpReg(_OUTPUT_F_INV, (INT16)tmp);

	tmp = getInvertorVinv();
	tmp *= 10;
	SetInpReg(_OUTPUT_VOLTAG, (INT16)tmp);


	if(costyl_control())
	        {
	     //Костыль для Мариуполя, отображение и старт\стоп из релейной схемы
	        if(tmp_stat){
	            SetInpBit(_RUN_BIT_STATUS, 1);
	            if(testDirectionCmd()){
	                SetInpBit(_REWERS_RUN, 1);
	                SetInpBit(_DIRECT_RUN, 0);
	            }else{
	                SetInpBit(_REWERS_RUN, 0);
	                SetInpBit(_DIRECT_RUN, 1);
	        }
	        }else{
	            SetInpBit(_RUN_BIT_STATUS, 0);
	            SetInpBit(_REWERS_RUN, 0);
	            SetInpBit(_DIRECT_RUN, 0);
	        }
	        }
	else
	            {
	            if(testRunMode()){
	                SetInpBit(_RUN_BIT_STATUS, 1);
	                if(testDirectionCmd()){
	                    SetInpBit(_REWERS_RUN, 1);
	                    SetInpBit(_DIRECT_RUN, 0);
	                }else{
	                    SetInpBit(_REWERS_RUN, 0);
	                    SetInpBit(_DIRECT_RUN, 1);
	            }
	            }else{
	                SetInpBit(_RUN_BIT_STATUS, 0);
	                SetInpBit(_REWERS_RUN, 0);
	                SetInpBit(_DIRECT_RUN, 0);
	            }
	            }

	refrErrorBits();

	if(testStatusConvertor(_BROLEN_PHASE)){
		SetInpBit(_BROKEN_PHASE, 1);
	}else{
		SetInpBit(_BROKEN_PHASE, 0);
	}

	if(testRunState())
		tmp = getAveragIm();//getMomentaryCurrentA();//
	else
		tmp = 0;
	tmp *= 7.071;//10;
	SetInpReg(_OUTPUT_CURRENT, (INT16)tmp);

	tmp = getIntegrVoltagDC();//getAverageVoltagDC();//getMomentaryVoltagDC();//
	tmp *= 10;
	SetInpReg(_DC_VOLTAG, (UINT16)tmp);

    tmp = getNetVoltag();
    tmp *= 10;
    SetInpReg(_NET_VOLTAG, (UINT16)tmp);

	if(testRunState())
		tmp = getRmsCurrentA();//getMomentaryCurrentA();//
	else
		tmp = 0;
	tmp *= 10;
	SetInpReg(_IA_CURRENT, (INT16)tmp);

	tmp = getTemperature();//getAverageVoltagDC();//getMomentaryVoltagDC();//
	tmp *= 10;
	SetInpReg(_ANALOG_TEMPERATURE, (UINT16)tmp);

	if(testRunState())
		tmp = getRmsCurrentB();//getMomentaryCurrentB();//
	else
		tmp = 0;
	tmp *= 10;
	SetInpReg(_IB_CURRENT, (INT16)tmp);

	if(testRunState())
		tmp = getAveragId();//getMomentaryVoltagDC();
	else
		tmp = 0;
	tmp *= 7.071;//10;
	SetInpReg(_OUTPUT_I_D, (INT16)tmp);

	if(testRunState())
		tmp = getAveragIq();//getMomentaryVoltagDC();
	else
		tmp = 0;
	tmp *= 7.071;//10;
	SetInpReg(_OUTPUT_I_Q, (INT16)tmp);

}
