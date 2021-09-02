/* ******************************************************************
 * интерфейс с модулем светодиодов
 * ******************************************************************/

#ifndef ___BLINK_H___
#define ___BLINK_H___

#define _led_splash_delay 2
#define _led_blink_delay 10

#define _off_led 0
#define _on_led 1
#define _blink_led 2
#define _splash_led 4
#define _fast_blink_led 8

#define _ERROR_LED 1
#define _WORCK_LED 2
#define _ADD_LED 0

void initLed(void);
void cntrlLedMode(void);
void setLedMode(char led, char mode);

void setPin1(void);
void clrPin1(void);
void setPin2(void);
void clrPin2(void);
void setPin3(void);
void clrPin3(void);

#endif
