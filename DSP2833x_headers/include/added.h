/*
 * ������������ � �������
 * */

#ifndef ___ADDED_H___
#define ___ADDED_H___

#ifdef __TMS320C2000__

#include "DSP2833x_Device.h"

/*
 * c-compiler TMS �� ������������ 8-��� ��������
 * */

typedef unsigned int UINT8;
typedef unsigned int UINT16;
typedef unsigned long UINT32;

typedef signed int INT8;
typedef signed int INT16;
typedef signed long INT32;

typedef unsigned int uint;
typedef signed int Int16;
typedef signed long Int32;

#endif

#define disINT(intGr,intBit)	\
	DINT;						\
	PieCtrlRegs.PIEIER##intGr.bit.intBit=0;	\
	asm("    NOP");					\
	asm("    NOP");					\
	asm("    NOP");					\
	asm("    NOP");					\
	asm("    NOP");					\
	PieCtrlRegs.PIEIFR##intGr.bit.intBit=0;	\
	PieCtrlRegs.PIEACK.all|= PIEACK_GROUP##intGr;	\
	EINT;						\

#define enINT(intGr,intBit)	\
	DINT;						\
	PieCtrlRegs.PIEIER##intGr.bit.intBit=1;	\
	PieCtrlRegs.PIEACK.all|= PIEACK_GROUP##intGr;	\
	EINT;						\

//#define __CORCK_ADC /*�������� �� ���*/

#define __CORCK_PWM /*�������� �� ���*/
//#define __CORCK_ADC_UD /*�������� UD*/
//#define __BLOCK_PWM_ERR
//#define __BLOCK_PWR /*������������� ������ �������� ����*/


#define __MIDDLE_POWER /*средняя мощность ПЧ*/
//#define __HIGH_POWER  /*мощность ПЧ от 45кВт(100 А)*/

#define __ERROR_PHASE_IND   1

#if (defined __MIDDLE_POWER) && (defined __HIGH_POWER)
#error "select single value"
#endif

#ifdef __HIGH_POWER
#define __USE_EXTERN_SIFU /*внешнее SIFU управляется через pin ZARYAD*/
#endif

#define _VERSION 105
/* ****************************************
 * v 10.0
 * доработана программа каскадного регулятора
 * для насосов
 * v 10.1
 * добавлена опция обходного контактора при
 * прергрузке
 * v 10.2
 * переключение каскадов завязано на Delta
 * обратной связи (KaskadLimit lines 26, 37)
 * добавлена deltaOS kaskad switch
 * v 10.3
 * разделена пауза вкл-выкл. каскадов
 * v 10.4
 * в каскадный режим ОС добавлена процедура заполнения насоса
 * v 10.5
 * доработка под Мелитополь - Aqua, обрыв фаз
 * антизаклинивание
 * ***************************************/

/*
 * задействовать контроль обрыва фаз двигателя
 */
#define _BROKEN_PHASE_CNTRL 1

/*
 * счетчмк активной энергии
 */
#define _POWER_OUTPUT_CNRTL 1

/*
 * счетчик моторесурса
 */
#define _TIME_MOTO

#endif
char incButton( int lim_button,  int por_button);
char decButton( int lim_button,  int por_button);
