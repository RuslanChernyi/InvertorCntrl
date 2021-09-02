/*
 * taskCntrl.c
 *
 *  Created on: 16 ����. 2014
 *      Author: Tester
 */

#include "added.h"
#include "AdcCntrl/AdcCntrl.h"
#include "ModBus/modbus.h"
#include "DiscretPin/DiscretPin.h"
#include "ConvertorCntrl/statusConvertor.h"
#include "TaskCntrl.h"
#include "AnalogInput/AnalogInput.h"
#include "OsCntrl/OsCntrl.h"

float TaskAct = 0;
UINT16 ModeTaskMain;

float getTaskAct(void){
	return TaskAct;
}


float getDiscrTask(void){
	float tmp;
	char pin;

	switch(getRegister(_MODE_TASK_DISCR_IN)){
		default:
		case 0:
		{
			if(testDiscretInp(DISCR_INP_PIN_1)){
				tmp = getRegister(_TASK_D1);
				tmp = tmp /100;
				break;
			}
			if(testDiscretInp(DISCR_INP_PIN_2)){
				tmp = getRegister(_TASK_D2);
				tmp = tmp /100;
				break;
			}
			if(testDiscretInp(DISCR_INP_PIN_3)){
				tmp = getRegister(_TASK_D3);
				tmp = tmp /100;
				break;
			}
			tmp = getRegister(_TASK_D0);
			tmp = tmp /100;
			break;
		}
		case 1:{
			pin = testDiscretInp(DISCR_INP_PIN_3);
			pin <<= 1;
			pin |= testDiscretInp(DISCR_INP_PIN_2);
			pin <<= 1;
			pin |= testDiscretInp(DISCR_INP_PIN_1);

			switch(pin)
			{
				default:
				case 0:{
					tmp = getRegister(_TASK_D0);
					break;}
				case 1:{
					tmp = getRegister(_TASK_D1);
					break;}
				case 2:{
					tmp = getRegister(_TASK_D2);
					break;}
				case 3:{
					tmp = getRegister(_TASK_D3);
					break;}
				case 4:{
					tmp = getRegister(_TASK_D4);
					break;}
				case 5:{
					tmp = getRegister(_TASK_D5);
					break;}
				case 6:{
					tmp = getRegister(_TASK_D6);
					break;}
				case 7:{
					tmp = getRegister(_TASK_D7);
					break;}
			}
			tmp = tmp /100;

			break;
		}
	}
	if(testDirectionCmd())
		tmp = -tmp;
	return tmp;
}

float getMainTask(char mode){
    float lim;
	if(mode){
	    lim = getRegister(_MIN_INV_TASK);
	    lim = lim / 10;
		switch(ModeTaskMain){
			default:
			case 0:{
				TaskAct = getRegister(_PULT_TASK);
				TaskAct = TaskAct /100;
				if(TaskAct < lim)
				    TaskAct = lim;
				if(testDirectionCmd())
					TaskAct = -TaskAct;
				break;
			}
			case 1:{
				TaskAct = getAnalogTask();
				break;
			}
			case 2:{ //2
				TaskAct = getDiscrTask();
				break;
			}
			case 3:{
				TaskAct = getRegister(_MAIN_TASK);
				TaskAct = TaskAct /100;
				if(TaskAct < lim)
                    TaskAct = lim;
				if(testDirectionCmd())
					TaskAct = -TaskAct;
				break;
			}
			case 4:{//4
			                TaskAct = getRegister(_REMOTE_TASK);
			                TaskAct = TaskAct /100;
			                if(TaskAct < lim)
			                    TaskAct = lim;
			                if(testDirectionCmd())
			                    TaskAct = -TaskAct;
			                break;
			            }
			case 0x010:{	/*����� �� ���������� ��*/
				TaskAct = getFtaskOs();
				break;
			}
		}
		SetInpReg(_ACTIVE_F_TASK, (INT16)(TaskAct*100));
	}else{
		if(ModeTaskMain == 0x010)/*����� �� ���������� ��*/
			TaskAct = getFtaskOs();
		else
			TaskAct = 0;
	}

	return TaskAct;
}

void initTaskMain(void){


        ModeTaskMain = getRegister(_TASK_SOURCE);
         if(getRegister(_MODE_OS_PID)){
             ModeTaskMain = 0x10;


	}
}

