/*
 * Stab_Iq.c
 *
 *  Created on: 02 нояб. 2016 г.
 *      Author: test
 *
 *      стабилизация реактивной составляющей выходного тока
 *
 */

#include "AdcCntrl/AdcCntrl.h"
#include "ModBus/Registers.h"

//#define _D_REG_SF
#define _P_REG_SF

#define _N_sf ((float)2.0)
#define dt_sf ((float)0.001)

#ifdef _D_REG_SF
float k1_sf, k2_sf;
#endif

#ifdef _P_REG_SF
float k1_sf;
#endif

float en_1_sf;
float yn_1_sf;

void initIqStab(void){
	float tmp;
	tmp = getRegister(_Tau_StabSf);
#ifdef _D_REG_SF
	tmp = tmp /10000;
	k1_sf = tmp /(tmp + (_N_sf * dt_sf));
	k2_sf = (_N_sf * tmp) / (tmp + (_N_sf * dt_sf));
#endif
#ifdef _P_REG_SF
	tmp = tmp /5000;
	k1_sf = tmp;
#endif
	en_1_sf = 0;
	yn_1_sf = 0;
}

void getIqStab(float *outF){
	float en, yn;
	en = getAveragId() - getFiltrIq();
#ifdef _D_REG_SF
	yn = (k1_sf * yn_1_sf) + (k2_sf * (en - en_1_sf));
#endif

#ifdef _P_REG_SF
	yn = yn_1_sf + (k1_sf * (en - en_1_sf));
#endif
	yn_1_sf = yn;
	en_1_sf = en;
	if(yn > 0.35)
		yn = 0.35;
	if(yn < (-0.35))
		yn = -0.35;
	en = *outF;
	if(en >= 0){
	  en += yn;
	}else{
	  en -= yn;
	}
	*outF = en;
}
