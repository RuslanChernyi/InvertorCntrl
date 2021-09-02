/*
 * shellTrackCmd.c
 *
 *  Created on: 8 ����. 2015
 *      Author: Tester
 *
 *   ��������� ������� ��������� ����� --track [name] [pntr]
 *
 *   name - ��� ����� , �� ���������� ���.���
 *
 *   pntr - ����� ����� 0 - ���������(�� ���������); 1,2..7 - ���������� (����������� ������)
 *
 */

#include "added.h"
#include "shell.h"
#include "string.h"
#include "stdlib.h"
#include "ErrorTrack/ErrorTrack.h"

const char *trackCmdMain = {"--track"};

char* convSimv(char simv, char* out){
	switch((simv)&0x0ff){
		case _ESC_SIMV:{
			*out = _ESC_SIMV; out++;
			*out = _ESC_ADD;
			break;
		}
		case _START_SIMV:{
			*out = _ESC_SIMV; out++;
			*out = _ESC_START_ADD;
			break;
		}
		case _FINISH_SIMV:{
			*out = _ESC_SIMV; out++;
			*out = _ESC_FINISH_ADD;
			break;
		}
		default:{
			*out = simv;
			break;
		}
	}
	out++;
	return out;
}

INT16 testCmdShell_Track(char* *buf, UINT16 number){
	char *str, *tmp;
//	UINT16 i;

	tmp = *buf;
	str = strstr(tmp, trackCmdMain);
	if (str == NULL)
		return 0;

	return 1;
}
