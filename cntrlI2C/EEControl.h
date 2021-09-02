/*
****** доступ к EEPROM через I2C
*/

#ifndef ___EE_CONTROL___
#define ___EE_CONTROL___

#include "added.h"

#define _2_BYTE_ADR
//#define _1_BYTE_ADR


char WriteEE ( char* msg, char cnt );
char ReadEE ( char* msg, char cnt );
char ReadEE_LN ( char* msg, char cnt );

#ifdef _2_BYTE_ADR
char ReadRegister ( uint adr, char* msg );
#endif
      
#ifdef _1_BYTE_ADR
char ReadRegister ( uchar adr, char* msg );
#endif
      
#ifdef _2_BYTE_ADR
char WriteRegister ( uint adr, char* msg );
#endif

#ifdef _1_BYTE_ADR
char WriteRegister ( uchar adr, char* msg );
#endif

char TestEEQuoue( void );

#ifdef _2_BYTE_ADR

char WriteAndWait ( uint adr, uint data );
char ReadAndWait ( uint adr, uint* data );

#endif

#ifdef _1_BYTE_ADR

char WriteAndWait ( uchar adr, uint data );
char ReadAndWait ( uchar adr, uint* data );

#endif

void InitEeContr (void);

#define _Flash_MbusRegister 0
#define _Flash_ErrorTrack 0x01000

#endif
