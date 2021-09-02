/*
 * shallHelpCmd.c
 *
 *  Created on: 8 жовт. 2015
 *      Author: Tester
 *
 *  обработка команды --help
 *
 */

#include "shell.h"
#include "string.h"
#include "stdlib.h"

#define _help_str_lenth 64

const char *helpCmd = {"--help"};
const char *infoStr = {
		"Преобразователь частоты ЭКТ-5 г.Запорожье, ПАО /0x22 ЗЭАЗ /0x22 . /n"};

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
