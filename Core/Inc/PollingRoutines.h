/*
 * PollingRoutines.h
 *
 *  Created on: Feb 15, 2023
 *      Author: 90531
 */

#ifndef INC_POLLINGROUTINES_H_
#define INC_POLLINGROUTINES_H_
#ifdef __cplusplus
extern "C" {
#endif


#define UART_BUF_SIZE 32
#define uartMsgData_SIZE 16
void dataDisplay(void);

void PollingInit();
void PollingRoutine();


#ifdef __cplusplus
}
#endif
#endif /* SRC_POLLINGROUTINES_H_ */
