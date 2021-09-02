/* ********************************************************
 * ���������� ���������� ��������������� �������
 * ********************************************************/

#ifndef ___CNTRL_CONVERTOR_H___
#define ___CNTRL_CONVERTOR_H___

#include "added.h"
#include "SysTimer/sysTime.h"
#include "interface/TaskCntrl.h"

#define _off_convertor 0x00
#define _stop_convertor 0x01
#define _charg_convertor 0x02
#define _run_convertor 0x04
#define _job_convertor 0x08
#define _error_convertor 0x010
#define _error_store_fin 0x011
#define _postrun_pause_stop	18

#define _chek_off 0
#define _chek_redy_conv 1

#define _chek_stop 2
#define _chek_cmd_conv 3
#define _presetup_conv 4

#define _start_charg 5
#define _check_charge_conv 6
#define _on_invertor 7
#define _on_main_pwr 8

#define __pre_run_stage 9
#define _soft_start_convertor 10
#define _cold_start_convertor 11
#define _run_stage_convertor 12
#define _stop_stage_convertor 0x0d

UINT16 getCntrlOffConvertor(UINT16 *mode);
UINT16 getCntrlStopConvertor(UINT16 *mode);
UINT16 getCntrlChargConvertor(UINT16 *mode);
UINT16 getCntrlRunConvertor(UINT16 *mode);
UINT16 getCntrlErrorConvertorState(UINT16 *mode);

void initCntrlConvertor(void);

void refreshRegisterMbus(void);

UINT16 getModeConvertor(void);
void cntrlConvertor(void);

UINT16 controlPwm(float *taskF, float *taskV,
		          float *currentF, float *currentV);

//float getMainTask(char mode);

float getCurrentFreq(void);
float getCurrentVinv(void);
float getInvertorFreq(void);
float getInvertorVinv(void);

void clrConvertorRunData(void);
void rstCntrlPwm(void);

void setFlagPwmSet(void);
UINT8 getFlagPwmSet(void);
void cntrlMotorCntrlPause(float *taskF, float *taskV, float *currentF, float *currentV);

INT16 getStatusRun(void);
void initIqStab(void);
void getIqStab(float *outF);

DECKLARATE_DELAY(CONV_DL)

#define __CORC_SOFT_SATRT

UINT8 testRunState(void);
UINT16 refrErrorBits(void);

#endif
