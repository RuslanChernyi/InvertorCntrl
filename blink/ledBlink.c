/* ******************************************************************
 *
 * ******************************************************************/

#include "DSP2833x_Device.h"
#include "SysTimer/sysTime.h"
#include "blink.h"

INIT_DELAY(LED_BLINK)

char stateLed[3];
char modeLed[3];

void initLed(void){
	UINT8 tmp;

	ADD_DELAY(LED_BLINK, 0);

	for(tmp = 0; tmp < 3; tmp++){
		stateLed[tmp] = 0;
		modeLed[tmp] = 0;
	}

    EALLOW;

    // Enable an GPIO output on GPIOx, set it high
    GpioCtrlRegs.GPBPUD.bit.GPIO40 = 0;   // Enable pullup on GPIOx
    GpioDataRegs.GPBSET.bit.GPIO40 = 1;   // Load output latch
    GpioCtrlRegs.GPBMUX1.bit.GPIO40 = 0;  // GPIOx = GPIOx
    GpioCtrlRegs.GPBDIR.bit.GPIO40 = 1;   // GPIOx = output

    // Enable an GPIO output on GPIOx, set it high
    GpioCtrlRegs.GPBPUD.bit.GPIO41 = 0;   // Enable pullup on GPIOx
    GpioDataRegs.GPBSET.bit.GPIO41 = 1;   // Load output latch
    GpioCtrlRegs.GPBMUX1.bit.GPIO41 = 0;  // GPIOx = GPIOx
    GpioCtrlRegs.GPBDIR.bit.GPIO41 = 1;   // GPIOx = output

    // Enable an GPIO output on GPIOx, set it high
    GpioCtrlRegs.GPBPUD.bit.GPIO42 = 0;   // Enable pullup on GPIOx
    GpioDataRegs.GPBSET.bit.GPIO42 = 1;   // Load output latch
    GpioCtrlRegs.GPBMUX1.bit.GPIO42 = 0;  // GPIOx = GPIOx
    GpioCtrlRegs.GPBDIR.bit.GPIO42 = 1;   // GPIOx = output

    // Enable an GPIO output on GPIOx, set it high
    GpioCtrlRegs.GPCPUD.bit.GPIO77 = 0;   // Enable pullup on GPIOx
    GpioDataRegs.GPCCLEAR.bit.GPIO77 = 1;   // Load output latch
    GpioCtrlRegs.GPCMUX1.bit.GPIO77 = 0;  // GPIOx = GPIOx
    GpioCtrlRegs.GPCDIR.bit.GPIO77 = 1;   // GPIOx = output

    // Enable an GPIO output on GPIOx, set it high
    GpioCtrlRegs.GPCPUD.bit.GPIO78 = 0;   // Enable pullup on GPIOx
    GpioDataRegs.GPCCLEAR.bit.GPIO78 = 1;   // Load output latch
    GpioCtrlRegs.GPCMUX1.bit.GPIO78 = 0;  // GPIOx = GPIOx
    GpioCtrlRegs.GPCDIR.bit.GPIO78 = 1;   // GPIOx = output

    // Enable an GPIO output on GPIOx, set it high
    GpioCtrlRegs.GPCPUD.bit.GPIO79 = 0;   // Enable pullup on GPIOx
    GpioDataRegs.GPCCLEAR.bit.GPIO79 = 1;   // Load output latch
    GpioCtrlRegs.GPCMUX1.bit.GPIO79 = 0;  // GPIOx = GPIOx
    GpioCtrlRegs.GPCDIR.bit.GPIO79 = 1;   // GPIOx = output

    EDIS;

}

void cntrlLedMode(void){
	char tmp;
	if(TEST_DELAY(LED_BLINK)){
		SET_DELAY(LED_BLINK, c50ms);
		for(tmp = 0; tmp<3; tmp++){
			switch(modeLed[tmp])
			{
				case _off_led:
				{
					stateLed[tmp] = 0;
					break;
				}
				case _on_led:
				{
					stateLed[tmp] = 1;
					break;
				}
				case _blink_led:
				{
					if(stateLed[tmp]<0){
						if(stateLed[tmp] < (-1)){
							stateLed[tmp]++;
						}else{
							stateLed[tmp] = _led_blink_delay;
						}
					}else{
						if(stateLed[tmp]){
							stateLed[tmp]--;
						}else{
							stateLed[tmp] = -_led_blink_delay;
						}
					}
					break;
				}
				case _splash_led:
				{
					if(stateLed[tmp]<0){
						if(stateLed[tmp] < (-1)){
							stateLed[tmp]++;
						}else{
							stateLed[tmp] = _led_splash_delay;
						}
					}else{
						if(stateLed[tmp]){
							stateLed[tmp]--;
						}else{
							stateLed[tmp] = -_led_blink_delay;
						}
					}
					break;
				}
				case _fast_blink_led:
				{
					if(stateLed[tmp]<0){
						if(stateLed[tmp] < (-1)){
							stateLed[tmp]++;
						}else{
							stateLed[tmp] = _led_splash_delay;
						}
					}else{
						if(stateLed[tmp]){
							stateLed[tmp]--;
						}else{
							stateLed[tmp] = -_led_splash_delay;
						}
					}
					break;
				}
			}
		}
	}

	if(stateLed[0]>0)
		GpioDataRegs.GPBSET.bit.GPIO40 = 1;   // Load output latch
	else
		GpioDataRegs.GPBCLEAR.bit.GPIO40 = 1;   // Load output latch

	if(stateLed[1]>0)
		GpioDataRegs.GPBSET.bit.GPIO41 = 1;   // Load output latch
	else
		GpioDataRegs.GPBCLEAR.bit.GPIO41 = 1;   // Load output latch

	if(stateLed[2]>0)
		GpioDataRegs.GPBSET.bit.GPIO42 = 1;   // Load output latch
	else
		GpioDataRegs.GPBCLEAR.bit.GPIO42 = 1;   // Load output latch
}

void setLedMode(char led, char mode){
	modeLed[led] = mode;
}

void setPin1(void){
	GpioDataRegs.GPCSET.bit.GPIO79 = 1;   // Load output latch
}

void clrPin1(void){
	GpioDataRegs.GPCCLEAR.bit.GPIO79 = 1;   // Load output latch
}

void setPin2(void){
	GpioDataRegs.GPCSET.bit.GPIO78 = 1;   // Load output latch
}

void clrPin2(void){
	GpioDataRegs.GPCCLEAR.bit.GPIO78 = 1;   // Load output latch
}

void setPin3(void){
	GpioDataRegs.GPCSET.bit.GPIO77 = 1;   // Load output latch
}

void clrPin3(void){
	GpioDataRegs.GPCCLEAR.bit.GPIO77 = 1;   // Load output latch
}
