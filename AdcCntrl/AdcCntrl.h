/* **********************************************
 * ��������� ������ ����������� ADC modul
 * **********************************************/

#ifndef ___ADC_CNTRL_H___
#define ___ADC_CNTRL_H___

#include "added.h"
#include "OutCurrentCalc.h"

#ifdef __CORCK_ADC

//#define setZerroShiftAdc() /**/
#define getLoadCurrent() 0 /**/

#else

#define getLoadCurrent() getImVal()

#endif

void setePWM_AdcCntrl(void);
void initADC(void);
void cntrlAdcModul(void);
void cntrlIntegreteADC( void );
UINT16 getMomenteryResult(UINT8 res);
UINT16 getAveregResult(UINT8 res);
void cntrlAdcModulZerro(void);
void cntrlAdcModulOne(void);

#define _NET_CH 7
#define getNetAdc() getMomenteryResult(_NET_CH)

#ifdef __CORCK_ADC_UD

#define getMiddleVoltagDC() 540.0 /**/
#define getVoltagDC() 540.0

#else

//#define getMiddleVoltagDC() getIntegrVoltagDC()
#define getVoltagDC() getMomentaryVoltagDC()
#define getMiddleVoltagDC() getAverageVoltagDC()

#endif

#define _CURRENT_A_VAL 0
#ifndef __HIGH_POWER
#define _CURRENT_B_VAL 1
#else
#define _CURRENT_B_VAL 6
#endif
#define _VOLTAG_DC_VAL 2
#define _TASK_ANALOG_VAL 3
#define _OS_INP_VAL 4
#define _Temperature_VAL 5

void initMessureHD(void);
void cntrlAdcMessure(void);
void initMessureMsht(void);
void initMessure(void);
float getMomentaryCurrentA(void);
float getMomentaryCurrentB(void);
float getMomentaryVoltagDC(void);
float getIntegrVoltagDC(void);
UINT16 presetAdc(char cmd);
float getTemperature(void);
/*void cntrlZerroShift(void);*/
void cntrlZerroShift(UINT16 zerro);

void initAverageMsr(void);
UINT16 getAveragDl(void);
void cntrlAverageMsr(void);
float getAverageVoltagDC(void);
float getRmsCurrent(UINT8 cur);

#define getRmsCurrentA() getRmsCurrent(1)
#define getRmsCurrentB() getRmsCurrent(0)

float getAveragIm(void);
float getAveragId(void);
float getAveragIq(void);

float getMaxVoltagDC(void);

float getOutToDcCurrent(void);

void cntrlNetMessure(void);
float getNetVoltag(void);
void initNetMessure(void);
void initNetCntrl(void);
UINT8 testNetMax(void);
UINT8 testNetMin(void);

#endif
