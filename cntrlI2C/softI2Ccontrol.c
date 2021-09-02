/* 

** ���������� ������� I2C

*/

//#include <p18cXXX.h>

#include "I2Ccore.h"
#include "stddef.h"

#include "added.h"

#include "I2Ccontrol.h"

#define WRITE_SET(adr) (adr)&0x0fe
#define READ_SET(adr) (adr)|0x01 

pstI2CTask currentI2C = NULL;

char PutI2CTask ( pstI2CTask task )
{
    if (currentI2C == NULL)
    {
        currentI2C = task;
        currentI2C->next = NULL;
        return 1;
    }
    else
    {
        if (currentI2C->next == NULL)
        {
            currentI2C->next = task;
            task->next=NULL;
            return 1;
        }
        else
            return 0;
    }
}

void I2C_Control( void )
{
    if (currentI2C != NULL)
    {
        
	  EALLOW;
      if(!StartI2C())
      {
        if (currentI2C->type==WRITE_I2C)
            currentI2C->adress=WRITE_SET(currentI2C->adress);
        else
        {
            if ((currentI2C->count > 0))
                currentI2C->adress=READ_SET(currentI2C->adress);
            else
                currentI2C->adress=WRITE_SET(currentI2C->adress);
        }
        
        if(!i2c_trsmt_byte(currentI2C->adress))
        {
          switch(currentI2C->type)
          {
            case WRITE_I2C:
            {
              while(currentI2C->count)
              {
                if(i2c_trsmt_byte(*currentI2C->data))
                  break;
                currentI2C->data++;
                currentI2C->count--;
              }
              break;
            }
            case READ_LN_I2C:
            {
              if(!i2c_trsmt_byte(*currentI2C->data))
                currentI2C->data++;
              else
                break;
            }
            case READ_I2C:
            {
              if(!i2c_trsmt_byte(*currentI2C->data))
              {
                currentI2C->data++;
                currentI2C->count= -currentI2C->count;
                if(ReStartI2C()) break;
                currentI2C->adress=READ_SET(currentI2C->adress);
                if(i2c_trsmt_byte(currentI2C->adress))
                  break;
                while(currentI2C->count)
                {
                  i2c_rcv_byte((UINT8*)currentI2C->data);
                  currentI2C->data++;
                  currentI2C->count--;
                  if(currentI2C->count)
                    i2c_set_ack(0);
                  else
                    i2c_set_ack(1);
                }
              }
            }
          }
          currentI2C->type=COMPLITE_OPERATION;
          currentI2C=currentI2C->next;
        }
        StopI2C();
      }
      EDIS;
    }
}

void InitI2C (void)
{
    InitI2CSoft();
}
