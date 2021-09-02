/*
 * ErrorTrack.h
 *
 *  Created on: 11 ����. 2015
 *      Author: Tester
 */

#ifndef ERRORTRACK_ERRORTRACK_H_
#define ERRORTRACK_ERRORTRACK_H_

#include "added.h"

typedef struct _ERROR{
    UINT16 code;
    UINT32 time;
}stError, *pError;

void initTrackError(void);
void cntrlErrorTrack(void);
char storyErrorTrack(float *strFloat, UINT16 adr);
char readErrorTrack(float *strFloat, UINT16 adr);
char storyErrorValue(float strFloat, UINT16 adr);
char readErrorValue(float *strFloat, UINT16 adr);
char storyTrack(UINT16 adrTrack);
char readTrack(UINT16 adrTrack);
void storySinglTrack(void);
void readSinglTrack(void);

char storyErrorTime(UINT32 time, UINT16 adr);
char readErrorTime(UINT32 *time, UINT16 adr);

char* convSimv(char simv, char* out);
INT16 testCmdShell_Track(char* *buf, UINT16 number);

float getTaskTrack(void);
float getFoutTrack(void);
float getFovervoltTrack(void);
float getFovercurrTrack(void);
float getIbyTTrack(void);
UINT32 getErrorTime(void);
UINT16 getErrorCode(void);

char* getAnswSingleValF32(UINT8 val, char* out);
char* getAnswSingleValI16(UINT8 val, char* out);

char* getAnswSingleErrTime(UINT8 val, char* out);
char* getAnswSingleErrCode(UINT8 val, char* out);

char* getAnswErrStore(UINT8 val, char* out);

float getIaTrack(UINT16 pntr);
float getIbTrack(UINT16 pntr);
float getUdTrack(UINT16 pntr);
float getImTrack(UINT16 pntr);
float getIdTrack(UINT16 pntr);
float getIqTrack(UINT16 pntr);

UINT8 getHistoryErrors(pError err, UINT8 num);

char* getAnswTrackValI16(UINT8 val, UINT16 bgn, UINT16 num,
		                 char* out, UINT16 max);

char* getAnswTrackValF32(UINT8 val, UINT16 bgn, UINT16 num,
		                 char* out, UINT16 max);


void rcvErrReq(char* rcv);

#define _ESC_SIMV 0x01b
#define _ESC_ADD 0x0db
#define _START_SIMV 0x0f
#define _ESC_START_ADD 0x0df
#define _FINISH_SIMV 0x0
#define _ESC_FINISH_ADD 0xd0

#define _TASK_TRACK 0x01
#define _F_OUT_TRACK 0x02
#define _F_OVV_TRACK 0x03
#define _F_OCL_TRACK 0x04
#define _I_BU_T_TRACK 0x05
#define _TIME_TRACK 0x06

#define _IA_TRACK 0x06
#define _IB_TRACK 0x07
#define _UD_TRACK 0x08
#define _IM_TRACK 0x09
#define _ID_TRACK 0x0a
#define _IQ_TRACK 0x0b

#define _track_lenth 256
#define _track_mask 0xff

#endif /* ERRORTRACK_ERRORTRACK_H_ */
