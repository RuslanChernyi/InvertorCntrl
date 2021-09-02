/*
 * Regulyator.c
 *
 *  Created on: 12 июл. 2017 г.
 *      Author: test
 */

#include <stddef.h>
#include "Regulator.h"

void clrReg(pRegData reg){
    reg->DReg = 0;
    reg->IReg = 0;
    reg->PReg = 0;
    reg->Im1 = 0;
    reg->Dm1 = 0;
    reg->reg = 0;
    reg->Em1 = 0;
    reg->E = 0;
}

INT8 setKoefReg(float KP, float KI, float KD, float deltaT, pRegData reg){
    float tmp;
    if(reg == NULL)
        return -1;
    reg->Kp = KP;
    reg->Ki = KI;
    if(KD){
        tmp = KD * deltaT;
        reg->Kd1 = tmp / (tmp + (_N_DifReg * deltaT));
        reg->Kd2 = (tmp *_N_DifReg) / (tmp + (_N_DifReg * deltaT));

        return 2;
    }else{
        reg->Kd1 = 0;
        reg->Kd2 = 0;
    }

    return 1;
}

INT8 setLimReg(float min, float max, pRegData reg){
    if(reg == NULL)
        return -1;
    if(min > max){
        reg->max = 0;
        reg->min = 0;
        return -2;
    }
    reg->max = max;
    reg->min = min;
    return 1;
}


void cntrlP(pRegData reg){
    reg->PReg = reg->E * reg->Kp;
}

void cntrlI(pRegData reg){
    float tmp;

    tmp = reg->Im1 + reg->Ki * reg->E;
    reg->Im1 = reg->IReg;

    if(tmp > reg->max)
        tmp = reg->max;
    if(tmp < reg->min)
        tmp = reg->min;

    reg->IReg = tmp;
}

void cntrlD(pRegData reg){
    float tmp;

    tmp = reg->Kd1 * reg->Dm1;
    tmp += reg->Kd2 * (reg->E - reg->Em1);
    reg->Dm1 = reg->DReg;

    if(tmp > reg->max)
        tmp = reg->max;
    if(tmp < reg->min)
        tmp = reg->min;

    reg->DReg = tmp;
}

void cntrlRegulyatorPID(float e, pRegData reg){
    float tmp;
    reg->Em1 = reg->E;
    reg->E = e;
    cntrlP(reg);
    cntrlI(reg);
    cntrlD(reg);
    tmp = reg->IReg + reg->PReg + reg->DReg;

    if(tmp > reg->max)
        tmp = reg->max;
    if(tmp < reg->min)
        tmp = reg->min;

    reg->reg = tmp;
}

void cntrlRegulyatorPI(float e, pRegData reg){
    float tmp;
    reg->Em1 = reg->E;
    reg->E = e;
    cntrlP(reg);
    cntrlI(reg);
    tmp = reg->IReg + reg->PReg;

    if(tmp > reg->max)
        tmp = reg->max;
    if(tmp < reg->min)
        tmp = reg->min;

    reg->reg = tmp;
}

void cntrlRegulyatorP(float e, pRegData reg){
    reg->Em1 = reg->E;
    reg->E = e;
    cntrlP(reg);
    reg->reg = reg->PReg;
}

void cntrlRegulyatorI(float e, pRegData reg){
    reg->Em1 = reg->E;
    reg->E = e;
    cntrlI(reg);
    reg->reg = reg->IReg;
}

void cntrlRegulyatorD(float e, pRegData reg){
    reg->Em1 = reg->E;
    reg->E = e;
    cntrlD(reg);
    reg->reg = reg->DReg;
}

