/*
 * rs485Mbus.h
 *
 *  Created on: 6 квіт. 2015
 *      Author: Tester
 */

#ifndef MODBUS_RS485MBUS_H_
#define MODBUS_RS485MBUS_H_

void cntrlMbusMsg(UINT8 msgByte);
void initMbusMsg(void);
void initModbusType(void);
char testAdress485(char *buf);

#endif /* MODBUS_RS485MBUS_H_ */
