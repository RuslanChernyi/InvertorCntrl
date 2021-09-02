/* ******************************************************************
 * ������ ���������� �������
 * ******************************************************************/

#ifndef ___SYS_TIME_H___
#define ___SYS_TIME_H___

#include "added.h"

typedef UINT32 typeTIC;

typedef struct _TIME_TIC{
	typeTIC tic;
	struct _TIME_TIC *next;
}stTimeTic, *pTimeTic;

void initSysTime(void);
void initCountTime(void);
UINT32 getCountTimer(void);

char addedTimeTic(pTimeTic tic);
char removeTimeTic(pTimeTic tic);

#define INIT_DELAY(TASK)                \
    stTimeTic UCH_DELAY_##TASK;  \
    void SET_DELAY_##TASK ( typeTIC time ) \
    {                                   \
      UCH_DELAY_##TASK.tic = time;      \
    }                                   \
    void REM_DELAY_##TASK ( void )      \
    {                                   \
      removeTimeTic(&UCH_DELAY_##TASK); \
    }                                   \
    UINT32 GET_TIME_##TASK( void )      \
    {                                   \
        return UCH_DELAY_##TASK.tic;    \
    }                                   \
    char REG_TINE_##TASK(void)			\
    {									\
    	return addedTimeTic(&UCH_DELAY_##TASK);	\
    }//

#define DECKLARATE_DELAY(TASK)          \
    void SET_DELAY_##TASK ( typeTIC );     	\
    void REM_DELAY_##TASK ( void );     	\
    UINT32 GET_TIME_##TASK( void );//

#define SET_DELAY(TASK,TIME)            \
    SET_DELAY_##TASK ( TIME );          \

#define ADD_DELAY(TASK,TIME)            \
	REG_TINE_##TASK();					\
    SET_DELAY_##TASK ( TIME );          \

#define TEST_DELAY(TASK) (!GET_TIME_##TASK())
#define WHILE_DELAY(TASK) (GET_TIME_##TASK())
#define GET_TEST_DELAY(TASK) (!GET_TIME_##TASK())
#define REMOVE_DELAY(TASK) REM_DELAY_##TASK();

#define c2s 2000
#define c1sec 1000
#define c500ms 500
#define c250ms 250
#define c200ms 200
#define c100ms 100
#define c50ms 50
#define c20ms 20
#define c10ms 10
#define c1ms 1

#endif
