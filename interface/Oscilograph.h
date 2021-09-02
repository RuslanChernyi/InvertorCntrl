/*
 * ������� ����������� -���������
 */

#ifndef ___OSCILOGRAPH_H___
#define ___OSCILOGRAPH_H___

#include "SysTimer/sysTime.h"

DECKLARATE_DELAY(OSC_DL)

char MsgOsc ( int val, char* *msg );
char GetMsgOsc ( char* data );
void transmiteOscMsg(void);
void InitOSC ( char all );

#endif
