/*
 * shallHelpCmd.c
 *
 *  Created on: 8 ����. 2015
 *      Author: Tester
 *
 *  ��������� ������� --help
 *
 */

#include "shell.h"
#include "string.h"
#include "stdlib.h"

#define _help_str_lenth 64

const char *helpCmd = {"--help"};
const char *infoStr = {
		"��������������� ������� ���-5 �.���������, ��� /0x22 ���� /0x22 . /n"};

INT16 testCmdShell_Help(char* *buf, UINT16 number){
	char *str, *tmp;
	UINT16 i;

	tmp = *buf;
	str = strstr(tmp, helpCmd);
	if (str == NULL)
		return 0;

	i = strlen(infoStr);
	str = malloc(sizeof(char)*i);
	strcpy(str, infoStr);
	return i;
}
