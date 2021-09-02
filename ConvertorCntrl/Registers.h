/* ********************************************************
 * �������� � ���� ������� � ������
 * ��� ����������� ������� ������
 * ******************************************************** */

#include "added.h"
#include "ModBus.Def"

#ifndef ___REGISTERS_H___
#define ___REGISTERS_H___

#define NumberRegister 256
#define NumberInpReg 32
#define NumberInpBit 32
#define NumberCoils 16

//inpit register
#define _OUTPUT_F_INV           1
#define _OUTPUT_VOLTAG          2
#define _OUTPUT_CURRENT         3
#define _OUTPUT_I_Q             4
#define _OUTPUT_I_D             5
#define _CONER_OUTPUT_I_U       6
#define _DC_VOLTAG              7
#define _POWER_NET_VOLTAG       8
#define _ACTIVE_F_TASK          9
#define _ANALOG_INPUT_TASK      10
#define _ANALOG_INPUT_OS        11
#define _IA_CURRENT             12
#define _IB_CURRENT             0x0d
#define _ANALOG_VALUE_TASK      0x0E
#define _ANALOG_VALUE_OS        0x0f
#define _ANALOG_TEMPERATURE     0x010
#define _NET_VOLTAG             0x011
#define _SOFT_VERSION     0x012
#define _MOTO_RESOURSE 19
#define _OUT_POWER_CNT 0x014
#define _OUT_PoWeR_CNT_H 0x015
#define _MOTO_RESOURSE_MINUT 0x016

//coil addres
#define _START_DIRECT           0x01
#define _START_REWERS           0x02
#define _ALARM_STOP_CMD         0x03
#define _RESET_ALARM            0x04
#define _RUN_PULT               0x05
#define _REWERS_PULT            0x06
#define _RUN_JOB                0x07
#define _STOP_NORM              0x08

//input bit
#define _RUN_BIT_STATUS         0x01
#define _DIRECT_RUN             0x02
#define _REWERS_RUN             0x03
#define _OVER_CURRENT_LIM       0x04
#define _DRIVER_ALARM           0x05
#define _ALARM_STOP             0x06
#define _I_MAX_ALARM            0x07
#define _I_BY_T_ALARM           0x08
#define _UD_MAX_ALARM           0x09
#define _UD_MIN_ALARM           0x0a
#define _BROKEN_PHASE           0x0b
#define _BROKEN_SINCHRO         0x0c
#define _EXT_ALARM_STOP             0x0d
#define _NET_PWR_ALARM          0x0e

#define _DIST_CMD_1             0x010
#define _DIST_CMD_2             0x011
#define _DIST_CMD_3             0x012
#define _DIST_F_1               19
#define _DIST_F_2               20
#define _DIST_F_3               21

#define _Driver_A_Alrm          0x01a
#define _Driver_B_Alrm          0x01b
#define _Driver_C_Alrm          0x01c
#define _Driver_Br_Alrm         0x01d

//HOLDING REG
#define _MAIN_TASK              0x01/*FOR RS485 OR MIRRORED TASK*/
#define _ACCEL_TIME             0x02
#define _DECEL_TIME             0x03
#define _CURENT_LIMIT           4/* ����������� �������� ���� */
#define _OVERLOAD_CURRENT       5/* �����-������� ������ */
#define _OVERLOAD_TIME          6
#define _TIME_CURRENT_PROTECT   7
/* �� ������ 0 - I2T ������,
 1 - ������� ������ ��� ���������� ������ */
#define _P_REG_CURRENT_LIM      8
#define _I_REG_CURRENT_LIM      9
#define _VOLTAG_LIMIT           10/* ����� ����������� ���������� */
#define _P_REG_VOLTAG_LIM       11
#define _I_REG_VOLTAG_LIM       12
#define _P_LEVEL_VOLTAG_LIM     0x0d/*����� ����������������� ����������*/
#define _MAGTETIC_VOLTAG        0x0e
#define _MAGTETIC_FREQ          0x0f
#define _1_BROCK_VOLTAG         0x010
#define _1_BROCK_FREQ           17
#define _2_BROCK_VOLTAG         18
#define _2_BROCK_FREQ           19
#define _3_BROCK_VOLTAG         20
#define _3_BROCK_FREQ     		21
#define _SAT_VOLTAG             0x016
#define _SAT_FREQ               0x017

#define _PULT_TASK              0x018
#define _TASK_SOURCE            0x019   /*�������� �������*/
/*
 0 - �����
 1 - ���������� ����
 2 - ���������� ����� 3 ����. �������
 3 - ���������� ����� 8 ����. �������
 4 - ������� RS485
 5 - ��� - ���������
 */
#define _MODE_SIGN_OS           0x01a
#define _OS_PID_TASK            0x01b

#define _ADRESS_RS485           0x01c
#define _SPEED_RS485            0x01d

#define _MASHTAB_CURENT         0x01e
#define _MASHTAB_VOLTAG         0x01f

#define _U_F_TYPE               32
/*
 0 - u/f - const;
 1 - u/f - const � ����������������;
 2 - �������-��������
 3 - ������������ u=f^2 + const
 */

#define _F_SKIP_NUMBER          33
/*  0,1,2,3 �������� */
#define _F_1_SKIP               0x022
#define _F_2_SKIP               0x023
#define _F_3_SKIP               0x024
#define _F_DELTA_SKIP           0x025

#define _CURRENT_MULT           0x026
#define _FREQUANCY_MULT         0x027
#define _VOLTAG_MULT            0x028
/*
 ��� ����������� � ���������
 1 - ����� * 10 , ����� * 10
 2 - ����� * 100 , ����� * 100
 */

#define _NOMINAL_CURRENT        0x029
#define _NOMINAL_VOLTAG         0x02a

#define _MAX_ALARM_CURENT       0x02b
#define _MAX_ALARM_VOLTAG       0x02c
#define _MIN_ALARM_VOLTAG       0x02d

#define _PULT_ADR               0x02e
#define _RESET_EPROM_NEED       0x02f

#define _TASK_D0                0x030
#define _TASK_D1                0x031
#define _TASK_D2                0x032
#define _TASK_D3                0x033
#define _TASK_D4                0x034
#define _TASK_D5                0x035
#define _TASK_D6                0x036
#define _TASK_D7                0x037

#define _P_OS_PID               0x038
#define _I_OS_PID               0x039
#define _D_OS_PID               0x03a

#define _CNTRL_SOURCE            0x03b   /*�������� ����������*/
/*
 0 - �����
 1 - �������.�����
 2 - ������� RS485
 */

#define _UsqrF_ZERRO            0x03c
#define _UsqrF_FREQ             0x03d
#define _UsqrF_VOLTAG           0x03e

#define _MODE_OS_PID            0x03f
#define _ANALOG_INP_MODE        0x040
#define _MIN_TASK_AN_IN         0x041
#define _MAX_TASK_AN_IN         0x042

#define _MIN_LEVEV_CORR_AN_IN   0x043
#define _MAX_LEVEV_CORR_AN_IN   0x044

#define _MODE_TASK_DISCR_IN     0x045
#define _MODE_DISCR_INPUT       0x046
#define _MODE_PIN_STOP          0x047

#define _OSC_VAL1       0x048
#define _OSC_VAL2       0x049
#define _OSC_VAL3           0x04a
#define _OSC_VAL_NUMBER     0x04b

#define _DISC_STOP_MODE     0x04c

#define _OS_INP_MODE        0x04d

#define _EN_TEST_MOTOR_SPEED 	0x050
#define _ENABLE_ALARM_STOP 		0x051
#define _DISABLE_REWERS 		0x052

#define _MODE_CMD_DIST 0x053

#define _DISCR_OUT_VAL1       0x054
#define _DISCR_OUT_VAL2       0x055
#define _DISCR_OUT_VAL3       0x056

#define _MIN_OS_AN_IN         0x057
#define _MAX_OS_AN_IN         0x058

#define _MIN_LEVEV_CORR_OS_IN   0x059
#define _MAX_LEVEV_CORR_OS_IN   0x05a

#define _MIN_TASK_OS    0x05b
#define _MAX_TASK_OS    0x05c

#define _VOLTAG_DC_DECEL   0x05d
#define _FREQ_DC_DECEL   0x05e
#define _TIME_DC_DECEL   0x05f

#define _MOTOR_PWR_NOM       0x060
#define _MOTOR_SPEED_NOM       0x061
#define _MOTOR_POLUS_PAIR       0x062
#define _MOTOR_COS_FI_NOM       0x063
#define _MOTOR_CURRENT_NOM       0x064
#define _MOTOR_PUSK_MOMENT       0x065

#define _MAX_INV_FREQ       	0x06a
#define _KOEF_DECEL_MAX_TIME 	0x06b
#define _KOEF_DECEL_TIME 		0x06c

#define _US_BREAK_KEY 0x06d /*������������� Break decelaration*/
#define _Tau_StabSf 0x06e

/* ��������� ��������� AQUA*/
#define _FREQ_SLOW_STOP 	0x070
#define _AQUA_TIME_ACCEL 	0x071
#define _SLOW_START_AQUA 	0x072
#define _FREQ_MAX_START 	0x073
#define _AQUA_SLOW_STOP 	0x074
#define _AQUA_FAST_STOP 	0x075
#define _AQUA_MIDDLE_STOP 	0x076
#define _AQUA_F_MIDDLE_STOP 0x077
#define _F_MAX_OS       0x078
#define _F_MIN_OS       0x079
#define _MODE_TASK_OS	0x07a
#define _DELTA_OS       0x07b
#define _DIRECTION_REG_OS       0x07c
#define _AQUA_EKONOM_P_STOP 	0x07d /*допустимый уровень превышения*/
#define _AQUA_EKONOM_P_WORK 	0x07e /*допустимый уровень расхода*/
#define _AQUA_ECONOMISER 	0x07f

#define _MODE_RS485		0x080
#define _OVERLOAD_TYPE	0x081
#define _OVERLOAD_OBJ	0x082

#define _FREQ_BUMP		0x083
#define _U_WITH_F_TYPE	0x084
#define _PWM_FRC		0x085
#define _MAX_PULT_TASK	0x086

#define _CURRENT_LIMIT_SF		0x087
#define _TIME_FIND		0x088
#define _FIND_KOEF_P	0x089
#define _FIND_KOEF_I	0x08a
#define _DIRECTION_FIND	0x08b

#define _LEVEL_BREAK_LIM	0x08c

#define _DAY_ADR_MB		0x08d
#define _TIME_ADR_MB	0x08e
#define _YEAR_ADR_MB	0x08f

#define _FILL_TUBE_TIME 0x090
#define _FILL_TUBE_TASK 0x091
#define _FILL_TUBE_TEMP 0x092 /*����� ����������, �����*/
#define _FILL_TUBE_LEVEL 0x093
#define _FILL_TUBE_ENABLE 0x094

#define _FIX_AQUA_P_MIN 0x095
#define _AQUA_P_MIN_VAL 0x096

#define _MASHTAB_CURENT_2 0x097

#define _ECONOM_VOLTAG  0x098
#define _ECONOM_I_ON    0x099
#define _ECONOM_I_OFF   0x09a

#define _STOP_METHOB   0x09b
#define _STOP_PAUSE   0x09c
#define _COOLER_CNTRL_METHOB 0x09d
#define _NET_MSHT   0x09e
#define _NET_MAX   0x09f
#define _NET_MIN   0x0a0

#define _P_KOEF_LIM_I_BY_U 0x0a1
#define _I_KOEF_LIM_I_BY_U 0x0a2
#define _D_KOEF_LIM_I_BY_U 0x0a3
#define _D_KOEF_LIM_I_BY_F 0x0a4

#define _NUM_KASKAD 0x0a5
#define _MODE_KASKAD 0x0a6
#define _SPEED_RS232 0x0a7
#define _ACCESS_RS232 0x0a8
#define _MODE_KSKD_SWTCH 0x0a9
#define _KASKAD_PAUSE_UP 0x0aa
#define _MIN_INV_TASK 0x0ab
#define _DELTA_KASKAD 0x0ac
#define _KASKAD_PAUSE_DOWN 0x0ad

#define _CLEAN_REV_NUM  0x0ae/*количество циклов очистки реверсом*/
#define _CLEAN_REV_TASK 0x0af/*частота толчка для очистки реверсом*/
#define _CLEAN_REV_ACCEL 0x0b0/*темп разгона для очистки*/

#define _REMOTE_TASK 0x0b1 //11_06_21 remote pult task from buttons

#define _LOOSE_BELT 0x0b2
// ���������� ������� ������ � ��������� ������
#define _INTERNAL_SAVE_ /*��������� ������� � ���*/
#define ___EXTARNAL_ROM_SAVE___ /* �������� ������� � ������� I2C EEPROM*/
//#define __DEBUG_READ /*����������� ������ ������*/

#define getNominalCurrent() getRegister(_NOMINAL_CURRENT)
#define getMashtCurrent() getRegister(_MASHTAB_CURENT)
#define getMashtCurrent2() getRegister(_MASHTAB_CURENT_2)
#define getNominalVoltag() getRegister(_NOMINAL_VOLTAG)
#define getMashtVoltag() getRegister(_MASHTAB_VOLTAG)
#define testSoftStart() getRegister(_EN_TEST_MOTOR_SPEED)
#define getAlarmCurrent() getRegister(_MAX_ALARM_CURENT)
#define getAlarmVoltag() getRegister(_MAX_ALARM_VOLTAG)
#define getLimitCurrent() getRegister(_OVER_CURRENT_LIM)
#define getMinimalUd() getRegister(_MIN_ALARM_VOLTAG)
#define getLimitIbyT() getRegister(_OVERLOAD_CURRENT)
#define getMaxInvertorFrequency() getRegister(_MAX_INV_FREQ)
#define getOverloadKi() getRegister(_I_REG_CURRENT_LIM)
#define getOverloadKp() getRegister(_P_REG_CURRENT_LIM)
#define getOvervoltKi() getRegister(_I_REG_VOLTAG_LIM)
#define getOvervoltKp() getRegister(_P_REG_VOLTAG_LIM)
#define getOvervoltKd() getRegister(_D_REG_VOLTAG_LIM)
#define getPultTask() getRegister(_PULT_TASK)

#define getLooseBeltUst() getRegister(_LOOSE_BELT) //������� ���� ��������� ����
//  typedef unsigned char ACK;
//  #define _GOOD 0
//  #define _NOT_ACK 1

/*****************************************
 ������ ����������� ��������
 adr - �����, reg - ��������� �� ��������
 0-���������; 1-������ ������
 *****************************************/
ACK GetRegister(char, int*);
int getRegister(char);
/*****************************************
 ��������� ����������� ��������
 adr - �����, reg - ����� ��������
 0-���������; 1-������ ������
 *****************************************/
ACK SetRegister(char, int);
ACK GetInpReg(char adr, int *reg);
int getInpReg(char adr);
ACK SetInpReg(char adr, int reg);
ACK GetCoil(char adr, char *reg);
char getCoil(char adr);
ACK SetCoil(char adr, char reg);
ACK GetInpBit(char adr, char *reg);
char getInpBit(char adr);
ACK SetInpBit(char adr, char reg);
void initHoldingReg(void);
char getCoil(char adr);

#endif
