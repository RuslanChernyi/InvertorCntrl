/*
 * Regulator.h
 *
 *  Created on: 12 июл. 2017 г.
 *      Author: test
 */

#ifndef CNTRLLIMIT_REGULATOR_H_
#define CNTRLLIMIT_REGULATOR_H_

#include "added.h"

typedef struct _REG_STR{
    float Kp, Ki, Kd1, Kd2; // коефф. регулирования
    float PReg, IReg, DReg; // текущие значения регуляторов
    float Im1, Dm1;    // прошлые значения регуляторов
    float E, Em1;   // значения текущей и прощлой ошибки рассогласования
    float min, max; // лимиты регулятора
    float reg;  // знвчение регулятора
}stRegData, *pRegData;

#define _N_DifReg 3
        /*определяется частотой фильтрации диф.регулятора обычно 2...20*/

void clrReg(pRegData reg); // preset reg

/*
 * установка коефф. регулятора
 * KP - пропорциональный
 * KI - deltaT / tauI
 * KD - tauD / deltaT
 * deltaT - дискретность вычисления регулятора;
 * reg - параметры регулятора
 */
INT8 setKoefReg(float KP, float KI, float KD, float deltaT, pRegData reg);

/*
 * установка пределов регулятора
 */
INT8 setLimReg(float min, float max, pRegData reg);

/*
 * тело регулятора.
 */
void cntrlRegulyatorPID(float e, pRegData reg);
void cntrlRegulyatorPI(float e, pRegData reg);
void cntrlRegulyatorP(float e, pRegData reg);
void cntrlRegulyatorI(float e, pRegData reg);
void cntrlRegulyatorD(float e, pRegData reg);

#endif /* CNTRLLIMIT_REGULATOR_H_ */
