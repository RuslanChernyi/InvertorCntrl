/*
 * KaskadOs.h
 *
 *  Created on: 23 окт. 2017 г.
 *      Author: test
 */

#ifndef OSCNTRL_KASKADOS_H_
#define OSCNTRL_KASKADOS_H_

#include "added.h"


#define _INIT_KASKAD_OS 0x01
#define _START_KASKAD_OS 0x02
#define _RUN_KASKAD_OS 0x03
#define _STOP_KASKAD_OS 0x04
#define _OFF_KASKAD_OS 0x05
#define _STEP_UP_KASKAD_OS 0x06
#define _STEP_DOWN_KASKAD_OS 0x07
#define _TUBE_FILL 0x08
#define _ALARM_KASKAD_OS 0x0e0

void KaskadOsPidCntrl(void);
INT8 testKaskadLimit(void);
UINT8 tstPinKaskad(void);
void initStagePin(void);
UINT8 tstPinKaskad(void);
void cntrlAddedKaskadStage(UINT8 stage);

#endif /* OSCNTRL_KASKADOS_H_ */
