/*
 * Flow232Cntrl.h
 *
 *  Created on: 31 марта 2017 г.
 *      Author: test
 */

#ifndef UART_FLOW232CNTRL_H_
#define UART_FLOW232CNTRL_H_

void initFlow232Cntrl(void);
void cntrFlow232(void);
void chengFlow232cntrl(void(*cntrl)(void));

void cntrlModbuAndShellRs232(void);
void initModbusAndShell232(void);

#endif /* UART_FLOW232CNTRL_H_ */
