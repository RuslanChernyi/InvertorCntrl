/*
 * AnswCmdDecode.h
 *
 *  Created on: 04 апр. 2017 г.
 *      Author: test
 */

#ifndef ERRORTRACK_ANSWCMDDECODE_H_
#define ERRORTRACK_ANSWCMDDECODE_H_

#include "ErrorTrack.h"

UINT8 testTrackArrCmd(char* ln);
UINT8 testTrackValCmd(char* ln);
UINT8 testTrackCmd(char* ln);
UINT16 getFormAnsw(char* in);
UINT16 getAnswValTrack(char* in, char* *out);
UINT16 getDigTrkKey(const char* ln, const char* key);
UINT16 getAnswArrTrack(char* in, char* *out);
UINT16 cntrlTrackCmd(char* lnIn, char* *lnOut);

#endif /* ERRORTRACK_ANSWCMDDECODE_H_ */
