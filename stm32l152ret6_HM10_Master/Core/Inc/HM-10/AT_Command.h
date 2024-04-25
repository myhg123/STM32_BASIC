/*
 * AT_Command.h
 *
 *  Created on: Jan 30, 2024
 *      Author: Andrianov Vitaly
 */

#ifndef INC_AT_COMMAND_H_
#define INC_AT_COMMAND_H_

#include "stm32l1xx_hal.h"

/**
  * available HM10 commands
  */
typedef enum {
	AT,
	RESET1,
	RENEW,
	BAUD_GET,
	BAUD_SET,
	CONN,
	IMME_GET,
	IMME_SET,
	NAME_GET,
	NAME_SET,
	ROLE_GET,
	ROLE_SET,
	START,
	ADDR,
	POWER_GET,
	POWER_SET,
	MODE_GET,
	MODE_SET,
	TEMP_GET,
	RSSI_GET
} AT_commands;

/**
  * available HM10 baudrate
 */
typedef enum {
	BAUD_9600,
	BAUD_19200,
	BAUD_38400,
	BAUD_57600,
	BAUD_115200
} hm10_baud;

/**
  * available HM10 role
 */
typedef enum {
	SLAVE,
	MASTER
} hm10_role;

/**
  * available HM10 work (imme0 = data + at cmds, imme1 = only at cmds)
 */
typedef enum {
	BASE,
	ONLY_AT
} hm10_imme;

/**
  * available HM10 power (-23dbm, -6, 0, 6)
 */
typedef enum {
	dbm_m23,
	dbm_m6,
	dbm_0,
	dbm_6,
} hm10_power;

/**
  * HM10 working mode when connected:
  * 0 - Only data,
  * 1 - data + at from ble connected device,
  * 2 - data + at + pio
 */
typedef enum {
	MODE_0,
	MODE_1,
	MODE_2,
} hm10_mode;

/**
  * connection to other device result
 */
typedef enum {
	connected,
	disconnected
} hm10_connection_status;

/**
  * AT command string and result length
 */
struct AT_command {
	char * command;
	int res_len;
};

uint8_t * getCommand(AT_commands command);	// get Command from array
uint8_t getResLength(AT_commands command);	// get result length from array

#endif /* INC_AT_COMMAND_H_ */
