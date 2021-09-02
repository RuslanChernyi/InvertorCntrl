/* ********************************************************
 * ��������� ���������� ���. ���������� ��� ���������
 * ********************************************************/

#include "added.h"
#include "AdcCntrl/AdcCntrl.h"
#include "CntrlConvertor.h"
#include "motorCntrl.h"
#include "ModBus/modbus.h"
#include "CntrlLimit/cntrlLimit.h"
#include "DiscretPin/DiscretPin.h"
#include "statusConvertor.h"
#include "cntrlPWM/cntrlPwm.h"
#include "interface/TaskCntrl.h"
#include "IRcompensation.h"
#include "DC_Decelaration.h"
#include "blink/blink.h"

UINT16 cntDelayMC;
INT16 statusRun;
float MaxInvertorFrequency;
float mainTask;

float EconomVoltag, EconomCurrent, EconomDelta;
char EconomSwitch;

INT16 getStatusRun(void){
	return statusRun;
}

void rstCntrlPwm(void){
	mainTask = 0;
	cntDelayMC = 0;
	MaxInvertorFrequency = getMaxInvertorFrequency();
	MaxInvertorFrequency /= 10;
	statusRun = 0;
}

void initEconomParam(void){
	EconomVoltag = getRegister(_ECONOM_VOLTAG);
	if(EconomVoltag > 300)
		EconomVoltag = 0;
	EconomCurrent = getRegister(_ECONOM_I_ON);
	EconomDelta = getRegister(_ECONOM_I_OFF);
	if((EconomVoltag == 0) || (EconomCurrent == 0) || (EconomDelta == 0)){
		EconomSwitch = -1;
		EconomVoltag = 1;
	}else{
		EconomSwitch = 0;
		EconomVoltag = 1000 - EconomVoltag;
		EconomVoltag = EconomVoltag / 1000.0;
		EconomCurrent = EconomCurrent / 10;
		EconomDelta = EconomDelta / 10;
	}
}

char cntrlEconomPwr(float *voltag){
	float current;
	if(EconomSwitch < 0){
		return -1;
	}else{
		current = getAveragIm();
		if(current < EconomCurrent){
			EconomSwitch = 1;
		}else{
			if(current > (EconomCurrent + EconomDelta)){
				EconomSwitch = 0;
			}
		}
		if(EconomSwitch){
			*voltag = (*voltag) * EconomVoltag;
		}
	}
	return EconomSwitch;
}

void cntrlInvOutParam(float *taskF, float *taskV, float *currentF, float *currentV){
	cntrlIrCompensation();
	getMotorFrequency(*taskF, currentF);
	if((!(testOvervoltag())) && (!(testOvercurrent()))){
	    getIqStab(currentF); //стабилизация реактивной составляющей 15.12.16
	}
	getMotorVoltag(*taskF, *currentF, taskV, currentV);
	cntrlEconomPwr(currentV);
	setStepAmpl(*currentV);
	setStepConer(*currentF);
}

void cntrlMotorCntrlPause(float *taskF, float *taskV, float *currentF, float *currentV){
	switch(statusRun){
		default:
			statusRun = 0;
		case 0:
		case 1:
		case 2:{
			rstDecelReg();
			AccelarationPause(mainTask, taskF, currentF);
			break;
		}
		case -1:
		case -2:{
			DecelarationPause(mainTask, taskF, currentF);
			break;
		}
	}
	cntrlInvOutParam(taskF, taskV, currentF, currentV);
}

UINT16 controlPwm(float *taskF, float *taskV, float *currentF, float *currentV){
	INT16 tmp;
	float newTask;
	if(cntDelayMC){
		cntDelayMC--;
		cntrlMotorCntrlPause(taskF, taskV, currentF, currentV);
	}else{
		if(testRunMode())
			newTask = getMainTask(_run_motor);
		else{
			newTask = getMainTask(_stop_motor);
			if(getRegister(_STOP_METHOB) == 1){
			    tmp = DcDecelatration(taskF, currentV);
				*taskF = 0;
			    *currentF = 0;
			    if(tmp>0){
			        return _run_stage_convertor;
			    }else
			        return _stop_stage_convertor;
			}

			if(*taskF/*currentF*/ == 0)
				return _stop_stage_convertor;
		}

		if(newTask != mainTask){
			mainTask = newTask;
			*taskF = *currentF;
		}

		if(mainTask){
			if(mainTask > 0){
				/*�������� �����*/

				if((*taskF >= 0) && (mainTask >= *taskF)){
					/*��������� �����*/
					Accelaration(mainTask, taskF, currentF);
					cntDelayMC = getAccelTime();
				}else{
					Decelaration(mainTask, taskF, currentF);
					cntDelayMC = getDecelTime();
				}

			}else{
				if((*taskF <= 0) && (mainTask <= *taskF)){
					/*�������� ��������*/
					Accelaration(mainTask, taskF, currentF);
					cntDelayMC = getAccelTime();
				}else{
					Decelaration(mainTask, taskF, currentF);
					cntDelayMC = getDecelTime();
				}
			}
			cntrlInvOutParam(taskF, taskV, currentF, currentV);
		}else{
			tmp = DcDecelatration(taskF, currentV);
			if(!tmp){
				*taskF = 0;
				*taskV = 0;
				*currentF = 0;
				*currentV = 0;
			}else{
				cntDelayMC = getDecelTime();
				DecelarationStop(taskF, currentF);
				cntrlInvOutParam(taskF, taskV, currentF, currentV);
			}
		}
	}
//	SET_DELAY(CONV_DL, c1ms);
	return _run_stage_convertor;
}

void getMotorFrequency(float task, float *frequency){
	float tmp;
	tmp = *frequency;
	if(!statusRun){
		if(task > 0){
			tmp += getAddedFrequency();
			if(tmp > MaxInvertorFrequency)
				tmp = MaxInvertorFrequency;

		}else{
			if(tmp){
				tmp -= getAddedFrequency();
				if(tmp < (-MaxInvertorFrequency))
					tmp = -MaxInvertorFrequency;
			}
		}/**/
	}
	*frequency = tmp;
}

float getCorrectionVoltag(float freq){
	float tmp = 0;
//	tmp = getAddedFrequency();
//	tmp = getVoltagFromFrequency(tmp);

	return tmp;
}

void getMotorVoltag(float frequencyInv, float frequencyMotor,
		            float *voltagInv, float *voltagMotor){
	float tmp/*, over*/;
	tmp = getVoltagFromFrequency(frequencyMotor);
	*voltagInv = tmp;

	if(frequencyMotor)
		tmp += getAddedVoltag();

	tmp += getCorrectionVoltag(frequencyMotor);
	tmp = scaleVoltagPwm(tmp);
	stabilisationByUdc(&tmp);


	*voltagMotor = tmp;
}

void Accelaration(float task, float *freqTask, float *freqMotor){
	float stepCheng;
	float tmp/*, over*/;
	stepCheng = getStepAccelCheng();
	tmp = *freqTask;
	if(!(testOvercurrent())){
		if((statusRun != -2)&&(statusRun != 2)){
			if(tmp){
				if(tmp > 0){
					tmp += stepCheng;
					if(tmp > task){
						tmp = task;
					}
				}else{
					tmp -= stepCheng;
					if(tmp < task){
						tmp = task;
					}
				}
			}else{
				if(task > 0){
					tmp += stepCheng;
				}else{
					tmp -= stepCheng;
				}
			}
		}

        if(tmp == task)
            statusRun = 0;
        else
            statusRun = 1;
        *freqTask = tmp;
        *freqMotor = tmp;
        setLedMode(_ADD_LED,_off_led);
	}else{
		AccelarationPause(task, freqTask, freqMotor);
		setLedMode(_ADD_LED,_on_led);
	}
}

void DecelarationStop(float *freqTask, float *freqMotor){
	float stepCheng;
	float tmp;
	stepCheng = getStepDecelCheng();
	tmp = *freqTask;
	if((!(testOvervoltag())) && (!(testOvercurrent()))){
		if((statusRun != -2)&&(statusRun != 2)){
			if(tmp > 0){
				tmp -= stepCheng;
				if(tmp < 0)
					tmp = 0;
			}else{
				tmp += stepCheng;
				if(tmp > 0)
					tmp = 0;
			}
		}

		if(tmp == 0)
			statusRun = 0;
		else
		    statusRun = -1;

		*freqTask = tmp;
		*freqMotor = tmp;
		setLedMode(_ADD_LED,_off_led);
	}else{
		DecelarationPause(0, freqTask, freqMotor);
		setLedMode(_ADD_LED,_on_led);
		statusRun = -1;
	}
}

void Decelaration(float task, float *freqTask, float *freqMotor){
	float stepCheng;
	float tmp;
//	statusRun = -1;
	stepCheng = getStepDecelCheng();
	tmp = *freqTask;
	if((!(testOvervoltag())) && (!(testOvercurrent()))){
		if((statusRun != -2)&&(statusRun != 2)){
			if(tmp > 0){
				tmp -= stepCheng;
				if(tmp < task){
					tmp = task;
				}
			}else{
				tmp += stepCheng;
				if(tmp > task){
					tmp = task;
				}
			}
		}

		if(tmp == task){
			statusRun = 0;
		}else{
			statusRun = -1;
		}

		*freqTask = tmp;
		*freqMotor = tmp;
		setLedMode(_ADD_LED,_off_led);
	}else{
		statusRun = -1;
		DecelarationPause(task, freqTask, freqMotor);
		setLedMode(_ADD_LED,_on_led);
	}
}

void AccelarationPause(float task, float *freqTask, float *freqMotor){
	float tmp, over;
	tmp = *freqTask;
	if(testOvercurrent()){
	    over = getOverloadFrequency();
        if(over)
		if(tmp > 0){
			tmp -= over;
			if(tmp < 0)
				tmp = 0;
		}else{
			if(tmp)
				tmp += over;
			if(tmp > 0)
				tmp = 0;
		}
        statusRun = 2;
		*freqTask = tmp;//12.07.17
	}
	*freqMotor = tmp;
}

void DecelarationPause(float task, float *freqTask, float *freqMotor){
	float tmp, over;
	tmp = *freqTask;
//	over *= tmp / 100;
	if(testOvervoltag()){
	    over = getOvervoltagFrequency();
		if(tmp > 0){
			tmp += over;
			if(tmp > MaxInvertorFrequency)
				tmp = MaxInvertorFrequency;
		}else{
			if(tmp)
				tmp -= over;
			if(tmp < -MaxInvertorFrequency)
				tmp = -MaxInvertorFrequency;
		}
		statusRun = -2;
	}
	*freqMotor = tmp;
}
