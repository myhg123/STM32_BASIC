/*
 * uart.c
 *
 *  Created on: Mar 11, 2024
 *      Author: myhg1
 */

#include "uart.h"
#include <stdio.h>
UART_HandleTypeDef *myHuart1;
UART_HandleTypeDef *myHuart2;
UART_HandleTypeDef *myHuart3;

#define rxBufferMax 255

int rxBufferGp1; //get pointer(read)
int rxBufferPp1; // put pointer (write)
uint8_t rxBuffer1[rxBufferMax];
uint8_t rxChar1;
int rxBufferGp2; //get pointer(read)
int rxBufferPp2; // put pointer (write)
uint8_t rxBuffer2[rxBufferMax];
uint8_t rxChar2;
int rxBufferGp3; //get pointer(read)
int rxBufferPp3; // put pointer (write)
uint8_t rxBuffer3[rxBufferMax];
uint8_t rxChar3;

// init device
void initUart1(UART_HandleTypeDef *inHuart) {
	myHuart1 = inHuart;
	HAL_UART_Receive_IT(myHuart1, &rxChar1, 1);
}
void initUart2(UART_HandleTypeDef *inHuart) {
	myHuart2 = inHuart;
	HAL_UART_Receive_IT(myHuart2, &rxChar2, 1);
}
void initUart3(UART_HandleTypeDef *inHuart) {
	myHuart3 = inHuart;
	HAL_UART_Receive_IT(myHuart3, &rxChar3, 1);
}


// process received charactor
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart == myHuart1) {
		rxBuffer1[rxBufferPp1++] = rxChar1;
		rxBufferPp1 %= rxBufferMax;
		HAL_UART_Receive_IT(myHuart1, &rxChar1, 1);
	}
	if (huart == myHuart2) {
		rxBuffer2[rxBufferPp2++] = rxChar2;
		rxBufferPp2 %= rxBufferMax;
		HAL_UART_Receive_IT(myHuart2, &rxChar2, 1);
	}
	if (huart == myHuart3) {
		rxBuffer3[rxBufferPp3++] = rxChar3;
		rxBufferPp3 %= rxBufferMax;
		HAL_UART_Receive_IT(myHuart3, &rxChar3, 1);
	}
}

// get charator from buffer
uint8_t getChar1() {
	uint8_t result;
	if (rxBufferGp1 == rxBufferPp1)
		return 0;
	result = rxBuffer1[rxBufferGp1++];
	rxBufferGp1 %= rxBufferMax;
	return result;
}

uint8_t getChar2() {
	uint8_t result;
	if (rxBufferGp2 == rxBufferPp2)
		return 0;
	result = rxBuffer2[rxBufferGp2++];
	rxBufferGp2 %= rxBufferMax;
	return result;
}

uint8_t getChar3() {
	uint8_t result;
	if (rxBufferGp3 == rxBufferPp3)
		return 0;
	result = rxBuffer3[rxBufferGp3++];
	rxBufferGp3 %= rxBufferMax;
	return result;
}

int _write(int file, char *p, int len) {
	HAL_UART_Transmit(myHuart2, p, len, 10);
	return len;
}
