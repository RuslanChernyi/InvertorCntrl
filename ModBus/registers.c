/* **********************************************
 * ������ � ���������� � ������ � ��� � ���
 * ********************************************** */

#include "Registers.h"
#include "cntrlI2C/EEControl.h"
#include "cntrlI2C/ResetEprom.h"
#include "RtcCntrl/RtcCntrl.h"

 #ifdef _INTERNAL_SAVE_
    //������������� ��� �������� ����������� ���������
    //����������� ���

  int RegisterMb[ NumberRegister ];

  int InputMb[ NumberInpReg ];
  char InputBitMb[ NumberInpBit ];
  char CoilMb[ NumberCoils ];

  /*****************************************
  ������ ����������� ��������
  adr - �����, reg - ��������� �� ��������
  0-���������; 1-������ ������
  *****************************************/
  ACK GetCoil ( char adr, char *reg)
  {
    if ( adr < NumberCoils )
    {
        *reg = CoilMb[adr];
        return _GOOD;
    }
    else
        return _NOT_ACK;
  }

  char getCoil ( char adr)
  {
    if ( adr < NumberCoils )
    {
        return CoilMb[adr];
    }
    else
        return 0;
  }
  /*****************************************
  ��������� ����������� ��������
  adr - �����, reg - ����� ��������
  0-���������; 1-������ ������
  *****************************************/
  ACK SetCoil ( char adr, char reg)
  {
    if ( adr < NumberCoils )
    {
    	switch(adr){
			case _START_REWERS:{
				if(reg){
					CoilMb[_STOP_NORM] = 0;
					CoilMb[_START_DIRECT] = 0;
				}
				break;
			}
			case _START_DIRECT:{
				if(reg){
					CoilMb[_STOP_NORM] = 0;
					CoilMb[_START_REWERS] = 0;
				}
				break;
			}
			case _STOP_NORM:{
				if(reg){
					CoilMb[_START_DIRECT] = 0;
					CoilMb[_START_REWERS] = 0;
				}
				break;
			}
    	}
        CoilMb[adr] = reg;
        return _GOOD;
    }
    else
        return _NOT_ACK;
  }

   /*****************************************
  ������ ����������� ��������
  adr - �����, reg - ��������� �� ��������
  0-���������; 1-������ ������
  *****************************************/
  ACK GetInpBit ( char adr, char *reg)
  {
    if ( adr < NumberInpBit )
    {
        *reg = InputBitMb[adr];
        return _GOOD;
    }
    else
        return _NOT_ACK;
  }

  char getInpBit ( char adr)
  {
    if ( adr < NumberInpBit )
    {
    	return (char) InputBitMb[adr];
    }
	return -1;
  }
  /*****************************************
  ��������� ����������� ��������
  adr - �����, reg - ����� ��������
  0-���������; 1-������ ������
  *****************************************/
  ACK SetInpBit ( char adr, char reg)
  {
    if ( adr < NumberInpBit )
    {
        InputBitMb[adr] = reg;
        return _GOOD;
    }
    else
        return _NOT_ACK;
  }

  /*****************************************
  ������ ����������� ��������
  adr - �����, reg - ��������� �� ��������
  0-���������; 1-������ ������
  *****************************************/

  /*****************************************
  ������ ����������� ��������
  adr - �����, reg - ��������� �� ��������
  0-���������; 1-������ ������
  *****************************************/
  ACK GetInpReg ( char adr, int *reg)
  {
    if ( adr < NumberInpReg )
    {
        *reg = InputMb[adr];
        return _GOOD;
    }
    else
        return _NOT_ACK;
  }

  int getInpReg ( char adr)
  {
    if ( adr < NumberInpReg )
    {
    	return InputMb[adr];

    }
    return -1;
  }
  /*****************************************
  ��������� ����������� ��������
  adr - �����, reg - ����� ��������
  0-���������; 1-������ ������
  *****************************************/
  ACK SetInpReg ( char adr, int reg)
  {
    if ( adr < NumberRegister )
    {
        InputMb[adr] = reg;
        return _GOOD;
    }
    else
        return _NOT_ACK;
  }

  /*****************************************
  ������ ����������� ��������
  adr - �����, reg - ��������� �� ��������
  0-���������; 1-������ ������
  *****************************************/
  int getRegister ( char adr )
  {
    if ( adr < NumberRegister )
    {
    	switch(adr){
			default:{
				return RegisterMb[adr];
			}
			case _TIME_ADR_MB:{
				return getRegTime();
			}
			case _DAY_ADR_MB:{
				return getRegData();
			}
			case _YEAR_ADR_MB:{
				return getRegYear();
			}
    	}
    }
    else
        return _NOT_ACK;
  }

  ACK GetRegister ( char adr, int *reg)
  {
    if ( adr < NumberRegister )
    {
        *reg = getRegister(adr);
        return _GOOD;
    }
    else
        return _NOT_ACK;
  }

/*****************************************
  ��������� ����������� ��������
  adr - �����, reg - ����� ��������
  0-���������; 1-������ ������
  *****************************************/
  ACK SetRegister ( char adr, int reg)
  {
    if ( adr < NumberRegister )
    {
        RegisterMb[adr] = reg;

 #ifdef ___EXTARNAL_ROM_SAVE___
      #ifndef __DEBUG_READ
        switch(adr){
//        	case _REWERS_PULT:
//        		break;
			default:{
				WriteAndWait(adr<<1, reg);
				break;
			}
			case _TIME_ADR_MB:{
				setRegTime(reg);
				break;
			}
			case _DAY_ADR_MB:{
				setRegData(reg);
				break;
			}
			case _YEAR_ADR_MB:{
				while(setCurrentYear(reg));
				break;
			}
        }
      #endif
        //WriteUINT(adr<<1, reg);
 #endif
        return _GOOD;
    }
    else
        return _NOT_ACK;
  }

 #endif

#ifdef ___EXTARNAL_ROM_SAVE___

void initHoldingReg( void )
{
   uint tmp, i;
   //while(1){

#ifndef __DEBUG_READ
   while (!ReadAndWait(_RESET_EPROM_NEED<<1, &tmp));
   if (tmp)//(RegisterMb[_RESET_EPROM_NEED])
     ResetEEPROM();
#endif
       for (i=0; i<NumberRegister; i++){
           tmp = i;
		 #ifndef __DEBUG_READ
           while (!ReadAndWait(i<<1, &tmp));
		 #endif
           RegisterMb[i] = tmp;
       }
   //}
   for (i=0; i < NumberInpReg; i++)
       InputMb[i]=0;
   for (i=0; i < NumberInpBit; i++)
       InputBitMb[i]=0;
   for (i=0; i < NumberCoils; i++)
       CoilMb[i]=0;

   InputMb[_SOFT_VERSION] = _VERSION;
}

#endif
