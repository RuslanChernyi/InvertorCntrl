/*
 * ���������� �� ���������� �������� �����
 */

#ifndef ___OS_CNTRL_H___
#define ___OS_CNTRL_H___

#include "SysTimer/sysTime.h"
#include "added.h"

DECKLARATE_DELAY(PID_DL)

#define _OVERREG_SIMPLE_REG 0x020
#define _UNDERREG_SIMPLE_REG 0x010

#define _alrm_dry_run 0x0100
#define _alrm_eor_run 0x0200
#define _down_suturetion_run 0x0400
#define _up_suturetion_run 0x0800

void setFtaskOs(float task);
float getFtaskOs(void);

void initOsPid(char);
void cntrlOsPid(void);
void noPidOs(void);
void simplePidOs(void);
void waterPidOs(void);
void KaskadPidOs(void);
UINT16 getStatusOs(void);
void setStatusOs(UINT16 stat);
void clrStatusOs(UINT16 stat);
float getNormaOs(void);

float getPidOs(void);
float setPidOs(float val);
void cntrlPIDregulator(void);
void initPIDregulator(void);
void setLimitOs(float max, float min);
void setAtomPidDelay(UINT16 delay);
void clrPIDCalc(void);
void initPIDmplr(void);

float getDeltaError(void);
float getDeltaKaskad(void);
INT8 getStateKaskadAdd(void);
void reinitKaskadOs(void);

INT8 testOsKHN(void);

#endif
