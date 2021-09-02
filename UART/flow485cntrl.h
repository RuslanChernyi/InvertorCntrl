/*
 * flow485cntrl.h
 *
 *  Created on: 6 квіт. 2015
 *      Author: Tester
 */

#ifndef UART_FLOW485CNTRL_H_
#define UART_FLOW485CNTRL_H_

void initFlow485Cntrl(void);
void cntrFlow485(void);
void chengFlow485cntrl(void(*cntrl)(void));

#endif /* UART_FLOW485CNTRL_H_ */
