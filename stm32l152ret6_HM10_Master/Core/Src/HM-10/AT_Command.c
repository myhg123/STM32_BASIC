/*
 * AT_Command.c
 *
 *  Created on: Jan 30, 2024
 *      Author: Andrianov Vitaly
 */

#include "HM-10/AT_command.h"

/**
  * available AT_command array
 */
struct AT_command AT_command_list[] = {
		{"AT", 2},
		{"AT+RESET\r\n", 8},
		{"AT+RENEW\r\n", 8},
		{"AT+BAUD?\r\n", 8},
		{"AT+BAUD", 8},
		{"AT+CON", 19},
		{"AT+IMME?\r\n", 8},
		{"AT+IMME", 8},
		{"AT+NAME?\r\n", -1},
		{"AT+NAME", -1},
		{"AT+ROLE?\r\n", 8},
		{"AT+ROLE", 8},
		{"AT+START\r\n", 8},
		{"AT+ADDR?\r\n", 20},
		{"AT+POWE?\r\n", 8},
		{"AT+POWE", 8},
		{"AT+MODE?\r\n", 8},
		{"AT+MODE", 8},
		{"AT+TEMP?", 13},
		{"AT+RSSI?", 12},
};

/**
  * @brief  Get at command from array
  * @param  Current HM10 cmd
  * @retval uint8_t * command
  */
uint8_t * getCommand(AT_commands command) {
	return (uint8_t *) AT_command_list[command].command;
}

/**
  * @brief  Get at command result length from array
  * @param  Current HM10 cmd
  * @retval uint8_t
  */
uint8_t getResLength(AT_commands command) {
	return AT_command_list[command].res_len;
}
