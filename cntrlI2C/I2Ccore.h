/*
   ��������������� ���������� ������ MSSP - I2C
*/
#ifndef ___I2C_CORE___
#define ___I2C_CORE___

enum I2C_STATE {IDLE_I2C, START_I2C, ADR_I2C, ACK_I2C_GET, ACK_I2C_SET, 
                DATA_I2C, STOP_I2C, RESTART_I2C, READ_I2C_START, removeTask};

#include "added.h"

#define CPU_FRQ_100MHZ /*CPU CLK*/1

#define _ACK_BIT 0x01
#define _SCL_LATCHING 0x010

char testIdle_I2C(void);
void InitI2CSoft (void);
char PulseSCL (void);
void presetSCL(void);
char StartI2C (void);
char StopI2C (void);
char ReStartI2C(void);
char i2c_trsmt_byte(UINT8 trsmt);
char i2c_rcv_byte(UINT8 *rcv);
char i2c_set_ack(UINT8 ack);
void I2CDelay(Uint16 dl);

#endif
