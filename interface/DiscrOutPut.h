/*
 * ������� �������� ���������� �������
 */

#ifndef ___DISCR_OUTPUT_H___
#define ___DISCR_OUTPUT_H___

void initDiscrPinOut(void);
void cntrlDiscrPinOut(void);

void onTermoRele(void);
void offTermoRele(void);

UINT16 setDiscrOutFlag(UINT16 flg);
UINT16 clrDiscrOutFlag(UINT16 flg);
UINT16 getDiscrOutFlag(void);

#define _DISCR_OUT_FLG_OVERLOAD 0x01
#define _BLOCK_DISCR_OUT_1 0x02
#define _BLOCK_DISCR_OUT_2 0x04
#define _DISCR_OUT_FLG_BYPASS 0x08
#define _BYPASS_DISCR_OUT_1 0x010
#define _BYPASS_DISCR_OUT_2 0x020

#endif
