/*
 * uart.c
 *
 *  Created on: Mar 11, 2024
 *      Author: myhg1
 */

#include "uart.h"
#include <stdio.h>
UART_HandleTypeDef *myHuart;

#define rxBufferMax 255

int rxBufferGp; //get pointer(read)
int rxBufferPp; // put pointer (write)
uint8_t rxBuffer[rxBufferMax];
uint8_t rxChar;

// init device
void initUart(UART_HandleTypeDef *inHuart){
	myHuart = inHuart;
	HAL_UART_Receive_IT(myHuart, &rxChar, 1);
}

// process received charactor
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	rxBuffer[rxBufferPp++]=rxChar;
	rxBufferPp %= rxBufferMax;
	HAL_UART_Receive_IT(myHuart, &rxChar, 1);
}

// get charator from buffer
uint8_t getChar(){
	uint8_t result;
	if(rxBufferGp == rxBufferPp) return 0;
	result = rxBuffer[rxBufferGp++];
	rxBufferGp %= rxBufferMax;
	return result;
}

//바이너리 데이터 전송
void binaryTransmit(protocol_t inData){
	uint8_t txBuffer[]={STX,0,0,0,0,0,0,0,ETX};
	//데이터 복사
	//memcpy(&txBuffer[1],&inData, 6);
	txBuffer[1] = inData.id | 0x80;
	txBuffer[2] = inData.command | 0x80;
	txBuffer[3] = inData.data | 0x80;
	txBuffer[4] = inData.data>>7 | 0x80;
	txBuffer[5] = inData.data>>14 | 0x80;
	txBuffer[6] = inData.data>>21 | 0x80;
	//CRC 계산
	for(int i=0;i<7;i++)
		txBuffer[7]+=txBuffer[i];
	//전송
	HAL_UART_Transmit(myHuart, txBuffer, sizeof(txBuffer), 10);

}



int _write(int file, char *p, int len) {
	HAL_UART_Transmit(myHuart, p, len, 10);
	return len;
}
