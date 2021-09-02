/*
 * AnswCmdDecode.c
 *
 *  Created on: 04 апр. 2017 г.
 *      Author: test
 */

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "added.h"
#include "AnswCmdDecode.h"

const char* errorTrackValCmd = "--trkval";
const char* errorTrackArrCmd = "--trkarr";
const char* errorTrackEndCmd = "\x0d\x0a";

const char* formSel[]={
	"form=F32",
	"form=I16"
};

#define _formSelNum 2

const char* valSel[] = {
		"val=task",
		"val=fcurr",
		"val=fovvolt",
		"val=fovcur",
		"val=ibyt",
		"val=time",
		"val=err",
        "val=store"
};

#define _valSelNum 8
#define _TimeRec 6
#define _CodeRec 7
#define _ErrStoreRec 8

const char* adr_trk_key={"adr="};
const char* num_trk_key={"num="};

#define _key_lnth 4

const char* trkSel[] = {
		"trk=ia",
		"trk=ib",
		"trk=ud",
		"trk=im",
		"trk=id",
		"trk=iq"
};

#define _numTrkSel 6

#define _answValLenF32 16
#define _answValLenI16 8

#define _maxTrakNum 64
#define _answTrkLenI16 128
#define _answTrkLenF32 336


UINT8 testTrackArrCmd(char* ln){
	char* tmp;
	tmp = strstr(ln, errorTrackArrCmd);
	if(tmp == NULL){
		return 2;
	}

	tmp = strstr(ln, errorTrackEndCmd);
	if(tmp==NULL){
		return 1;
	}

	return 0;
}

UINT8 testTrackValCmd(char* ln){
	char* tmp;
	tmp = strstr(ln, errorTrackValCmd);
	if(tmp == NULL){
		return 2;
	}

	tmp = strstr(ln, errorTrackEndCmd);
	if(tmp==NULL){
		return 1;
	}

	return 0;
}

UINT8 testTrackCmd(char* ln){
	UINT8 tmp;
	tmp = testTrackValCmd(ln);
	switch(tmp){
		default:
			break;
		case 0:
			return 0x081;
		case 1:
			return 1;
	}
	tmp = testTrackArrCmd(ln);
	switch(tmp){
		default:
			break;
		case 0:
			return 0x082;
		case 1:
			return 3;
	}
	return 2;
}

UINT16 getFormAnsw(char* in){
	char* tmp;
	tmp = strstr(in, formSel[0]);
	if(tmp!=NULL){
		return 1;
	}else{
		tmp = strstr(in, formSel[1]);
		if(tmp!=NULL){
			return 2;
		}else{
			return 0x080;
		}
	}
}

UINT16 getAnswValTrack(char* in, char* *out){
	UINT16 val, form;
	char *tmp, *res;
	for(val=0; val<_valSelNum; val++){
		if(strstr(in, valSel[val])!=NULL){
			break;
		}
	}
	val++;
	if(val>_valSelNum)
		return 0;
	if(val==_TimeRec){
		res = (char*)malloc(sizeof(char)*_answValLenF32);
		tmp = getAnswSingleErrTime(val, res);
	}else if(val == _CodeRec){
	    res = (char*)malloc(sizeof(char)*_answValLenI16);
	    tmp = getAnswSingleErrCode(val, res);
	}else if(val == _ErrStoreRec){
	    res = (char*)malloc(sizeof(char)*_answValLenF32);
	    val = getDigTrkKey(in, num_trk_key);
        tmp = getAnswErrStore(val, res);
	}else{
	    form = getFormAnsw(in);
		if(form==0x080)
			return 0;
		if(form==2){
			res = (char*)malloc(sizeof(char)*_answValLenI16);
			tmp = getAnswSingleValI16(val, res);
		}else if(form==1){
			res = (char*)malloc(sizeof(char)*_answValLenF32);
			tmp = getAnswSingleValF32(val, res);
		}
	}
	if(form){
		*tmp = 0;tmp++;
		val = (UINT16)(tmp - res);
	}
	if(val>0){
		*out = res;
		return val;
	}
	return 0;
}

UINT16 getDigTrkKey(const char* ln, const char* key){
	char* tmp;
	UINT16 val;
	tmp = strstr(ln, key);
	if(tmp!=NULL){
		if(!sscanf(&tmp[_key_lnth], "%u", &val)){
			val = 0;
		}
	}else{
		val = 0;
	}
	return val;
}

UINT16 getAnswArrTrack(char* in, char* *out){
	UINT16 val, form, adr, num;
	char *res, *tmp;
	form = getFormAnsw(in);
	for(val=0; val<_numTrkSel; val++){
		if(strstr(in, trkSel[val])!=NULL){
			break;
		}
	}
	val+=_IA_TRACK;

	adr = getDigTrkKey(in, adr_trk_key);
	num = getDigTrkKey(in, num_trk_key);
	if((adr+num)>_track_lenth){
		num = num + _track_lenth - adr;
	}
	if(form==2){
		res = (char*)malloc(sizeof(char)*_answTrkLenI16);
		tmp = getAnswTrackValI16(val, adr, num, res, _answTrkLenI16);
	}else if(form==1){
		res = (char*)malloc(sizeof(char)*_answValLenF32);
		tmp = getAnswTrackValF32(val, adr, num, res, _answTrkLenF32);
	}
	if(form){
		*tmp = 0;tmp++;
		val = (UINT16)(tmp - res);
	}
	if(val>0){
		*out = res;
		return val;
	}
	return 0;
}

UINT16 cntrlTrackCmd(char* lnIn, char* *lnOut){
	switch(testTrackCmd(lnIn)){
	default:
		return 0;
	case 0x081:
		return getAnswValTrack(lnIn, lnOut);
	case 0x082:
		return getAnswArrTrack(lnIn, lnOut);
	}
}
