/*
 * пїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ
 */
#include "ModBus/Registers.h"
#include "added.h"
#include "DSP2833x_Device.h"
extern unsigned int inc_ctr;
extern unsigned int dec_ctr;
unsigned int ready=0;
void initDiscrInput(void){

    EALLOW;

    GpioCtrlRegs.GPBCTRL.bit.QUALPRD2 = 0x0ff;
    GpioCtrlRegs.GPBCTRL.bit.QUALPRD3 = 0x0ff;

    // Enable an GPIO output on GPIOx, set it high
    GpioCtrlRegs.GPBPUD.bit.GPIO52 = 0;   // Enable pullup on GPIOx
    GpioDataRegs.GPBCLEAR.bit.GPIO52 = 1;   // Load output latch
    GpioCtrlRegs.GPBMUX2.bit.GPIO52 = 0;  // GPIOx = GPIOx
    GpioCtrlRegs.GPBQSEL2.bit.GPIO52 = 2;
    GpioCtrlRegs.GPBDIR.bit.GPIO52 = 0;   // GPIOx = input

    // Enable an GPIO output on GPIOx, set it high
    GpioCtrlRegs.GPBPUD.bit.GPIO53 = 0;   // Enable pullup on GPIOx
    GpioDataRegs.GPBCLEAR.bit.GPIO53 = 1;   // Load output latch
    GpioCtrlRegs.GPBMUX2.bit.GPIO53 = 0;  // GPIOx = GPIOx
    GpioCtrlRegs.GPBQSEL2.bit.GPIO53 = 2;
    GpioCtrlRegs.GPBDIR.bit.GPIO53 = 0;   // GPIOx = input

    // Enable an GPIO output on GPIOx, set it high
    GpioCtrlRegs.GPBPUD.bit.GPIO54 = 0;   // Enable pullup on GPIOx
    GpioDataRegs.GPBCLEAR.bit.GPIO54 = 1;   // Load output latch
    GpioCtrlRegs.GPBMUX2.bit.GPIO54 = 0;  // GPIOx = GPIOx
    GpioCtrlRegs.GPBQSEL2.bit.GPIO54 = 2;
    GpioCtrlRegs.GPBDIR.bit.GPIO54 = 0;   // GPIOx = input

    // Enable an GPIO output on GPIOx, set it high
    GpioCtrlRegs.GPBPUD.bit.GPIO55 = 0;   // Enable pullup on GPIOx
    GpioDataRegs.GPBCLEAR.bit.GPIO55 = 1;   // Load output latch
    GpioCtrlRegs.GPBMUX2.bit.GPIO55 = 0;  // GPIOx = GPIOx
    GpioCtrlRegs.GPBQSEL2.bit.GPIO55 = 2;
    GpioCtrlRegs.GPBDIR.bit.GPIO55 = 0;   // GPIOx = input

    // Enable an GPIO output on GPIOx, set it high
    GpioCtrlRegs.GPBPUD.bit.GPIO56 = 0;   // Enable pullup on GPIOx
    GpioDataRegs.GPBCLEAR.bit.GPIO56 = 1;   // Load output latch
    GpioCtrlRegs.GPBMUX2.bit.GPIO56 = 0;  // GPIOx = GPIOx
    GpioCtrlRegs.GPBQSEL2.bit.GPIO56 = 2;
    GpioCtrlRegs.GPBDIR.bit.GPIO56 = 0;   // GPIOx = input

    // Enable an GPIO output on GPIOx, set it high
    GpioCtrlRegs.GPBPUD.bit.GPIO57 = 0;   // Enable pullup on GPIOx
    GpioDataRegs.GPBCLEAR.bit.GPIO57 = 1;   // Load output latch
    GpioCtrlRegs.GPBMUX2.bit.GPIO57 = 0;  // GPIOx = GPIOx
    GpioCtrlRegs.GPBQSEL2.bit.GPIO57 = 2;
    GpioCtrlRegs.GPBDIR.bit.GPIO57 = 0;   // GPIOx = input

    // Enable an GPIO output on GPIOx, set it high
    GpioCtrlRegs.GPCPUD.bit.GPIO68 = 0;   // Enable pullup on GPIOx
    GpioDataRegs.GPCCLEAR.bit.GPIO68 = 1;   // Load output latch
    GpioCtrlRegs.GPCMUX1.bit.GPIO68 = 0;  // GPIOx = GPIOx
    GpioCtrlRegs.GPCDIR.bit.GPIO68 = 0;   // GPIOx = input

    // Enable an GPIO output on GPIOx, set it high
    GpioCtrlRegs.GPCPUD.bit.GPIO67 = 0;   // Enable pullup on GPIOx
    GpioDataRegs.GPCCLEAR.bit.GPIO67 = 1;   // Load output latch
    GpioCtrlRegs.GPCMUX1.bit.GPIO67 = 0;  // GPIOx = GPIOx
    GpioCtrlRegs.GPCDIR.bit.GPIO67 = 0;   // GPIOx = input

    EDIS;

}

char testDiscretInp2(UINT32 pin){
	if(GpioDataRegs.GPCDAT.all & pin)
		return 0;
	else
		return 1;
}

char testDiscretInp(UINT32 pin){
	if(GpioDataRegs.GPBDAT.all & pin)
		return 0;
	else
		return 1;
}
//                           предел значения задания, порог срабатывания ,         счетчик,                              шаг приращения
char incButton( int lim_button,  int por_button)
{
     int tmp;
    if(!GpioDataRegs.GPBDAT.bit.GPIO54)
    {
        inc_ctr++;
        if(inc_ctr>=por_button)
        {
            tmp=getRegister(_OS_PID_TASK);
            tmp+=10;
            if(tmp<=lim_button)
            {
                SetRegister(_OS_PID_TASK, tmp);
            }
            else
            {
                SetRegister(_OS_PID_TASK, lim_button);
            }
            inc_ctr=0;
        }
     }
    else
    {
        inc_ctr--;
        if(inc_ctr<=0)
        {
            inc_ctr=0;
        }
    }
}

char decButton( int lim_button, int por_button)
{
     int tmp;
    if(!GpioDataRegs.GPBDAT.bit.GPIO53)
    {
        dec_ctr++;
        if(dec_ctr>=por_button)
        {
            tmp=getRegister(_OS_PID_TASK);
            tmp-=10;
            if(tmp<=0)
                     {
                         SetRegister(_OS_PID_TASK, 0);
                     }
                     else
                     {
                         SetRegister(_OS_PID_TASK, tmp);
                     }
            dec_ctr=0;
        }
     }
    else
    {
        dec_ctr--;
        if(dec_ctr<=0)
        {
            dec_ctr=0;
        }
    }
}

unsigned int check_relay_ready(void)
{
ready=0;
ready= !GpioDataRegs.GPBDAT.bit.GPIO55 + !GpioDataRegs.GPBDAT.bit.GPIO56 +  !GpioDataRegs.GPCDAT.bit.GPIO68;
    return ready;
}
