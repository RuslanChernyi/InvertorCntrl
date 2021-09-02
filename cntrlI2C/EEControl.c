/*
*
**
*** ������ � EEPROM ����� I2C
**
*
*/

#include "SysTimer/sysTime.h"
#include "I2Ccontrol.h"
#include "EEControl.h"

#define __SAVE_CNTRL_EN

#define _24LC256_ADR 0b010100000

char WriteEE ( char* msg, char cnt )
{
	stI2CTask EETask;
	do{
		EETask.type = COMPLITE_OPERATION;
		EETask.adress = _24LC256_ADR;
		EETask.type = WRITE_I2C;
		EETask.count = cnt;
		EETask.data = msg;

		if(!PutI2CTask(&EETask))
			return 0;

#ifdef __SAVE_CNTRL_EN
		while(EETask.type != COMPLITE_OPERATION){
			I2C_Control();
		}
//#else
//    while(EETask.type != COMPLITE_OPERATION);
#endif
	}while(EETask.count != 0);
    
    if (EETask.count)
        return 0;

    return 1;
}

char ReadEE ( char* msg, char cnt )
{
	stI2CTask EETask;
	do{
		EETask.type = COMPLITE_OPERATION;
		EETask.adress = _24LC256_ADR;
		EETask.type = READ_I2C;
		EETask.count = -cnt;
		EETask.data = msg;

		if(!PutI2CTask(&EETask))
			return 0;

#ifdef __SAVE_CNTRL_EN
		while(EETask.type != COMPLITE_OPERATION){
			I2C_Control();
		}
//#else
//    while(EETask.type != COMPLITE_OPERATION);
#endif
	}while(EETask.count != 0);

    if (EETask.count)
        return 0;

    return 1;
}

char ReadEE_LN ( char* msg, char cnt )
{
	stI2CTask EETask;
	do{
		EETask.type = COMPLITE_OPERATION;
		EETask.adress = _24LC256_ADR;
		EETask.type = READ_LN_I2C;
		EETask.count = -cnt;
		EETask.data = msg;

		if(!PutI2CTask(&EETask))
			return 0;


#ifdef __SAVE_CNTRL_EN
		while(EETask.type != COMPLITE_OPERATION){
			I2C_Control();
		}
//#else
//    while(EETask.type != COMPLITE_OPERATION);
#endif
	}while(EETask.count != 0);

    if (EETask.count)
        return 0;

    return 1;
}

#ifdef _2_BYTE_ADR
char ReadRegister ( uint adr, char* msg )
{
    *msg = (char)(adr>>8);
    *(msg+1) = (char)(adr&0x0ff);
    return ReadEE_LN (msg, 2);
}
#endif
      
#ifdef _1_BYTE_ADR
char ReadRegister ( uchar adr, char* msg )
{
    *buf = adr;
    return ReadEE (buf, 1);
}
#endif
      
#ifdef _2_BYTE_ADR
char WriteRegister ( uint adr, char* buf )
{
    *buf = (char)(adr>>8);
    *(buf+1) = (char)(adr&0x0ff);
    return WriteEE (buf, 4);
}
#endif

#ifdef _1_BYTE_ADR
char WriteRegister ( uchar adr, char* msg )
{
    *buf = adr;
    return WriteEE (buf, 3);
}
#endif

#ifdef _2_BYTE_ADR

char WriteAndWait ( uint adr, uint data )
{
    char EEbuf[4];
    EEbuf[3]=(char)(data>>8)&0x0ff;
    EEbuf[2]=(char)(data&0x0ff);
    
    if (!WriteRegister(adr, EEbuf))
        return 0;
            
    return 1;
}

char ReadAndWait ( uint adr, uint* data )
{
    char EEbuf[4];
	uint tmp;
//    EEbuf[0]=(char)(adr>>8)&0x0ff;
//    EEbuf[1]=(char)(adr&0x0ff);
    
    if (!ReadRegister(adr, EEbuf))
        return 0;

    tmp = (uint)EEbuf[3];
    tmp = tmp <<8;
    tmp += ((uint)EEbuf[2])&0x0ff;
    *data = tmp;
    return 1;
}

//char WriteUINT ( uint adr, uint data )
//{
//    char EEbuf[4];
////    EEbuf[0]=(char)(adr>>8);
////    EEbuf[1]=(char)(adr&0x0ff);
//    EEbuf[2]=(char)(data>>8);
//    EEbuf[3]=(char)(data&0x0ff);
//    
//    if (!WriteRegister(adr, EEbuf))
//        return 0;
//    return 1;
//}

//char setReadUINT ( uint adr )
//{
////    char EEbuf[4];
////    EEbuf[0]=(char)(adr>>8);
////    EEbuf[1]=(char)(adr&0x0ff);
////    
////    if (!ReadRegister(adr, EEbuf))
////        return 0;
////    return 1;
//    
//}

//void getReadUINT (uint* data )
//{
//    char EEbuf[4];
//    *data = EEbuf[4];
//    *data = *data <<8;
//    *data += EEbuf[3];
//}


#endif

void InitEeContr( void )
{
//	EETask.type = COMPLITE_OPERATION;
//	EETask.adress = 0b010100000;
}
