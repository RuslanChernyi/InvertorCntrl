/*
 * ��������� ������ �����/����/�����������
 */

#include "added.h"
#include "ConvertorCntrl/statusConvertor.h"
#include "ConvertorCntrl/CntrlConvertor.h"
#include "ModBus/modbus.h"
#include "DiscretPin/DiscretPin.h"

char testExtAlarmStopPin(void){
	switch(getRegister(_ENABLE_ALARM_STOP)){
		default:
			return 0;
		case 1:{
			if(testDiscretInp2(DISCR_ADD_PIN_1))
				return 0;
			else
				return 1;
		}
		case 2:{
			if(testDiscretInp2(DISCR_ADD_PIN_1))
				return 1;
			else
				return 0;
		}
	}
}

void modeSPD(void){
    char pin;
    pin = testDiscretInp(DISCR_CMD_PIN_1);
    if(!getRegister(_DISC_STOP_MODE))
        pin = !pin;
	if(pin){
		setNormalStop();
	}else{
		if(testDiscretInp(DISCR_CMD_PIN_2)){
			setRunMode();
		}
	}
	if(!getRegister(_DISABLE_REWERS)){
		if(testDiscretInp(DISCR_CMD_PIN_3)){
			setDirectionCmd();
			SetCoil(_REWERS_PULT, 1);
		}else{
			clrDirectionCmd();
			SetCoil(_REWERS_PULT, 0);
		}
	}
}

void modePdirStopPrew(void){
	if(testDiscretInp(DISCR_CMD_PIN_1)){
		setNormalStop();
		SetRegister(_REWERS_PULT, 0);
	}else{
		if(testDiscretInp(DISCR_CMD_PIN_2)){
			setRunMode();
			clrDirectionCmd();
			SetCoil(_REWERS_PULT, 0);
		}else{
			if(testDiscretInp(DISCR_CMD_PIN_3)){
				setRunMode();
				setDirectionCmd();
				SetCoil(_REWERS_PULT, 1);
			}
		}
	}
}

void DiscrStartStopCntrl(void){
	switch(getRegister(_MODE_CMD_DIST))
	{
		default:
		case 0:{
			modeSPD();
			break;
		}
		case 1:{
			modePdirStopPrew();
			break;
		}
	}
}

void cntrlStartStopDir(void){
	switch(getRegister(_CNTRL_SOURCE)){
		default:
		case 0:{
			/*���������� �� ������ RS232*/
			if(!getCoil(_RUN_PULT)){
				setNormalStop();
			}else{
				setRunMode();
			}
			if(!getRegister(_DISABLE_REWERS)){
				if(!getCoil(_REWERS_PULT)){
					clrDirectionCmd();
				}else{
					setDirectionCmd();
				}
			}
			break;
		}
		case 1:{
			/*����������� �������*/
			DiscrStartStopCntrl();
			break;
		}
		case 2:{
			/*���������� �� RS485*/
			if(getRegister(_ENABLE_ALARM_STOP)){
				if(getCoil(_ALARM_STOP_CMD)){
					setAlarmStop();
				}
			}
			if((!getCoil(_START_DIRECT)) && (!getCoil(_START_REWERS))){
				setNormalStop();
			}else{
				setRunMode();
			}
			if(!getRegister(_DISABLE_REWERS)){
				if(!getCoil(_START_REWERS)){
					clrDirectionCmd();
				}else{
					setDirectionCmd();
				}
			}
			if(getCoil(_STOP_NORM)){
				setNormalStop();
				SetCoil(_START_DIRECT, 0);
				SetCoil(_START_REWERS, 0);
			}
			break;
		}
	}
	if(testExtAlarmStopPin()){
		SetInpBit(_EXT_ALARM_STOP, 1);
		setAlarmStop();
	}else{
		if((getModeConvertor() != _error_store_fin) &&
		   (getModeConvertor() != _error_convertor))
		SetInpBit(_EXT_ALARM_STOP, 0);
	}
}

unsigned int costyl_control(void)
{
    if(!getRegister(_CNTRL_SOURCE))
    {
        return 0;
    }
    else
    {
        return 1;
    }

}
