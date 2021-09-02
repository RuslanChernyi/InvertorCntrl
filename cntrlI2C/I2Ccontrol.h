/* 

** ���������� ������� I2C

*/

//#include <p18cXXX.h>

#ifndef ___I2C_CONTROL___
#define ___I2C_CONTROL___

enum PORT_OPERATION { READ_I2C, READ_LN_I2C, WRITE_I2C, COMPLITE_OPERATION };

typedef struct __I2C_TASK{
    enum PORT_OPERATION type;
    char adress;
    char* data;
    char count;
    struct __I2C_TASK *next;
}stI2CTask, *pstI2CTask;

char PutI2CTask ( pstI2CTask task );
void I2C_Control( void );
void InitI2C (void);

#define _LIMIT_I2C_TIME 44

#endif
