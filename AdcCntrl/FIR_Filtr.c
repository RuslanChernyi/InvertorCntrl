/*
 * FIR_Filtr.c
 *
 *  Created on: 02 нояб. 2016 г.
 *      Author: test
 *
 *
Filter type: Low Pass
Filter model: Butterworth
Filter order: 2
Sampling Frequency: 3 KHz
Cut Frequency: 0.050000 KHz
Coefficents Quantization: 16-bit
 *
 *
 */

#include "DSP2833x_Device.h"
#include "AdcCntrl.h"
#include "FIR_Filtr.h"

const Int16 FIRCoef[Ntap] = {
        11913,
        13234,
        14652,
        16168,
        17786,
        19506,
        21331,
        22269,
        21331,
        19506,
        17786,
        16168,
        14652,
        13234,
        11913,
        10686
    };

Int16 getFiltrVal(Int16* array, Int16 val){
    Int16 *x; //input samples
    Int32 y=0;            //output sample
    Int16 n;
    x = array;

    //shift the old samples
    for(n=Ntap-1; n>0; n--)
       x[n] = x[n-1];

    //Calculate the new output
    x[0] = val;
    for(n=0; n<Ntap; n++)
        y += (Int32)FIRCoef[n] * x[n];

    return y / DCgain;
}
