/*
 * KaskadPin.c
 *
 *  Created on: 7 нояб. 2017 г.
 *      Author: test
 *
 *  управление пуском дополнительных ступеней
 *
 */

#include "added.h"
#include "ModBus/modbus.h"
#include "DSP2833x_Device.h"
#include "KaskadOs.h"

#define setReleK1() GpioDataRegs.GPBSET.bit.GPIO60=1;
#define setReleK2() GpioDataRegs.GPCSET.bit.GPIO64=1;
#define setReleK3() GpioDataRegs.GPBSET.bit.GPIO61=1;
#define setReleK4() GpioDataRegs.GPBSET.bit.GPIO59=1;
#define clrReleK1() GpioDataRegs.GPBCLEAR.bit.GPIO60=1;
#define clrReleK2() GpioDataRegs.GPCCLEAR.bit.GPIO64=1;
#define clrReleK3() GpioDataRegs.GPBCLEAR.bit.GPIO61=1;
#define clrReleK4() GpioDataRegs.GPBCLEAR.bit.GPIO59=1;

void onStage1(void){
    if((getRegister(_MODE_KASKAD)) && (tstPinKaskad()&0x01)){
            clrReleK1();
            setReleK2();
            clrReleK3();
            clrReleK4();
    }else{
        setReleK1();
        clrReleK2();
        clrReleK3();
        clrReleK4();
    }
}

void onStage2(void){
    if(getRegister(_MODE_KASKAD)){
        switch(tstPinKaskad()){
        default:
        case 1:{
            clrReleK1();
            setReleK2();
            setReleK3();
            clrReleK4();
            break;
        }
        case 2:{
            setReleK1();
            clrReleK2();
            setReleK3();
            clrReleK4();
            break;
        }
        case 8: ;
        case 4:{
            setReleK1();
            setReleK2();
            clrReleK3();
            clrReleK4();
            break;
        }
        }
    }else{
        setReleK1();
        setReleK2();
        clrReleK3();
        clrReleK4();
    }
}

void onStage3(void){
    if(getRegister(_MODE_KASKAD)){
        switch(tstPinKaskad()){
        default:
        case 1:{
            clrReleK1();
            setReleK2();
            setReleK3();
            setReleK4();
            break;
        }
        case 2:{
            setReleK1();
            clrReleK2();
            setReleK3();
            setReleK4();
            break;
        }
        case 4:{
            setReleK1();
            setReleK2();
            clrReleK3();
            setReleK4();
            break;
        }
        case 8:{
            setReleK1();
            setReleK2();
            setReleK3();
            clrReleK4();
            break;
        }
        }
    }else{
        setReleK1();
        setReleK2();
        setReleK3();
        clrReleK4();
    }
}

void onStage4(void){
    if(!getRegister(_MODE_KASKAD)){
        setReleK1();
        setReleK2();
        setReleK3();
        setReleK4();
    }
}

void offStage(void){
    clrReleK1();
    clrReleK2();
    clrReleK3();
    clrReleK4();
}

void initStagePin(void){
    EALLOW;

    clrReleK4();   // Load output latch
    clrReleK1();   // Load output latch
    clrReleK3();   // Load output latch
    clrReleK2();   // Load output latch
    // Enable an GPIO output on GPIOx, set it high
    GpioCtrlRegs.GPBPUD.bit.GPIO59 = 0;   // Enable pullup on GPIOx
    GpioCtrlRegs.GPBMUX2.bit.GPIO59 = 0;  // GPIOx = GPIOx
    GpioCtrlRegs.GPBDIR.bit.GPIO59 = 1;   // GPIOx = output

    // Enable an GPIO output on GPIOx, set it high
    GpioCtrlRegs.GPBPUD.bit.GPIO60 = 0;   // Enable pullup on GPIOx
    GpioCtrlRegs.GPBMUX2.bit.GPIO60 = 0;  // GPIOx = GPIOx
    GpioCtrlRegs.GPBDIR.bit.GPIO60 = 1;   // GPIOx = output

    // Enable an GPIO output on GPIOx, set it high
    GpioCtrlRegs.GPBPUD.bit.GPIO61 = 0;   // Enable pullup on GPIOx
    GpioCtrlRegs.GPBMUX2.bit.GPIO61 = 0;  // GPIOx = GPIOx
    GpioCtrlRegs.GPBDIR.bit.GPIO61 = 1;   // GPIOx = output
    // Enable an GPIO output on GPIOx, set it high

    GpioCtrlRegs.GPCPUD.bit.GPIO64 = 0;   // Enable pullup on GPIOx
    GpioCtrlRegs.GPCMUX1.bit.GPIO64 = 0;  // GPIOx = GPIOx
    GpioCtrlRegs.GPCDIR.bit.GPIO64 = 1;   // GPIOx = output

    // Enable an GPIO input on GPIOx, set it high
    GpioCtrlRegs.GPCPUD.bit.GPIO69 = 0;   // Enable pullup on GPIOx
    GpioDataRegs.GPCCLEAR.bit.GPIO69 = 1;   // Load output latch
    GpioCtrlRegs.GPCMUX1.bit.GPIO69 = 0;  // GPIOx = GPIOx
    GpioCtrlRegs.GPCDIR.bit.GPIO69 = 0;   // GPIOx = input

    // Enable an GPIO input on GPIOx, set it high
    GpioCtrlRegs.GPCPUD.bit.GPIO82 = 0;   // Enable pullup on GPIOx
    GpioDataRegs.GPCCLEAR.bit.GPIO82 = 1;   // Load output latch
    GpioCtrlRegs.GPCMUX2.bit.GPIO82 = 0;  // GPIOx = GPIOx
    GpioCtrlRegs.GPCDIR.bit.GPIO82 = 0;   // GPIOx = input

    // Enable an GPIO input on GPIOx, set it high
    GpioCtrlRegs.GPCPUD.bit.GPIO81 = 0;   // Enable pullup on GPIOx
    GpioDataRegs.GPCCLEAR.bit.GPIO81 = 1;   // Load output latch
    GpioCtrlRegs.GPCMUX2.bit.GPIO81 = 0;  // GPIOx = GPIOx
    GpioCtrlRegs.GPCDIR.bit.GPIO81 = 0;   // GPIOx = input

    // Enable an GPIO input on GPIOx, set it high
    GpioCtrlRegs.GPCPUD.bit.GPIO80 = 0;   // Enable pullup on GPIOx
    GpioDataRegs.GPCCLEAR.bit.GPIO80 = 1;   // Load output latch
    GpioCtrlRegs.GPCMUX2.bit.GPIO80 = 0;  // GPIOx = GPIOx
    GpioCtrlRegs.GPCDIR.bit.GPIO80 = 0;   // GPIOx = input

    EDIS;
}

UINT8 tstPinKaskad1(void){
    if(GpioDataRegs.GPCDAT.bit.GPIO69)
        return 0;
    else
        return 1;
}


UINT8 tstPinKaskad2(void){
    if(GpioDataRegs.GPCDAT.bit.GPIO80)
        return 0;
    else
        return 1;
}

UINT8 tstPinKaskad3(void){
    if(GpioDataRegs.GPCDAT.bit.GPIO81)
        return 0;
    else
        return 1;
}

UINT8 tstPinKaskad4(void){
    if(GpioDataRegs.GPCDAT.bit.GPIO82)
        return 0;
    else
        return 1;
}

UINT8 tstPinKaskad(void){
    UINT8 tmp;
    tmp = tstPinKaskad4();
    tmp<<=1;
    tmp |= tstPinKaskad3();
    tmp<<=1;
    tmp |= tstPinKaskad2();
    tmp<<=1;
    tmp |= tstPinKaskad1();
    return tmp;
}

void cntrlAddedKaskadStage(UINT8 stage){
    UINT16 max;
    max = getRegister(_NUM_KASKAD);
    if(stage>max)
        stage = max;
    switch(stage){
    case 4:{
        onStage4();
        break;
    }
    case 3:{;
        onStage3();
        break;
    }
    case 2:{
        onStage2();
        break;
    }
    case 1:{
        onStage1();
        break;
    }
    default:
    case 0:{
        offStage();
    }
    }
}
