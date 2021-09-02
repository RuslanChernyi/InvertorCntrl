/*
 * aqua.h
 *
 *  Created on: 26 ����. 2014
 *      Author: Tester
 */

#ifndef AQUA_H_
#define AQUA_H_

#include "added.h"
#include "ConvertorCntrl/CntrlConvertor.h"


#define _off_all_state 1
#define _off_water 2
#define _start_water 3
#define _fix_water_p_min 4
#define _run_water 5
#define _stop_water 6
#define _error_water 7
#define _fill_tube 8
#define _clean_pump_water 9

#define _bgn_start 10
#define _slow_start_aqua 11
#define _finish_slow_accel 12

#define _bgn_fill_tube 22
#define _slow_fill_tube 23
#define _finish_fill_tube 24
#define _fill_tube_water 25

#define _begin_fast_stop 15
#define _bgn_stop_water 16
//#define _wait_fmin_water 17
#define _wait_stop_fast 18
#define _wait_fmin_water 19
#define _wait_fmiddle_water 20
#define _middle_stop_water 21

#define _no_cntrl_alrm 22
#define _cntrl_KHN 23
#define _cntrl_dry_run 24

#define _aqua_econom_test 25
#define _aqua_econom_stop 26
#define _aqua_econom_wait 27
#define _aqua_econom_restart 28

#define _aqua_clean_accel 29
#define _aqua_clean_run 30
#define _aqua_clean_decel 31
#define _aqua_clean_stop 32
#define _aqua_clean_test 33

#define _direct_mode 1
#define _revers_mode 2

#define _loose_belt 34

void initAqua(void);
void cntrlStateWater(void);

UINT16 initFillTube(void);
char testFuillTube(void);

UINT8 cntrlFillTube(void);
INT8 cntrlStartAqua(UINT8 *state);
INT8 cntrlSlowStop(UINT8 *state);
INT8 cntrlRunWater(UINT8 *state);
INT8 cntrlAquaPumpClean(UINT8 *state);
UINT16 testLooseBelt(void);
#endif /* AQUA_H_ */
