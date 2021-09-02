/* ********************************************************
 * ��������� MODBUS
 * ******************************************************** */

#ifndef ___MODBUS_H___
#define ___MODBUS_H___

#include "Registers.h"
#include "MessageASCII.h"

void InitFlow (void);
void initMbusFlow(void);
void UartFlowControl(char* line);

char testFlowMsg(char *msg, UINT16 number);
void initFlowMsg(void);

#endif
