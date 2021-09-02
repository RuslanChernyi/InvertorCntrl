/* ********************************************************
 * тригонометрические функции
 * ********************************************************/

#include "added.h"

#define c90dgr 0x03fff

const float sinCnst[17] = {
		0, 0.098, 0.195, 0.29,
		0.3827, 0.4714, 0.5556, 0.6344,
		0.7071, 0.772, 0.83147, 0.8819,
		0.9239, 0.95694, 0.980785, 0.9952,
		1
};

/*
 * 0x03fff - 90 dgr
 * */

float getTableSin(UINT16 coner){
	float tmp1, tmp2;
	UINT16 ind;
	ind = coner >> 10;
	coner &= 0x03ff; //~(0x03f<<10);
	tmp1 = sinCnst[ind];
	tmp2 = sinCnst[ind+1];
	tmp2 = tmp2 - tmp1;
	tmp2 = (tmp2 * ((float)coner)) / ((float)1024);
	tmp2 += tmp1;

	return tmp2;
}

float getSin(UINT16 coner){
	float tmp;

	switch(coner&0x0c000)
	{
		default:{
			tmp= getTableSin(coner);
			break;
		}
		case 0x04000:{
			coner = 0x07fff-coner;
			tmp= getTableSin(coner);
			break;
		}
		case 0x08000:{
			coner = 0x03fff&coner;
			tmp= -getTableSin(coner);
			break;
		}
		case 0x0c000:{
			coner = 0x0ffff-coner;
			tmp= -getTableSin(coner);
			break;
		}
	}
	return tmp;
}

float getCos(UINT16 coner){
	coner = c90dgr - coner;
	return getSin(coner);
}

