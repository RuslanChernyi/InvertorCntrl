/*
 * ��������� ���������� (����������� ����������)
 */

#include "added.h"
#include "UART/cntrlUartTransfer.h"
#include "SysTimer/sysTime.h"
#include "ModBus/modbus.h"
#include "AdcCntrl/OutCurrentCalc.h"
#include "AdcCntrl/AdcCntrl.h"
#include "cntrlPWM/PWM.h"
#include "cntrlPWM/cntrlPwm.h"
#include "ConvertorCntrl/IRcompensation.h"
#include "CntrlLimit/cntrlLimit.h"
#include "ConvertorCntrl/CntrlConvertor.h"

#define cStartOSC   0x0a
typedef float (*OscValPntr)(void);

INIT_DELAY(OSC_DL)

float (*OscVal1) (void);
float (*OscVal2) (void);
float (*OscVal3) (void);
static char numberOscVal;

#define byte_check(in,count,out)    \
    if (in==0x0a)                   \
    {                               \
        *out++ = 0x0db;             \
        *out++ = 0x0da;             \
        count+=2;                   \
    }                               \
    else                            \
    {                               \
        if (in==0x0db)              \
        {                           \
            *out++ = 0x0db;         \
            *out++ = 0x0dd;         \
            count+=2;               \
        }                           \
        else                        \
        {                           \
            *out++ = in;            \
            count+=1;               \
        }                           \
    }                               \

static char numberOscVal;

char MsgOsc ( float val, char* *msg )
{
    char cnt=0;
    unsigned char ctmp;
    char *msgB;
    int data;

    data = (int)(val*10);

    msgB=*msg;

    ctmp = (data>> 8) & 0x0ff;
    byte_check(ctmp, cnt, msgB);
    ctmp = (data) & 0x0ff;
    byte_check(ctmp, cnt, msgB);
    *msg = msgB;

    return  cnt;
}

char GetMsgOsc ( char* data )
{
    char cnt = 1;

    *data++ = cStartOSC;
    if (numberOscVal){
    	cnt += MsgOsc ( (*OscVal1)(), &data );//MsgOsc ( getValOSC( DATA1 ), &data );
    }
    if (numberOscVal>1){
    	cnt += MsgOsc ( (*OscVal2)(), &data );//MsgOsc ( getValOSC( DATA2 ), &data );
    }
    if (numberOscVal>2){
    	cnt += MsgOsc ( (*OscVal3)(), &data );//MsgOsc ( getValOSC( DATA3 ), &data );
    }
    return cnt;
}


void transmiteOscMsg(void)
{
    char MsgOSCstr[16];
    char cnt, i;
    if(TEST_DELAY(OSC_DL)){
    	if(numberOscVal){
    		SET_DELAY(OSC_DL, 1);
			if(testRcBufBCnt())
				return;

			cnt = GetMsgOsc(MsgOSCstr);

			for(i = 0; i < cnt; i++){
				putCharUartB(MsgOSCstr[i]);
			}
    	}else{
    		SET_DELAY(OSC_DL, 1000);
    	}
    }
}

float getMessureA(void){
	return ((float)getMomenteryResult(_CURRENT_A_VAL))/100;
}

float getMessureB(void){
	return ((float)getMomenteryResult(_CURRENT_B_VAL))/100;
}

float getExtTask(void){
	return ((float)getMomenteryResult(_TASK_ANALOG_VAL))/100;
}

float getExtOS(void){
	return ((float)getMomenteryResult(_OS_INP_VAL))/100;
}

float getExtTaskAv(void){
	return ((float)getAveregResult(_TASK_ANALOG_VAL))/100;
}

float getExtOSAv(void){
	return ((float)getAveregResult(_OS_INP_VAL))/100;
}

float getPwmA(void){
	return ((float)getTimePwm(0))/100;
}

float getPwmB(void){
	return ((float)getTimePwm(1))/100;
}

float getPwmC(void){
	return ((float)getTimePwm(2))/100;
}

float getConerU(void){
	return ((float)getCurrentConer())/100;
}

float getAddedFrequencyOsc(void){
	return getAddedFrequency();
}

OscValPntr getValOSC(char val)
{
    switch (val)
    {
        default :
        case 1:
            return &getMomentaryVoltagDC;
        case 2:
            return &getMomentaryCurrentA;
        case 3:
            return &getMomentaryCurrentB;
        case 4:
            return &getImVal;
        case 5:
            return &getAlfaVal;
        case 6:
            return &getIdVal;
        case 7:
            return &getIqVal;
        case 8:
            return &getAverageVoltagDC;
        case 9:
            return &getMessureA;
        case 10:
            return &getMessureB;
        case 11:
            return &getExtTask;
        case 12:
            return &getExtOS;
        case 13:
            return &getExtTaskAv;
        case 14:
            return &getExtOSAv;
        case 15:
            return &getPwmA;
        case 16:
            return &getPwmB;
        case 17:
            return &getPwmC;
        case 18:
            return &getConerU;
        case 19:
            return &getAddedVoltag;
        case 20:
            return &getAddedFrequencyOsc;
        case 21:
            return &getMaxVoltagDC;
        case 22:
            return &getOvervoltagFrequency;
        case 23:
            return &getOverloadFrequency;
        case 24:
            return &getCurrentFreq;
        case 25:
            return &getInvertorFreq;
    }
}

void InitOSC ( char all )
{
    numberOscVal = getRegister(_OSC_VAL_NUMBER);
    OscVal1 = getValOSC(getRegister(_OSC_VAL1));
    OscVal2 = getValOSC(getRegister(_OSC_VAL2));
    OscVal3 = getValOSC(getRegister(_OSC_VAL3));
    if(all){
    	ADD_DELAY(OSC_DL, 1000);
    }else{
    	SET_DELAY(OSC_DL, 1000);
    }

}
