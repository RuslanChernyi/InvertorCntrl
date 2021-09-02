/* ********************************************************
 * �������� ������ ������� ���������������
 * ********************************************************/

#ifndef ___STATUS_CONVERTOR_H___
#define ___STATUS_CONVERTOR_H___

#include "added.h"

#define _DIRECTION_CMD 0x01
#define _REWESR_ROTATE 0x02
#define _RUN_CONVERTOR 0x04
#define _JOB_CONVERTOR 0x08

#define _RESET_CONVERTOR 0x00
#define _INVERTOR_OK 0x00

#define _ALARM_INV 0x08000

#define _PHASE_A 0x010
#define _PHASE_B 0x020
#define _PHASE_C 0x030
#define _BreakErr 0x040

#define _DriverError (_PHASE_A | _PHASE_B | _PHASE_C | _BreakErr)

#define _MaxCurrentLimit 0x050
#define _CurentByTimeLimit 0x060
#define _VoltagMaxLimit 0x070
#define _VoltagMinLimit 0x080
#define _NetMinLimit 0x090
#define _NetMaxLimit 0x0a0

#define _AllError 0x0f0

#define _BROLEN_PHASE 0x0100
#define _AlgoritmError 0x0200
#define _BROLEN_RECT 0x0300

#define _ALARM_EXT 0x01000
#define _ALARM_SENSOR_EXT 0x02000
#define _LooseBelt 0x03000
void initStatusConvertor(void);
UINT16 setStatusConvertor(UINT16 stat);
UINT16 clrStatusConvertor(UINT16 stat);
UINT16 getStatusConvertor(UINT16 stat);
UINT8 testStatusConvertor(UINT16 msk);

#define _ErrorConturRer (_MaxCurrentLimit | _CurentByTimeLimit | \
					  _VoltagMaxLimit | _VoltagMinLimit)

#define getCntrlErrorConvertor() testStatusConvertor(_ALARM_INV | _BROLEN_PHASE | _ALARM_EXT)
#define testErrorInv() testStatusConvertor(_ALARM_INV | _BROLEN_PHASE | \
		                                   _ErrorConturRer | _AlgoritmError | \
										   _ALARM_EXT)

#define testRun() testStatusConvertor(_RUN_CONVERTOR | _JOB_CONVERTOR)
#define testRunMode() testStatusConvertor(_RUN_CONVERTOR)
#define setRunMode() setStatusConvertor(_RUN_CONVERTOR)
#define testJobMode() testStatusConvertor(_JOB_CONVERTOR)
#define setNormalStop() clrStatusConvertor(_RUN_CONVERTOR | _JOB_CONVERTOR)
#define setAlarmStop() setStatusConvertor(_ALARM_EXT)
#define testDirectionCmd() testStatusConvertor(_DIRECTION_CMD)
#define setDirectionCmd() setStatusConvertor(_DIRECTION_CMD)
#define clrDirectionCmd() clrStatusConvertor(_DIRECTION_CMD)

#define setOvervoltagAlrm() setStatusConvertor(_VoltagMaxLimit)
#define setUnderVoltagAlrm() setStatusConvertor(_VoltagMinLimit)
#define setOverCurrent() setStatusConvertor(_MaxCurrentLimit)
#define setIbyT_Alrm() setStatusConvertor(_CurentByTimeLimit)

#define setLooseBelt() setStatusConvertor(_LooseBelt)

#define testAlgoritmErr() 0

#endif
