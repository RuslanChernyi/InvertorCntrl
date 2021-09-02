/*
 * �������� ��������� ModBus
 */

#include "SysTimer/sysTime.h"
#include "UART/cntrlUartTransfer.h"
#include "modbus.h"
#include "CommandSlave.h"
#include <stddef.h>

#define UARTIntGetRxBufferDataSize() (testRcBufCnt())
#define UARTIntGetChar getCharUart
#define UARTIntPutChar putCharUart

#define MAX_CNT_MBUS 256

enum FLOW_RX_STATE {START_WAIT, END_0D, END_0A};

enum FLOW_RX_STATE State_RX;
char RX_MSG[MAX_CNT_MBUS];

char cnt;

INIT_DELAY(MBUS_DL)

void InitFlow (void)
{
    MirrorAdrMbus();
    initUartBufer();
    State_RX=START_WAIT;
}

void initMbusFlow(void){
	ADD_DELAY(MBUS_DL, 0);
	InitFlow();
    putCharUart('O');
    putCharUart('K');
    putCharUart(0x0d);
    putCharUart(0x0a);
}

void UartFlowControl(char *rc)
{
    char tmp;
    if(rc != NULL)
    {
        tmp = *rc;
        switch (State_RX)
        {
            case START_WAIT:
            {
                if (tmp == _START)
                {
                    RX_MSG[0]=tmp;
                    cnt=0;
                    State_RX=END_0D;
                    SET_DELAY(MBUS_DL, c1sec)
                }
                break;
            }
            case END_0D:
            {
                if(tmp==_START)
                    State_RX=START_WAIT;
                if(tmp==STOP_2)
                    State_RX=START_WAIT;

                if(tmp==STOP_1)
                {
                    if(cnt < MAX_CNT_MBUS)
                    {
                        cnt++;
                        RX_MSG[cnt]=STOP_1;
                        State_RX=END_0A;
                    }
                }
                else
                {
                    if (cnt<MAX_CNT_MBUS)
                    {
                        cnt++;
                        RX_MSG[cnt]=tmp;
                    }
                    else
                        State_RX=START_WAIT;
                }
                break;
            }
            case END_0A:
            {
            	if(tmp != STOP_2)
                {
                    State_RX=START_WAIT;
                    break;
                }
                cnt++;
                RX_MSG[cnt]=tmp;

                if (DeCompileMessageSlv(&RX_MSG[0], &cnt) == _GOOD)
                {
                    register char cntTX=0;
                    while(cnt)
                    {
                        UARTIntPutChar( RX_MSG[cntTX] );
                        cnt--; cntTX++;
                    }
                    while(testRcBufCnt()){
                        getCharUart(&cnt);
                    }
                }
                State_RX=START_WAIT;
                break;
            }
            default :
                State_RX=START_WAIT;
        }
    }else{
    	if(TEST_DELAY(MBUS_DL)){
    		if(cnt){
    			cnt = 0;
    			tmp = 0;
    		}
    		State_RX=START_WAIT;
    	}
    }
}
