/*
 * cntrlPidSimple.c
 *
 *  Created on: 18 ����. 2014
 *      Author: Tester
 *
 *  ���������� ������� �����������
 *
 */

#include "added.h"
#include "OsCntrl.h"
#include "cntrlPidSimple.h"
#include "ConvertorCntrl/CntrlConvertor.h"
#include "ConvertorCntrl/statusConvertor.h"
#include "ModBus/modbus.h"

UINT16 stateSimplePid;

void cntrlPidSimple(void){
	float tmp, tmp1;
	switch(stateSimplePid){
		default:
		case _off_simple_pid:{
			tmp = getRegister(_F_MIN_OS);
			tmp1 = getRegister(_F_MAX_OS);
			tmp /= 10;
			tmp1 /= 10;
			setLimitOs(tmp1, tmp);
			initPIDregulator();
			if(testRunMode()){
				stateSimplePid = _start_simple_pid;
				setFtaskOs(tmp);
			}else{
				setFtaskOs(0);
			}

			break;
		}
		case _start_simple_pid:{
			tmp1 = getFtaskOs();
			tmp = getInvertorFreq();
			if(tmp >= tmp1){
				stateSimplePid = _run_simple_pid;
			    setPidOs(tmp);
			}
			break;
		}
		case _run_simple_pid:{
			if(testRunMode()){
				cntrlPIDregulator();
				setFtaskOs(getPidOs());
			}else{
				stateSimplePid = _stop_simple_pid;
				tmp = getRegister(_F_MIN_OS);
				tmp /= 10;
				setFtaskOs(tmp);
			}
			break;
		}
		case _stop_simple_pid:{
			tmp1 = getRegister(_F_MIN_OS);
			tmp1 /= 10;
			tmp = getInvertorFreq();
			if(tmp <= tmp1){
				stateSimplePid = _off_simple_pid;
			}
			break;
		}
	}
}

void initPidCntrlSimple(void){
	stateSimplePid = _off_simple_pid;
}
