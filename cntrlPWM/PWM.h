/* ******************************************************************
 * ���������� ���������� PWM
 * ******************************************************************/

#ifndef ___PWM_H___
#define ___PWM_H___

#include "added.h"

void InitEPwmGpio(void);
void initPWM(void);
void setPwmDuty(UINT16 *PwmDuty);
void setPwmPriod(UINT16 prd);
interrupt void cpu_pwm_prd_isr(void);
void cntrlIO_PIN(char state);

void enablePwmInt(void);
void disablePwmInt(void);
char testAllDriverError(void);
UINT16 getTimePwm(UINT16 phase);
UINT16 getPeriodPWM(void);

#define getFixPeriodPWM() 8000
#define getDeadTime() 255
#define getPwm_1500Hz() 16000
#define getPwm_4500Hz() 5333
#define getPwm_6000Hz() 4000
#define getPwm_8000Hz() 3000

#ifdef __MIDDLE_POWER

#define __LOW_ACKT /*1 - ��� ������� */

#endif

#ifdef __HIGH_POWER
#define __HI_ACKT /*1 - ��� ������� */
#endif

void initBreak(void);
void rstBreak(void);
void cntrlBreak(void);

#endif
