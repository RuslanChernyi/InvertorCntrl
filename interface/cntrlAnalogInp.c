/*
 * ���������� ������� ���������� �������� ����������
 */

#include "added.h"
#include "AdcCntrl/AdcCntrl.h"
#include "ModBus/modbus.h"

#define _ZERRO_VOLTAG 15
#define _MAX_VOLTAG_TASK 3000
#define _20mA_VAL 3000
#define _4mA_VAL 500
#define _ZERRO_CURR 7
#define _MAX_CURR_CURR 2000

float getAnalogTaskInp(void){
	float tmp;
	UINT16 val;
	val = getAveregResult(_TASK_ANALOG_VAL);
	switch(getRegister(_ANALOG_INP_MODE)){
		default:
		case 0:{
			//�������������� 0-10B
			if(val < _ZERRO_VOLTAG)
				val = 0;
			if(val > _MAX_VOLTAG_TASK)
				val = _MAX_VOLTAG_TASK;
			tmp = 100;
			tmp *= val;
			tmp = tmp / _MAX_VOLTAG_TASK;
			break;
		}
		case 1:{
			//�������������� 4-20mA
			if(val < _4mA_VAL)
				val = 0;
			else{
				if(val > _20mA_VAL)
					val = _20mA_VAL;
				val -= _4mA_VAL;
			}
			tmp = 100;
			tmp *= val;
			tmp = tmp / (_20mA_VAL - _4mA_VAL);
			break;
		}
		case 2:{
			//�������������� 0-10mA
			if(val < _ZERRO_CURR)
				val = 0;
			if(val > _MAX_CURR_CURR)
				val = _MAX_CURR_CURR;
			tmp = 100;
			tmp *= val;
			tmp = tmp / _MAX_CURR_CURR;
			break;
		}
	}
	return tmp;
}

float getAnalogOSInp(void){
	float tmp;
	UINT16 val;
	val = getAveregResult(_OS_INP_VAL);
	switch(getRegister(_MODE_SIGN_OS)){
		default:
		case 0:{
			//�������������� 0-10B
			if(val < _ZERRO_VOLTAG)
				val = 0;
			if(val > _MAX_VOLTAG_TASK)
				val = _MAX_VOLTAG_TASK;
			tmp = 100;
			tmp *= val;
			tmp = tmp / _MAX_VOLTAG_TASK;
			break;
		}
		case 1:{
			//�������������� 4-20mA
			if(val < _4mA_VAL)
				val = 0;
			else{
				if(val > _20mA_VAL)
					val = _20mA_VAL;
				val -= _4mA_VAL;
			}
			tmp = 100;
			tmp *= val;
			tmp = tmp / (_20mA_VAL - _4mA_VAL);
			break;
		}
		case 2:{
			//�������������� 0-10mA
			if(val < _ZERRO_CURR)
				val = 0;
			if(val > _MAX_CURR_CURR)
				val = _MAX_CURR_CURR;
			tmp = 100;
			tmp *= val;
			tmp = tmp / _MAX_CURR_CURR;
			break;
		}
	}
	return tmp;
}
