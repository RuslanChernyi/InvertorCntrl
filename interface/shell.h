/*
 * shell.h
 *
 *  Created on: 8 груд. 2014
 *      Author: Tester
 *
 *  контроль интерфейса командной строки
 *
 */

#ifndef SHELL_H_
#define SHELL_H_

#include "added.h"

INT16 testShellMsg(char* *msg, UINT16 number);
char testEquStr(char *str1, const char *strNl);
char testCmdShell(char *msg, UINT16 num, const char *cmg);
void cntrlShellMsg(UINT8 shellByte);

INT16 testCmdShell_Help(char* *buf, UINT16 number);
INT16 testCmdShell_Track(char* *buf, UINT16 number);

#endif /* SHELL_H_ */
