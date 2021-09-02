/*
 * RtcCntrl.h
 *
 *  Created on: 31 ����. 2015
 *      Author: Tester
 */

#ifndef RTCCNTRL_RTCCNTRL_H_
#define RTCCNTRL_RTCCNTRL_H_

#include "added.h"

#define _MINUTS_TIME_MR 0x0a
#define _HOUR_TIME_MR 0x0c
#define _OUT_PWR_CNT_H 0x0e
#define _OUT_PWR_CNT_L 0x010

typedef struct _RTC_DATA{
	char day;
	char month;
}stRtcData, *pRtcData;

typedef struct _RT_CLOCK{
	char minute;
	char hour;
}stRtClock, *pRtClock;

typedef struct _FULL_RTC_DATA{
	char second;
	stRtClock time;
	char week;
	stRtcData data;
	UINT16 year;
}stFullRtcData, *pFullRtcData;

void initRtClock(void);
void cntrlRtcData(void);

char setRegTime(UINT16 reg);
char setCurrentTime(stRtClock time);
char setRegData(UINT16 reg);
char setCurrentData(stRtcData data);
char setCurrentYear(UINT16 year);
UINT16 getRegData(void);
UINT16 getRegTime(void);
UINT16 getRegYear(void);

UINT32 getCurrentTimeAndData(void);

UINT16 getRegSecund(void);
char setRegSecund(UINT16 reg);

INT8 getNVRam(UINT8 adr, UINT16 *val);
char setNVRam(UINT8 adr, UINT16 val);

#endif /* RTCCNTRL_RTCCNTRL_H_ */
