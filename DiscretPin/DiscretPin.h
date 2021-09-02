/*
 * ��������� ������� � ���������� �����
 * ����������� ���������
 */

#ifndef ___DISCRET_PIN_H___
#define ___DISCRET_PIN_H___

#include "added.h"

void initDiscrInput(void);
char testDiscretInp(UINT32 pin);
char testDiscretInp2(UINT32 pin);

#define DISCR_INP_PIN_1 ((UINT32)1 << 20)
#define DISCR_INP_PIN_2 ((UINT32)1 << 21)
#define DISCR_INP_PIN_3 ((UINT32)1 << 22)
#define DISCR_CMD_PIN_1 ((UINT32)1 << 23)
#define DISCR_CMD_PIN_2 ((UINT32)1 << 24)
#define DISCR_CMD_PIN_3 ((UINT32)1 << 25)

#define DISCR_ADD_PIN_1 ((UINT32)1 << 3)
#define DISCR_ADD_PIN_2 ((UINT32)1 << 4)

#endif
