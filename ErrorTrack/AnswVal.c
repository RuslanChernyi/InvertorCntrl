/*
 * AnswVal.c
 *
 *  Created on: 20 марта 2017 г.
 *      Author: test
 *
 *  Формирование ответа чтения следа
 *
 */

#include "added.h"
#include "ErrorTrack.h"

#define _F32_MAX 8
#define _I16_MAX 4

char* getSinglValueI16(float* val, char* res){
	INT16 tmp;
	char dig;
	tmp = (INT16)((*val) * 10);
	dig = (char)((tmp>>8)&0x0ff);
	res = convSimv(dig, res);
	dig = (char)(tmp&0x0ff);
	res = convSimv(dig, res);
	return res;
}

char* getSinglValueF32(float* val, char* res){
	UINT32* tmp;
	char dig;
	tmp = (UINT32*)val;
	dig = (char)(((*tmp)>>24)&0x0ff);
	res = convSimv(dig, res);
	dig = (char)(((*tmp)>>16)&0x0ff);
	res = convSimv(dig, res);
	dig = (char)(((*tmp)>>8)&0x0ff);
	res = convSimv(dig, res);
	dig = (char)((*tmp)&0x0ff);
	res = convSimv(dig, res);
	return res;
}

char* getSinglValHeader(UINT8 val, float *fVal, char *out){
	switch(val){
		default:
		case _TASK_TRACK:{
			*fVal = getTaskTrack();
			*out = 'T'; out++;
			*out = 'F';
			break;
		}
		case _F_OUT_TRACK:{
			*fVal = getFoutTrack();
			*out = 'F'; out++;
			*out = 'O';
			break;
		}
		case _F_OVV_TRACK:{
			*fVal = getFovervoltTrack();
			*out = 'F'; out++;
			*out = 'V';
			break;
		}
		case _F_OCL_TRACK:{
			*fVal = getFovercurrTrack();
			*out = 'F'; out++;
			*out = 'I';
			break;
		}
		case _I_BU_T_TRACK:{
			*fVal = getIbyTTrack();
			*out = 'I'; out++;
			*out = 'T';
			break;
		}
	}
	out++;
	return out;
}

char* getAnswSingleValI16(UINT8 val, char* out){
	float fVal;
	out = getSinglValHeader(val, &fVal, out);
	return getSinglValueI16(&fVal, out);
}

char* getAnswSingleErrTime(UINT8 val, char* out){
	UINT32 Time;
	char td;
	char* msg;
	Time = getErrorTime();
	msg = out;
	*msg = 'T'; msg++;
	*msg = 'E'; msg++;

	td = Time>>24;
	msg = convSimv(td, msg);
	td = (Time>>16)&0x0ff;
	msg = convSimv(td, msg);
	td = (Time>>8)&0x0ff;
	msg = convSimv(td, msg);
	td = Time&0x0ff;
	msg = convSimv(td, msg);

	return msg;
}

char* getAnswErrStore(UINT8 val, char* out){
	stError err;
	char td;
	char* msg;
	getHistoryErrors(&err, val);
	msg = out;
	*msg = 'H'; msg++;
	*msg = 'E'; msg++;

    td = (err.code>>8)&0x0ff;
    msg = convSimv(td, msg);
    td = err.code&0x0ff;
    msg = convSimv(td, msg);

	td = err.time>>24;
	msg = convSimv(td, msg);
	td = (err.time>>16)&0x0ff;
	msg = convSimv(td, msg);
	td = (err.time>>8)&0x0ff;
	msg = convSimv(td, msg);
	td = err.time&0x0ff;
	msg = convSimv(td, msg);

	return msg;
}

char* getAnswSingleErrCode(UINT8 val, char* out){
	UINT16 Code;
	char td;
	char* msg;
	Code = getErrorCode();
	msg = out;
	*msg = 'E'; msg++;
	*msg = 'C'; msg++;

	td = (Code>>8)&0x0ff;
	msg = convSimv(td, msg);
	td = Code&0x0ff;
	msg = convSimv(td, msg);

	return msg;
}

char* getAnswSingleValF32(UINT8 val, char* out){
	float fVal;
	out = getSinglValHeader(val, &fVal, out);
	return getSinglValueF32(&fVal, out);
}
typedef float (*tdAnsTrackVal)(UINT16 pntr);

char* fillAnswTrack(UINT8 val, tdAnsTrackVal *fn, char* out){
	switch(val){
		default:
		case _IA_TRACK:{
			*fn = &getIaTrack;
			*out = 'I'; out++;
			*out = 'a'; out++;
			break;
		}
		case _IB_TRACK:{
			*fn = &getIbTrack;
			*out = 'I'; out++;
			*out = 'b'; out++;
			break;
		}
		case _UD_TRACK:{
			*fn = &getUdTrack;
			*out = 'U'; out++;
			*out = 'd'; out++;
			break;
		}
		case _IM_TRACK:{
			*fn = &getImTrack;
			*out = 'I'; out++;
			*out = 'm'; out++;
			break;
		}
		case _ID_TRACK:{
			*fn = &getIdTrack;
			*out = 'I'; out++;
			*out = 'd'; out++;
			break;
		}
		case _IQ_TRACK:{
			*fn = &getIqTrack;
			*out = 'I'; out++;
			*out = 'q'; out++;
			break;
		}
	}
	return out;
}

char* getAnswTrackValI16(UINT8 val, UINT16 bgn, UINT16 num,
		                 char* out, UINT16 max){
	tdAnsTrackVal getVal;
	UINT16 i;
	char *res;
	float f;
	res = fillAnswTrack(val, &getVal, out);
	res = convSimv((num>>8)&0x0ff, res);
//	*res = (num>>8)&0x0ff; res++;
	res = convSimv(num&0x0ff, res);
//	*res = num&0x0ff; res++;

	for(i=0; i<num; i++){
		f = (*getVal)(i+bgn);
		res = getSinglValueI16(&f, res);
		if((res - out) >= (max - _I16_MAX))
			break;
	}

//	i = res - out;
//	*res = (i>>8)&0x0ff; res++;
//	*res = i&0x0ff; res++;

	return res;
}

char* getAnswTrackValF32(UINT8 val, UINT16 bgn, UINT16 num,
		                 char* out, UINT16 max){
	float (*getVal)(UINT16 pntr);
	UINT16 i;
	char *res;
	float f;
	res = fillAnswTrack(val, &getVal, out);
	res = convSimv((num>>8)&0x0ff, res);
//	*res = (num>>8)&0x0ff; res++;
	res = convSimv(num&0x0ff, res);
//	*res = num&0x0ff; res++;

	for(i=0; i<num; i++){
		f = (*getVal)(i+bgn);
		res = getSinglValueF32(&f, res);
		if((res - out) >= (max - _F32_MAX))
			break;
	}

	return res;
}
