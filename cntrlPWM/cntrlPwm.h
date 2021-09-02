/* ******************************************************************
 *
 * ******************************************************************/

#ifndef ___CNTRL_PWM_H___
#define ___CNTRL_PWM_H___

#include "added.h"
#include "PWM.h"

#define c180dgr 0x07fff
#define c180dgrBy32 0x07fffffff
#define c60dgr (c180dgr/3)
#define c90dgr (c180dgr/2)
#define c120dgr (2*(c180dgr/3))

#define c2bySqrt3 1.1547
#define mshtTime 0x01fff

UINT16 getCurrentConer(void);
void presetPwmTime(/*UINT16 pwm, */UINT16 pwr);
void setStepConer(float pwr);
void setStepAmpl(float pwr);
void rotateVectorVout(void);
void getTau(UINT16 coner, float* TA, float *TB);
void getTimePWM(float TA, float TB, UINT16 *time);
void getPhaseTimePWM(UINT16 *time);
void rstPwmCntrl(void);
void enableCntrlPwm(void);
INT16 testStepDirection(void);
void setPwmConer(void);
float getAmplVout(void);

#ifdef __CORCK_PWM

#define rstErrorInvertor() /**/

#endif

#endif
