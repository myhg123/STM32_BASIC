/*
 * uart.h
 *
 *  Created on: Mar 11, 2024
 *      Author: myhg1
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "main.h"

void initUart(UART_HandleTypeDef *inHuart);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
uint8_t getChar();
char getString();
int _write(int file, char *p, int len);


#endif /* INC_UART_H_ */
