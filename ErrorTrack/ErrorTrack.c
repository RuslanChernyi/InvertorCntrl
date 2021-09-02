/*
    ������ ������� ������
*/

#include "added.h"
#include "stdlib.h"
#include "AdcCntrl/AdcCntrl.h"
#include "cntrlI2C/EEControl.h"
#include "CntrlLimit/cntrlLimit.h"
#include "ConvertorCntrl/CntrlConvertor.h"
#include "ErrorTrack.h"
#include "RtcCntrl/RtcCntrl.h"

#define _BUFER_STORY_I2C 68
#define _NUM_STORY_I2C 64

#define _adr_ib (256*4)
#define _adr_vdc ((256*4)+_adr_ib)
#define _adr_id ((256*4)+_adr_vdc)
#define _adr_iq ((256*4)+_adr_id)
#define _adr_im ((256*4)+_adr_iq)
#define _adr_task ((256*4)+_adr_im)
#define _adr_udc_middle ((4)+_adr_task)
#define _adr_f_inv ((4)+_adr_udc_middle)
#define _adr_f_cu ((4)+_adr_f_inv)
#define _adr_f_ci ((4)+_adr_f_cu)
#define _adr_i_by_t ((4)+_adr_f_ci)
#define _adr_time ((4)+_adr_i_by_t)
#define _adr_error_code ((4)+_adr_time)

#define _adr_time_alrm1 ((4)+_adr_error_code)
#define _adr_error_code1 ((2)+_adr_time_alrm1)
#define _adr_time_alrm2 ((4)+_adr_error_code1)
#define _adr_error_code2 ((2)+_adr_time_alrm2)
#define _adr_time_alrm3 ((4)+_adr_error_code2)
#define _adr_error_code3 ((2)+_adr_time_alrm3)
#define _adr_time_alrm4 ((4)+_adr_error_code3)
#define _adr_error_code4 ((2)+_adr_time_alrm4)
#define _adr_time_alrm5 ((4)+_adr_error_code4)
#define _adr_error_code5 ((2)+_adr_time_alrm5)

#define _hider_i2c 2

#define _num_history_err 5
#define _NUM_ERROR_HISTORY_I2C ((_num_history_err*(2+4))+_hider_i2c)

#pragma DATA_SECTION ( trackCurrentA , "trackmem");
float trackCurrentA[_track_lenth];
#pragma DATA_SECTION ( trackCurrentB , "trackmem");
float trackCurrentB[_track_lenth];
#pragma DATA_SECTION (trackVoltagDC , "trackmem");
float trackVoltagDC[_track_lenth];
#pragma DATA_SECTION ( trackCurrentId , "trackmem");
float trackCurrentId[_track_lenth];
#pragma DATA_SECTION ( trackCurrentIq , "trackmem");
float trackCurrentIq[_track_lenth];
#pragma DATA_SECTION ( trackCurrentIm , "trackmem");
float trackCurrentIm[_track_lenth];
float lastTask, lastUdcMiddle;
float lastFinv, lastCorrektionFu;
float lastCorrectionFi, lastIbyT;
UINT16 pntrErrorStr, ErrorCode;
UINT32 ErrTime;

stError HistoryErrors[_num_history_err];

void initTrackError(void){
    UINT16 i;
    pntrErrorStr = 0;
    
    for(i=0; i<_track_lenth; i++){
        trackCurrentA[i] = 0;
        trackCurrentB[i] = 0;
        trackVoltagDC[i] = 0;
        trackCurrentId[i] = 0;
        trackCurrentIq[i] = 0;
        trackCurrentIm[i] = 0;
    }
}

void cntrlErrorTrack(void){
    trackCurrentA[pntrErrorStr] = getMomentaryCurrentA();
    trackCurrentB[pntrErrorStr] = getMomentaryCurrentB();
    trackVoltagDC[pntrErrorStr] = getMomentaryVoltagDC();
    trackCurrentId[pntrErrorStr] = getIdVal();
    trackCurrentIq[pntrErrorStr] = getIqVal();
    trackCurrentIm[pntrErrorStr] = getImVal();
    pntrErrorStr++;
    pntrErrorStr &= _track_mask;
}

char storyErrorTrack(float *strFloat, UINT16 adr){
    UINT16 i, pntrFloat;
    UINT8 *arrayI2C, val;
    UINT8 *tmp;
    arrayI2C = malloc(_BUFER_STORY_I2C);
    pntrFloat = 0;
    while(pntrFloat<256){
        i = 0;
        arrayI2C[0] = ((adr+pntrFloat*4)>>8)&0x0ff;
        arrayI2C[1] = (adr+pntrFloat*4)&0x0ff;
        while(i<64){
            tmp = (UINT8*) &strFloat[pntrFloat]; pntrFloat++;
            arrayI2C[_hider_i2c+i] = ((*tmp)&0x0ff); i++;
            arrayI2C[_hider_i2c+i] = ((*tmp)>>8)&0x0ff; tmp++; i++;
            arrayI2C[_hider_i2c+i] = ((*tmp)&0x0ff); i++;
            arrayI2C[_hider_i2c+i] = ((*tmp)>>8)&0x0ff; i++;
        }
        i += 2;
        do{
        	val = WriteEE((char*)arrayI2C, (_NUM_STORY_I2C+_hider_i2c));
        }while(!val);
    }
    free(arrayI2C);
    return val;
}

char storyErrorHistory(pError error, UINT16 adr){
    UINT16 i, cnt;
    UINT8 *arrayI2C, val;
    UINT8 *tmp;
    arrayI2C = malloc(_BUFER_STORY_I2C);
    i = _hider_i2c;
    arrayI2C[0] = (adr>>8)&0x0ff;
    arrayI2C[1] = (adr)&0x0ff;
    for(cnt=0; cnt<_num_history_err; cnt++){
        tmp = (UINT8*) &error[cnt].code;
        arrayI2C[i] = ((*tmp)&0x0ff); i++;
        arrayI2C[i] = ((*tmp)>>8)&0x0ff; i++;
        tmp = (UINT8*) &error[cnt].time;
        arrayI2C[i] = ((*tmp)&0x0ff); i++;
        arrayI2C[i] = ((*tmp)>>8)&0x0ff; i++; tmp++;
        arrayI2C[i] = ((*tmp)&0x0ff); i++;
        arrayI2C[i] = ((*tmp)>>8)&0x0ff; i++;
    }
    do{
        val = WriteEE((char*)arrayI2C, _NUM_ERROR_HISTORY_I2C);
    }while(!val);

    free(arrayI2C);
    return val;
}

char readErrorTrack(float *strFloat, UINT16 adr){
    UINT16 i, pntrFloat;
    UINT8 *arrayI2C, val;
    UINT8 *tmp;
    arrayI2C = malloc(_BUFER_STORY_I2C);
    pntrFloat = 0;
    while(pntrFloat<256){
        arrayI2C[0] = ((adr+pntrFloat*4)>>8)&0x0ff;
        arrayI2C[1] = (adr+pntrFloat*4)&0x0ff;
        do{
        	val = ReadEE_LN((char*)arrayI2C, _NUM_STORY_I2C);
        }while(!val);
        i = 0;
        while(i<64){
            tmp = (UINT8*) &strFloat[pntrFloat]; pntrFloat++;
            *tmp = arrayI2C[_hider_i2c+i]&0x0ff; i++;
            *tmp |= (arrayI2C[_hider_i2c+i]<<8); tmp++; i++;
            *tmp = (arrayI2C[_hider_i2c+i])&0x0ff; i++;
            *tmp |= (arrayI2C[_hider_i2c+i]<<8); i++;
        }
    }
    free(arrayI2C);
    return val;
}

readErrorHystory(pError error, UINT16 adr){
    UINT16 i, cnt;
    UINT8 *arrayI2C, val;
    UINT8 *tmp;
    arrayI2C = malloc(_BUFER_STORY_I2C);

    arrayI2C[0] = (adr>>8)&0x0ff;
    arrayI2C[1] = (adr)&0x0ff;
    do{
        val = ReadEE_LN((char*)arrayI2C, _NUM_ERROR_HISTORY_I2C);
    }while(!val);

    i = _hider_i2c;
    for(cnt=0; cnt<_num_history_err; cnt++){
        tmp = (UINT8*) &error[cnt].code;
        *tmp = arrayI2C[i]&0x0ff; i++;
        *tmp |= (arrayI2C[i]<<8); i++;
        tmp = (UINT8*) &error[cnt].time;
        *tmp = (arrayI2C[i])&0x0ff; i++;
        *tmp |= (arrayI2C[i]<<8); i++; tmp++;
        *tmp = (arrayI2C[i])&0x0ff; i++;
        *tmp |= (arrayI2C[i]<<8); i++;
    }

    free(arrayI2C);
    return val;
}

char storyErrorValue(float strFloat, UINT16 adr){
    UINT16 cnt;
    UINT8 *arrayI2C, val;
    UINT8 *tmp;
    arrayI2C = malloc(16);
    cnt = 0;
    tmp = (UINT8*) &strFloat;
    arrayI2C[_hider_i2c+cnt] = (*tmp)&0x0ff; cnt++;
    arrayI2C[_hider_i2c+cnt] = ((*tmp)>>8)&0x0ff; tmp++; cnt++;
    arrayI2C[_hider_i2c+cnt] = (*tmp)&0x0ff; cnt++;
    arrayI2C[_hider_i2c+cnt] = ((*tmp)>>8)&0x0ff; cnt++;
    arrayI2C[0] = (adr>>8)&0x0ff;
    arrayI2C[1] = adr&0x0ff;
    cnt += 2;
    do{
    	val = WriteEE((char*)arrayI2C, cnt);
    }while(!val);
    free(arrayI2C);
    return val;
}

char storyErrorTime(UINT32 time, UINT16 adr){
    UINT16 cnt;
    UINT8 *arrayI2C, val;
    UINT8 *tmp;
    arrayI2C = malloc(16);
    cnt = 0;
    tmp = (UINT8*) &time;
    arrayI2C[_hider_i2c+cnt] = (*tmp)&0x0ff; cnt++;
    arrayI2C[_hider_i2c+cnt] = ((*tmp)>>8)&0x0ff; tmp++; cnt++;
    arrayI2C[_hider_i2c+cnt] = (*tmp)&0x0ff; cnt++;
    arrayI2C[_hider_i2c+cnt] = ((*tmp)>>8)&0x0ff; cnt++;
    arrayI2C[0] = (adr>>8)&0x0ff;
    arrayI2C[1] = adr&0x0ff;
    cnt += 2;
    do{
    	val = WriteEE((char*)arrayI2C, cnt);
    }while(!val);
    free(arrayI2C);
    return val;
}

char storyErrorCode(UINT16 code, UINT16 adr){
    UINT16 cnt;
    UINT8 *arrayI2C, val;
    UINT8 *tmp;
    arrayI2C = malloc(16);
    cnt = 0;
    tmp = (UINT8*) &code;
    arrayI2C[_hider_i2c+cnt] = (*tmp)&0x0ff; cnt++;
    arrayI2C[_hider_i2c+cnt] = ((*tmp)>>8)&0x0ff; tmp++; cnt++;
    arrayI2C[0] = (adr>>8)&0x0ff;
    arrayI2C[1] = adr&0x0ff;
    cnt += 2;
    do{
    	val = WriteEE((char*)arrayI2C, cnt);
    }while(!val);
    free(arrayI2C);
    return val;
}

char readErrorValue(float *strFloat, UINT16 adr){
    UINT16 cnt;
    UINT8 *arrayI2C, val;
    UINT8 *tmp;
    arrayI2C = malloc(16);
    arrayI2C[0] = (adr>>8)&0x0ff;
    arrayI2C[1] = adr&0x0ff;
    cnt = _hider_i2c + 4;
    do{
    	val = ReadEE_LN((char*)arrayI2C, cnt);
    }while(!val);
    tmp = (UINT8*) strFloat;
    *tmp = (arrayI2C[_hider_i2c])&0x0ff;
    *tmp |= (arrayI2C[_hider_i2c+1])<<8; tmp++;
    *tmp = (arrayI2C[_hider_i2c+2])&0x0ff;
    *tmp |= (arrayI2C[_hider_i2c+3])<<8; tmp++;
    
    free(arrayI2C);
    return val;
}

char readErrorTime(UINT32 *time, UINT16 adr){
    UINT16 cnt;
    UINT8 *arrayI2C, val;
    UINT8 *tmp;
    arrayI2C = malloc(16);
    arrayI2C[0] = (adr>>8)&0x0ff;
    arrayI2C[1] = adr&0x0ff;
    cnt = _hider_i2c + 6;
    do{
    	val = ReadEE_LN((char*)arrayI2C, cnt);
    }while(!val);
    tmp = (UINT8*)time;
    *tmp = (arrayI2C[_hider_i2c])&0x0ff;
    *tmp |= (arrayI2C[_hider_i2c+1])<<8; tmp++;
    *tmp = (arrayI2C[_hider_i2c+2])&0x0ff;
    *tmp |= (arrayI2C[_hider_i2c+3])<<8; tmp++;

    free(arrayI2C);
    return val;
}

char readErrorCode(UINT16 *code, UINT16 adr){
    UINT16 cnt;
    UINT8 *arrayI2C, val;
    UINT8 *tmp;
    arrayI2C = malloc(8);
    arrayI2C[0] = (adr>>8)&0x0ff;
    arrayI2C[1] = adr&0x0ff;
    cnt = _hider_i2c + 2;
    do{
    	val = ReadEE_LN((char*)arrayI2C, cnt);
    }while(!val);
    tmp = (UINT8*)code;
    *tmp = (arrayI2C[_hider_i2c])&0x0ff;
    *tmp |= (arrayI2C[_hider_i2c+1])<<8; tmp++;

    free(arrayI2C);
    return val;
}

void shiftTrack(float *track, UINT16 pnt){
	float tmp;
	UINT16 j;
	while(pnt){
		tmp = track[0];
		for(j=0; j<(_track_lenth-1); j++)
			track[j] = track[j+1];
		track[_track_lenth-1] = tmp;
		pnt--;
	}
}

void shirtErrorHisory(void){
    UINT8 i;
    for(i=(_num_history_err-1); i>0; i--){
        HistoryErrors[i] = HistoryErrors[i-1];
    }
    HistoryErrors[0].code = ErrorCode;
    HistoryErrors[0].time = ErrTime;
}

char storyTrack(UINT16 adrTrack){
	shiftTrack(trackCurrentA, pntrErrorStr);
    storyErrorTrack(trackCurrentA, adrTrack);
    shiftTrack(trackCurrentB, pntrErrorStr);
    storyErrorTrack(trackCurrentB, adrTrack+_adr_ib);
    shiftTrack(trackVoltagDC, pntrErrorStr);
    storyErrorTrack(trackVoltagDC, adrTrack+_adr_vdc);
    shiftTrack(trackCurrentId, pntrErrorStr);
    storyErrorTrack(trackCurrentId, adrTrack+_adr_id);
    shiftTrack(trackCurrentIq, pntrErrorStr);
    storyErrorTrack(trackCurrentIq, adrTrack+_adr_iq);
    shiftTrack(trackCurrentIm, pntrErrorStr);
    storyErrorTrack(trackCurrentIm, adrTrack+_adr_im);
    lastTask = getMainTask(_run_motor);
    storyErrorValue(lastTask, adrTrack+_adr_task);
    lastUdcMiddle = getAverageVoltagDC();
    storyErrorValue(lastUdcMiddle, adrTrack+_adr_udc_middle);
    lastFinv = getCurrentFreq();
    storyErrorValue(lastFinv, adrTrack+_adr_f_inv);
    lastCorrektionFu = getOvervoltagFrequency();
    storyErrorValue(lastCorrektionFu, adrTrack+_adr_f_cu);
    lastCorrectionFi = getOverloadFrequency();
    storyErrorValue(lastCorrectionFi, adrTrack+_adr_f_ci);
    lastIbyT = getValIbyT();
    storyErrorValue(lastIbyT, adrTrack+_adr_i_by_t);
    shirtErrorHisory();
    storyErrorHistory(HistoryErrors, adrTrack+_adr_time_alrm1);
    ErrTime = getCurrentTimeAndData();
    storyErrorTime(ErrTime, adrTrack+_adr_time);
    ErrorCode = refrErrorBits();
    storyErrorCode(ErrorCode, adrTrack+_adr_error_code);
    return 0;
}

char readTrack(UINT16 adrTrack){
    readErrorTrack(trackCurrentA, adrTrack);
    readErrorTrack(trackCurrentB, adrTrack+_adr_ib);
    readErrorTrack(trackVoltagDC, adrTrack+_adr_vdc);
    readErrorTrack(trackCurrentId, adrTrack+_adr_id);
    readErrorTrack(trackCurrentIq, adrTrack+_adr_iq);
    readErrorTrack(trackCurrentIm, adrTrack+_adr_im);
    readErrorValue(&lastTask, adrTrack+_adr_task);
    readErrorValue(&lastUdcMiddle, adrTrack+_adr_udc_middle);
    readErrorValue(&lastFinv, adrTrack+_adr_f_inv);
    readErrorValue(&lastCorrektionFu, adrTrack+_adr_f_cu);
    readErrorValue(&lastCorrectionFi, adrTrack+_adr_f_ci);
    readErrorValue(&lastIbyT, adrTrack+_adr_i_by_t);
    readErrorTime(&ErrTime, adrTrack+_adr_time);
    readErrorCode(&ErrorCode, adrTrack+_adr_error_code);
    readErrorHystory(HistoryErrors, adrTrack+_adr_time_alrm1);
    return 0;
}

UINT8 getHistoryErrors(pError err, UINT8 num){
    if(num < _num_history_err){
        *err = HistoryErrors[num];
        return num;
    }else
        return 0x80;
}

void storySinglTrack(void){
	storyTrack(_Flash_ErrorTrack);
}

void readSinglTrack(void){
	readTrack(_Flash_ErrorTrack);
}

float getTaskTrack(void){
	return lastTask;
}

float getFoutTrack(void){
	return lastFinv;
}

float getFovervoltTrack(void){
	return lastCorrektionFu;
}

UINT32 getErrorTime(void){
	return ErrTime;
}

UINT16 getErrorCode(void){
	return ErrorCode;
}

float getFovercurrTrack(void){
	return lastCorrectionFi;
}

float getIbyTTrack(void){
	return lastIbyT;
}

float getIaTrack(UINT16 pntr){
	return trackCurrentA[pntr];
}

float getIbTrack(UINT16 pntr){
	return trackCurrentB[pntr];
}

float getUdTrack(UINT16 pntr){
	return trackVoltagDC[pntr];
}

float getImTrack(UINT16 pntr){
	return trackCurrentIm[pntr];
}

float getIdTrack(UINT16 pntr){
	return trackCurrentId[pntr];
}

float getIqTrack(UINT16 pntr){
	return trackCurrentIq[pntr];
}
