/*
 * MtoTimeCntrl.h
 *
 *  Created on: 18 нояб. 2019 г.
 *      Author: test
 */

#ifndef ADCCNTRL_MTOTIMECNTRL_H_
#define ADCCNTRL_MTOTIMECNTRL_H_

#include "added.h"

void initMotoresource(void);
void saveMinutsMR(UINT16 val);
void saveMotoResourse(UINT16 val);
void saveMotroPwrCnt(float val);
void cntrlMotoResource(void);
UINT32 getMotorPwrOut(void);
UINT16 getTimeMotoResourse(void);

#endif /* ADCCNTRL_MTOTIMECNTRL_H_ */
