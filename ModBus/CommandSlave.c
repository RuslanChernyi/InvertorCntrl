/**************************************************
������������ ������ MODBUS ��� ������� 
**************************************************/

#include "modbus.h"
#include "cntrlI2C/EEControl.h"

#define cSlave 0x0a 

//#pragma udata
unsigned char uchSlave=cSlave;

void setAdresMbus(char adr)
{
    uchSlave=adr;
}

void MirrorAdrMbus (void)
{
	UINT16 tmp;
    while (!ReadAndWait(_PULT_ADR<<1, &tmp));
    uchSlave = (unsigned char)tmp;
}

char testAdress(char* msg)
{
    if(uchSlave == msg[0])
        return 1;
    else
        return 0;
}

char getCmdCode(char* msg)
{
    return msg[1];
}

int getRegAdr(char* msg)
{
    int tmp ;
    tmp = msg[2];
    tmp <<= 8;
    tmp += ((int)msg[3])&0x0ff;
    return tmp;
}

int getRegVal(char* msg)
{
    int tmp ;
    tmp = msg[4];
    tmp <<= 8;
    tmp += ((int)msg[5])&0x0ff;
    return tmp;
}

char getRegCnt(char* msg)
{
    if(msg[4])
        return 127;
    if(msg[5] > 127)
        return 127;
    return msg[5];
}

/**********************************************************
����� �� ������� 03, 06, 16 - ������ �� SLAVE � ������
���������� ����� ������ ������� in,
������� cnt
������ ��������� ��� ������
**********************************************************/

char CmdGetReadingReg ( char *msg, char cntMsg)
{
    char cnt=0, cntReg, *out;
    int adrReg, reg;
    
    out=msg;
    *out=uchSlave;
    out++; cnt++;
    *out=_READ_REGISTER_CMD;
    out++; cnt++;
    
    adrReg=getRegAdr(msg);
    cntReg=getRegCnt(msg);
            
    if(cntReg > 25)
        cntReg = 25;
    *out=(cntReg)<<1;
    out++; cnt++;
    
    while(cntReg)
    {
        if (GetRegister(adrReg, &reg))
            return 0;
        *out=(char)(reg>>8);
        out++; cnt++;
        *out=(char)(reg&0x0ff);
        out++; cnt++;
        adrReg++; cntReg--;
    }
    *out=LRC(msg, cnt);
    cnt++;
    return cnt;
}

char CmdGetInputReg ( char *msg, char cntMsg)
{
    char cnt=0, cntReg, *out;
    int adrReg, reg;
    
    
    out=msg;
    *out=uchSlave;
    out++; cnt++;
    *out=_READ_INP_REG_CMD;
    out++; cnt++;
    
    adrReg=getRegAdr(msg);
    cntReg=getRegCnt(msg);
            
    if(cntReg > 25)
        cntReg = 25;
    *out=(cntReg)<<1;
    out++; cnt++;
    
    while(cntReg)
    {
        if (GetInpReg(adrReg, &reg))
            return 0;
        *out=(char)(reg>>8);
        out++; cnt++;
        *out=(char)(reg&0x0ff);
        out++; cnt++;
        adrReg++; cntReg--;
    }
    *out=LRC(msg, cnt);
    cnt++;
    return cnt;
}

char CmdRegSet ( char *msg, char cntMsg)
{
    if ( SetRegister(getRegAdr(msg), getRegVal(msg)) == _NOT_ACK ) 
        return 0;
        
    return cntMsg;
}

char CmdCoilCheng ( char *msg, char cntMsg)
{
    if(getRegVal(msg))
    {
        if ( SetCoil(getRegAdr(msg), 1) == _NOT_ACK) 
            return 0;
    }
    else
    {
        if ( SetCoil(getRegAdr(msg), 0) == _NOT_ACK) 
            return 0;
    }
    
    return cntMsg;
}

char CmdGetCoils ( char *msg, char cntMsg)
{
    char cnt=0, cntReg, cntBit, *out;
    int adrReg, reg;
    char tmp;
    
    out=msg;
    *out=uchSlave;
    out++; cnt++;
    *out=_READ_COILS_CMD;
    out++; cnt++;
    
    adrReg=getRegAdr(msg);
    cntReg=getRegCnt(msg);
            
    if(cntReg > 25)
        cntReg = 25;
    
    *out=((cntReg - 1)>>3) + 1;
    out++; cnt++;
    
    if(!cntReg > 7)
        cntBit = cntReg;
    else
        cntBit = 8;
    
//    
//    *out=cntReg;
//    out++; cnt++;
    
    while(cntReg > 0)
    {
      reg = 0;
      while (cntBit)
      {
          cntBit--;
          if (GetCoil(adrReg+cntBit, &tmp))
            return 0;
          tmp <<= cntBit;
          reg |= tmp;
      }
        adrReg += 8;
        *out=(char)(reg);
        out++; cnt++;
        cntReg -= 8;
        
        if(!cntReg > 7)
            cntBit = cntReg;
        else
            cntBit = 8;
    }
    *out=LRC(msg, cnt);
    cnt++;
    return cnt;
}

char CmdGetInputBits ( char *msg, char cntMsg)
{
    char cnt=0, cntReg, cntBit, *out;
    int adrReg, reg;
    char tmp;
    
    out=msg;
    *out=uchSlave;
    out++; cnt++;
    *out=_READ_INPUT_BIT_CMD;
    out++; cnt++;
    
    adrReg=getRegAdr(msg);
    cntReg=getRegCnt(msg);
            
    if(cntReg > NumberInpBit)
        cntReg = NumberInpBit;
    
    *out=((cntReg - 1)>>3) + 1;
    out++; cnt++;
    
//    if(!cntReg > 7)
        cntBit = cntReg;
//    else
//        cntBit = 8;
    
//    
//    *out=cntReg;
//    out++; cnt++;
    
    while(cntReg > 0)
    {
      reg = 0;
      while (cntBit)
      {
          cntBit--;
          if (GetInpBit(adrReg+cntBit, &tmp))
            return 0;
          tmp <<= cntBit;
          reg |= tmp;
      }
        adrReg += 8;
        *out=(char)(reg);
        out++; cnt++;
        cntReg -= 8;
        
        if(!cntReg > 7)
            cntBit = cntReg;
        else
            cntBit = 8;
    }
    *out=LRC(msg, cnt);
    cnt++;
    return cnt;
}

char CmdRegsNeedSet ( char *msg, char cntMsg)
{    
    int adrReg, reg;
    char cntReg, *in;
    
    adrReg=getRegAdr(msg);
    cntReg=getRegCnt(msg);
    in=msg+7;
    
    while ( cntReg )
    {
        reg=(int)(*in); in++; ;
        reg=reg<<8;
        reg+=((int)(*in))&0x0ff; in++;
        if ( SetRegister((char) adrReg, reg)) return 0;
        adrReg++; cntReg--;
    }
    *(msg+6)=LRC(msg, 6);
    
    return 7;
}

ACK DeCompileMessageSlv ( char *in_out, char *number )
{   
    char cnt;
    char MsgBin[_MAX_MSG_ASCII];
    
    cnt = ConvertString(in_out, *number, MsgBin);
    
    if ( !cnt )
        return _NOT_ACK;
        
    if ( LRC ( MsgBin, cnt ) )
        return _NOT_ACK;
    
    if(!testAdress(MsgBin))
        return _NOT_ACK;
    
    switch (getCmdCode(MsgBin))
    {
        case 0x01 :     //������� ������ ��������� SLAVE-�
        {            
            cnt = CmdGetCoils( MsgBin, cnt );
            break;
        }
        case 0x02 :     //������� ������ ��������� SLAVE-�
        {            
            cnt = CmdGetInputBits( MsgBin, cnt );
            break;
        }
        case 0x03 :     //������� ������ ��������� SLAVE-�
        {            
            cnt = CmdGetReadingReg( MsgBin, cnt );
            break;
        }
        case 0x04 :     //������� ������ ��������� SLAVE-�
        {            
            cnt = CmdGetInputReg( MsgBin, cnt );
            break;
        }
        case 0x05 :     //������� ������ ��������� SLAVE-�
        {            
            cnt = CmdCoilCheng( MsgBin, cnt );
            break;
        }
        case 0x06 :     //������� ��������� 1-�� �������� SLAVE-�
        {
            cnt = CmdRegSet( MsgBin, cnt );
            
            break;
        }
        case 0x010 :     //������� ��������� ���������� ��������� SLAVE-�
        {
            cnt = CmdRegsNeedSet( MsgBin, cnt );
            break;
        }
        default : return _UnKnownCommand;
    }
    
    if(!cnt)
        return _NOT_ACK;
        
    *number=ConvertMessag(&MsgBin[0], in_out, cnt);
    return _GOOD;
}

