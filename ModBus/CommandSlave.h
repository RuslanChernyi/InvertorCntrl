/**************************************************
������������ ������ MODBUS ��� ������� 
**************************************************/
#ifndef ___COMMAND_SLAVE___
#define ___COMMAND_SLAVE___

/**********************************************************
���������� ��������� ���������
���������� ����� ������ *in+out
���������� ������ ����������, � ��������� � ���-�� ������
**********************************************************/
ACK DeCompileMessageSlv ( char* , char* );

void setAdresMbus(char);
void MirrorAdrMbus (void);

char CmdGetReadingReg ( char *msg, char cntMsg);
char CmdGetInputReg ( char *msg, char cntMsg);
char CmdRegSet ( char *msg, char cntMsg);
char CmdCoilCheng ( char *msg, char cntMsg);
char CmdGetCoils ( char *msg, char cntMsg);
char CmdGetInputBits ( char *msg, char cntMsg);
char CmdRegsNeedSet ( char *msg, char cntMsg);
char getCmdCode(char* msg);
char testAdress(char* msg);

#endif
