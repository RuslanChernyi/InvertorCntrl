/*
 * RectifireCntrl.h
 *
 *  Created on: 19 дек. 2016 г.
 *      Author: test
 */

#ifndef RECTIFIRECNTRL_RECTIFIRECNTRL_H_
#define RECTIFIRECNTRL_RECTIFIRECNTRL_H_

#include "added.h"

void initRectifire(void);
void setConer(int coner);
void offRectifire(void);
void cntrlIO_PIN_Sifu(char state);

void cntrlFiitrSifu(void);
void cntrlPulseSifu(void);

#define _MaxConer 255

#define _NORMA_SINCHRO 0x01
#define _NORMA_PHASE 0x02

void initSinchroTest(void);
void waitSinchroPulse(void);
void testSinchroPulse(void);
char getTestSinchro(void);

#define fullOpenRect() cntrlIO_PIN_Sifu(1)
#define fullCloseRect() cntrlIO_PIN_Sifu(0)

#endif /* RECTIFIRECNTRL_RECTIFIRECNTRL_H_ */
