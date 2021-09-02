/* ******************************************************************
 * интерфейс доступа к функциям реле и дискр. выходов
 * ******************************************************************/

#ifndef ___RELE_CNTRL_H___
#define ___RELE_CNTRL_H___

void initDiscrOut(void);
void onChargeRele(void);
void offChargeRele(void);
void onPwrRele(void);
void offPwrRele(void);
void onDistRele(void);
void offDistRele(void);
void onDistOK1(void);
void offDistOK1(void);
void onDistOK2(void);
void offDistOK2(void);

void onBufferEn(void);
void offBufferEn(void);

#endif
