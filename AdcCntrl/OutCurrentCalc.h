/*
 * ��������� ������� � ���������� ������� ��������� ����
 */

#ifndef ___OUT_CURRENT_CALC_H___
#define ___OUT_CURRENT_CALC_H___

typedef struct _OUT_CURRENT{
	float current;
	float coner;
	float currentId;
	float currentIq;
}stCurrentOut, *pCurrentOut;

INT8 getOutCurrent(pCurrentOut current);

void refrOutCurrent(void);
float getIdVal(void);
float getIqVal(void);
float getImVal(void);
float getAlfaVal(void);
INT8 getStatusVal(void);

float getFiltrIq(void);
void initFiltrIv(void);
void cntrlFiltrIv(void);

#endif
