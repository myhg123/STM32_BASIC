/*
 * HM10_Setup.h
 *
 *  Created on: Jan 31, 2024
 *      Author: Andrianov Vitaly
 */
#ifndef INC_HM_10_HM10_SETUP_H_
#define INC_HM_10_HM10_SETUP_H_

#include "stm32l1xx_hal.h"
#include "AT_Command.h"
#include "HM10_Setup.h"
#include "utils/delay.h"
#include <string.h>
#include "stdlib.h"

#define delayUs 0x186A0						// default setup delay

#define default_mac_addr1 "D43639A63CD3"	// default device mac 1
#define default_mac_addr2 "D43639AB8B20"	// default device mac 2

typedef enum {
	OK,
	RENEW_OK,
	RESET_OK,
	LOST_AT,
	HM10_ERROR
} setup_result;

//---------------------------------------------------------------------------------------------------------------------------------------------//
	setup_result checkConnection(UART_HandleTypeDef *huart); 										// Check UART connection
	setup_result setBaudRate(UART_HandleTypeDef *huart, hm10_baud baudrate);						// set HM10 baudrate
	setup_result setRole(UART_HandleTypeDef *huart, hm10_role role);								// set HM10 role (master/slave)
	setup_result setImme(UART_HandleTypeDef *huart, hm10_imme imme);								// set working mode (at only/at + data)
	setup_result setName(UART_HandleTypeDef *huart, char * name);									// set HM10 BLE name
	setup_result setPower(UART_HandleTypeDef *huart, hm10_power power);								// set HM10 power
	setup_result setMode(UART_HandleTypeDef *huart, hm10_role mode);								// set HM10 working mode (when device con)
//---------------------------------------------------------------------------------------------------------------------------------------------//
	hm10_baud getBaudRate(UART_HandleTypeDef *huart);												// get current HM10 baudrate
	hm10_role getRole(UART_HandleTypeDef *huart);													// get current HM10 role
	hm10_imme getImme(UART_HandleTypeDef *huart);													// get current HM10 working mode
	void getAddr(UART_HandleTypeDef *huart);														// get current HM10 mac addr
	hm10_power getPower(UART_HandleTypeDef *huart);													// get current HM10 power
	hm10_mode getMode(UART_HandleTypeDef *huart);													// get current HM10 working mode (when con)
	void getTemp(UART_HandleTypeDef *huart,  char* temp_str);										// get connected HM10 temperature
	void getRSSI(UART_HandleTypeDef *huart,  char* rssi_str);										// get connected HM10 RSSI
//---------------------------------------------------------------------------------------------------------------------------------------------//
	setup_result renewDevice(UART_HandleTypeDef *huart);											// HM10 factory reset
	setup_result resetDevice(UART_HandleTypeDef *huart);											// reboot HM10
	setup_result startHM10(UART_HandleTypeDef *huart);												// start HM10 to transmit and receive data
//---------------------------------------------------------------------------------------------------------------------------------------------//
	setup_result setupSlave(UART_HandleTypeDef *huart, GPIO_TypeDef *brk_port, uint16_t brk_Pin);	// setup Slave mode
	setup_result setupMaster(UART_HandleTypeDef *huart, GPIO_TypeDef *brk_port, uint16_t brk_Pin);	// setup Master mode
//---------------------------------------------------------------------------------------------------------------------------------------------//
	hm10_connection_status connectOtherHM10(UART_HandleTypeDef *huart);								// connect to other (default mac addr) HM10
	hm10_connection_status connectToAddr(UART_HandleTypeDef *huart, char* addr);					// connect to mac (only in master mode)
//---------------------------------------------------------------------------------------------------------------------------------------------//
	char* concat_str(char * cmd, char mode);														// Concat string command and mode char
	char* concat_cmd_str(char * cmd, char * str);													// Concat string commands
	void clearingBuf();																				// Clear DMA receive buf
//---------------------------------------------------------------------------------------------------------------------------------------------//
#endif /* INC_HM10_SETUP_H_ */
