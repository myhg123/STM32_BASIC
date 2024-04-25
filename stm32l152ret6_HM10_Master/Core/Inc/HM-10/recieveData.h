/*
 * recieveData.h
 *
 *  Created on: 25 янв. 2024 г.
 *      Author: Andrianov Vitaly
 */

#ifndef INC_RECIEVEDATA_H_
#define INC_RECIEVEDATA_H_

#include "stm32l1xx_hal.h"
#include "main.h"
#include "st7789/st7789.h"
#include <string.h>
#include <stdio.h>

#define 	rxBuf_SIZE		128
#define		subBuf_SIZE		256

extern DMA_HandleTypeDef hdma_uart4_rx;

//--------------------------------------------------------------------------------------------------------------//
	void UART4_RxCpltCallback(UART_HandleTypeDef *huart, uint16_t Size);	// receive data post proc
	void clearingRXBuf();													// clearing receive data buffer
	void connEvent();														// successful connection event
	void connLostEvent();													// lost connection event
	void getTempRssiEvent();												// get Temperature or RSSI post proc
	void getMsgEvent(UART_HandleTypeDef *huart);							// get message event
//--------------------------------------------------------------------------------------------------------------//

#endif /* INC_RECIEVEDATA_H_ */
