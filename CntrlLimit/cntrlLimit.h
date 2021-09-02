/* ********************************************************
 * ��������� �������� ���� ��������
 * ********************************************************/

#ifndef ___CNTRL_CURRENT_H___
#define ___CNTRL_CURRENT_H___

void cntrlOverload(void);
void initOverloadCntrl(void);
float getOverloadFrequency(void);
UINT8 testOvercurrent(void);
UINT8 testLimitCurrent(void);
float getCurrentLimit(void);

void initOverVoltKoef(void);
void initOvervoltCntrl(void);
void initKoefOverload(void);
void cntrlOvervoltag(void);
float getOvervoltagFrequency(void);
UINT8 testOvervoltag(void);
INT8 testLimitVoltag(void);
INT8 testUnderVoltag(void);

float getIbyT(void);
float getSqrIbyT(void);
void initIbyTParam(void);
void initIbyTLimit(void);
void cntrlIbyT(void);
UINT8 testIbyT(void);
float getValIbyT(void);

void stabilisationByUdc(float *voltag);

void cntrlCooler(void);
void initCoolerCntrl(void);

void cntrlBrokePhase(void);
void initBrokenPhaseCntrl(void);

#endif
