/*
 * Rs485MbusAscii.h
 *
 *  Created on: 9 груд. 2014
 *      Author: Tester
 */

#ifndef RS485MBUSASCII_H_
#define RS485MBUSASCII_H_


char testASCIImsg(char *msg, UINT16 number);
char cntrlRs485ASCII(char *msg, UINT16 *number, UINT16 maxNum);
void cntrlMbusMsg(UINT8 msgByte);

#endif /* RS485MBUSASCII_H_ */
