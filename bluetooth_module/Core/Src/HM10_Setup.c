/*
 * HM10_Setup.c
 *
 *  Created on: Jan 31, 2024
 *      Author: Andrianov Vitaly
 */

#include "HM10_Setup.h"

static char dma_res[30]; // DMA receive buffer

/**
  * @brief  Setup slave mode
  * @note   Control setup delay value
  * @param  Current HM10 huart
  * @param  Current HM10 brk port
  * @param  Current HM10 brk pin
  * @retval setup_result
  */
setup_result setupSlave(UART_HandleTypeDef *huart, GPIO_TypeDef *brk_port, uint16_t brk_Pin) {

	HAL_GPIO_WritePin(brk_port, brk_Pin, RESET);
	HAL_Delay(105);
	HAL_GPIO_WritePin(brk_port, brk_Pin, SET);
	HAL_Delay(105);

	setup_result connection = checkConnection(huart);
	if (connection != OK) {
		return HM10_ERROR;
	}
	usDelay(delayUs);

	renewDevice(huart);
	usDelay(delayUs);

	if (getPower(huart) != dbm_6) {
		if (setPower(huart, dbm_6) == HM10_ERROR) return HM10_ERROR;
	}
	usDelay(delayUs);

	if (getRole(huart) == MASTER) {
		if (setRole(huart, SLAVE) == HM10_ERROR) return HM10_ERROR;
	}
	usDelay(delayUs);

	if (getImme(huart) == ONLY_AT) {
		if (setImme(huart, BASE) == HM10_ERROR) return HM10_ERROR;
	}
	usDelay(delayUs);

	if (getMode(huart) != MODE_1) {
		if (setMode(huart, MODE_1) == HM10_ERROR) return HM10_ERROR;
	}
	usDelay(delayUs);

	resetDevice(huart);
	usDelay(delayUs);

	return OK;
}

/**
  * @brief  Setup master mode
  * @note   Control setup delay value
  * @param  Current HM10 huart
  * @param  Current HM10 brk port
  * @param  Current HM10 brk pin
  * @retval setup_result
  */
setup_result setupMaster(UART_HandleTypeDef *huart, GPIO_TypeDef *brk_port, uint16_t brk_Pin) {

	HAL_GPIO_WritePin(brk_port, brk_Pin, RESET);
	usDelay(2*delayUs);
	HAL_GPIO_WritePin(brk_port, brk_Pin, SET);
	usDelay(2*delayUs);

	setup_result connection = checkConnection(huart);
	if (connection != OK) {
		return HM10_ERROR;
	}
	usDelay(delayUs);

	renewDevice(huart);
	usDelay(delayUs);

	if (getPower(huart) != dbm_6) {
		if (setPower(huart, dbm_6) == HM10_ERROR) return HM10_ERROR;
	}

	if (getRole(huart) == SLAVE) {
		if (setRole(huart, MASTER) == HM10_ERROR) return HM10_ERROR;
	}
	usDelay(delayUs);

	if (getImme(huart) == BASE) {
		if (setImme(huart, ONLY_AT) == HM10_ERROR) return HM10_ERROR;
	}
	usDelay(delayUs);

	if (getMode(huart) != MODE_1) {
		if (setMode(huart, MODE_1) == HM10_ERROR) return HM10_ERROR;
	}
	usDelay(delayUs);

	resetDevice(huart);
	usDelay(delayUs);

	return OK;
}

/**
  * @brief  Connect device to mac addr
  * @note   Only master mode and only default mac addr (check header)
  * @param  Current HM10 huart
  * @retval setup_result hm10_connection_status
  */
hm10_connection_status connectOtherHM10(UART_HandleTypeDef *huart) {
	clearingBuf();

	if (getRole(huart) == SLAVE) {
		return disconnected;
	}
	usDelay(delayUs);

	clearingBuf();

	getAddr(huart);
	usDelay(delayUs);

	char *token = strtok(dma_res, ":");
	token = strtok(NULL, ":");
	if (strcmp (token, default_mac_addr1) == 0) {
		token = default_mac_addr2;
	}
	else if (strcmp (token, default_mac_addr2) == 0) {
		token = default_mac_addr1;
	}
	else {
		return disconnected;
	}
	clearingBuf();
	while (connectToAddr(huart, token) != connected) {
		clearingBuf();
	}

	return connected;
}

/**
  * @brief  Connect device to mac addr
  * @note   Only master mode
  * @param  Current HM10 huart
  * @param  Mac addr slave device
  * @retval setup_result hm10_connection_status
  */
hm10_connection_status connectToAddr(UART_HandleTypeDef *huart, char* addr) {

	char* tx_cmd = concat_cmd_str((char *) getCommand(CONN), addr);

	HAL_UART_Receive_DMA(huart, (uint8_t *) dma_res, getResLength(CONN));
	HAL_UART_Transmit(huart, (uint8_t *) tx_cmd, strlen(tx_cmd), 0xFFFF);

	HAL_Delay(250);

	if (strcmp ( dma_res, "OK+CONNA\r\nOK+CONN\r\n" ) != 0) {
		return disconnected;
	}

	return connected;
}

/**
  * @brief  Check connection (sending AT command)
  * @note   Control current uart baudrate
  * @param  Current HM10 huart
  * @retval setup_result
  */
setup_result checkConnection(UART_HandleTypeDef *huart) {
	clearingBuf();

	HAL_UART_Receive_DMA(huart, (uint8_t *) dma_res, getResLength(AT));
	HAL_UART_Transmit(huart, getCommand(AT), strlen((char *) getCommand(AT)), 0xFFFF);

	usDelay(delayUs);

	if (strcmp (dma_res, "OK") != 0) return LOST_AT;

	return OK;
}

/**
  * @brief  Set HM10 baudrate
  * @note   After this fun need to reset (reboot) HM10
  * @param  Current HM10 huart
  * @param  hm10_baud
  * @retval setup_result
  */
setup_result setBaudRate(UART_HandleTypeDef *huart, hm10_baud baudrate) {
	clearingBuf();

	char tx_baud = baudrate + '0';
	char* tx_cmd = concat_str((char*) getCommand(BAUD_SET), tx_baud);

	HAL_UART_Receive_DMA(huart, (uint8_t *) dma_res, getResLength(BAUD_SET));
	HAL_UART_Transmit(huart, (uint8_t *) tx_cmd, strlen(tx_cmd), 0xFFFF);

	usDelay(delayUs);

	free(tx_cmd);
	int tx_res = dma_res[getResLength(BAUD_SET) - 1] - '0';
	if (tx_res != 0 && tx_res != 1 && tx_res != 2 && tx_res != 3 && tx_res != 4 &&
		tx_res != 5 && tx_res != 6 && tx_res != 7 && tx_res != 8) return HM10_ERROR;

	return OK;
}

/**
  * @brief  Set HM10 role
  * @note   After this fun need to reset (reboot) HM10
  * @param  Current HM10 huart
  * @param  hm10_role
  * @retval setup_result
  */
setup_result setRole(UART_HandleTypeDef *huart, hm10_role role) {
	clearingBuf();

	char tx_role = role + '0';
	char* tx_cmd = concat_str((char*) getCommand(ROLE_SET), tx_role);

	HAL_UART_Receive_DMA(huart, (uint8_t *) dma_res, getResLength(ROLE_SET));
	HAL_UART_Transmit(huart, (uint8_t *) tx_cmd, strlen(tx_cmd), 0xFFFF);

	usDelay(delayUs);

	free(tx_cmd);

	if (strcmp (dma_res, "OK+Set:1") != 0 && strcmp (dma_res, "OK+Set:0") != 0) return HM10_ERROR;

	return OK;
}

/**
  * @brief  Set HM10 imme mode (at only/at+data)
  * @note   After this fun need to reset (reboot) HM10
  * @param  Current HM10 huart
  * @param  hm10_imme mode
  * @retval setup_result
  */
setup_result setImme(UART_HandleTypeDef *huart, hm10_imme imme) {
	clearingBuf();

	char tx_imme = imme + '0';
	char* tx_cmd = concat_str((char*) getCommand(IMME_SET), tx_imme);

	HAL_UART_Receive_DMA(huart, (uint8_t *) dma_res, getResLength(IMME_SET));
	HAL_UART_Transmit(huart, (uint8_t *) tx_cmd, strlen(tx_cmd), 0xFFFF);

	usDelay(delayUs);

	free(tx_cmd);

	if (strcmp (dma_res, "OK+Set:1") != 0 && strcmp (dma_res, "OK+Set:0") != 0) return HM10_ERROR;

	return OK;
}

/**
  * @brief  Set HM10 BLE name
  * @param  Current HM10 huart
  * @param  name string
  * @retval setup_result
  */
setup_result setName(UART_HandleTypeDef *huart, char * name) {
	clearingBuf();

	char* tx_cmd = concat_cmd_str((char *) getCommand(NAME_SET), name);

	HAL_UART_Receive_DMA(huart, (uint8_t *) dma_res, getResLength(NAME_SET) + strlen(name));
	HAL_UART_Transmit(huart, (uint8_t *) tx_cmd, strlen(tx_cmd), 0xFFFF);

	usDelay(delayUs);

	free(tx_cmd);

	if (strcmp (dma_res, concat_cmd_str("OK+Set", name)) != 0) return HM10_ERROR;

	return OK;
}

/**
  * @brief  Set HM10 power
  * @note   After this fun need to reset (reboot) HM10
  * @param  Current HM10 huart
  * @param  hm10_power
  * @retval setup_result
  */
setup_result setPower(UART_HandleTypeDef *huart, hm10_power power) {
	clearingBuf();

	char tx_power = power + '0';
	char* tx_cmd = concat_str((char*) getCommand(POWER_SET), tx_power);

	HAL_UART_Receive_DMA(huart, (uint8_t *) dma_res, getResLength(POWER_SET));
	HAL_UART_Transmit(huart, (uint8_t *) tx_cmd, strlen(tx_cmd), 0xFFFF);

	usDelay(delayUs);

	free(tx_cmd);
	int tx_res = dma_res[getResLength(POWER_SET) - 1] - '0';
	if (tx_res != 0 && tx_res != 1 && tx_res != 2 && tx_res != 3) return HM10_ERROR;

	return OK;
}

/**
  * @brief  Set HM10 working mode (when device connected)
  * @note   After this fun need to reset (reboot) HM10
  * @param  Current HM10 huart
  * @param  hm10_mode
  * @retval setup_result
  */
setup_result setMode(UART_HandleTypeDef *huart, hm10_role mode) {
	clearingBuf();

	char tx_mode = mode + '0';
	char* tx_cmd = concat_str((char*) getCommand(MODE_SET), tx_mode);

	HAL_UART_Receive_DMA(huart, (uint8_t *) dma_res, getResLength(MODE_SET));
	HAL_UART_Transmit(huart, (uint8_t *) tx_cmd, strlen(tx_cmd), 0xFFFF);

	usDelay(delayUs);

	free(tx_cmd);

	if (
			strcmp (dma_res, "OK+Set:0") != 0 &&
			strcmp (dma_res, "OK+Set:1") != 0 &&
			strcmp (dma_res, "OK+Set:2") != 0
		) return HM10_ERROR;

	return OK;
}

/**
  * @brief  Get HM10 baudrate mode
  * @param  Current HM10 huart
  * @retval hm10_baud
  */
hm10_baud getBaudRate(UART_HandleTypeDef *huart) {
	clearingBuf();

	HAL_UART_Receive_DMA(huart, (uint8_t *) dma_res, getResLength(ROLE_GET));
	HAL_UART_Transmit(huart, getCommand(BAUD_GET), strlen((char *) getCommand(BAUD_GET)), 0xFFFF);

	usDelay(delayUs);

	switch(dma_res[getResLength(BAUD_GET) - 1]) {
		case BAUD_9600 + '0':
			return BAUD_9600;
		case BAUD_19200 + '0':
			return BAUD_19200;
		case BAUD_38400 + '0':
			return BAUD_38400;
		case BAUD_57600 + '0':
			return BAUD_57600;
		default:
			return BAUD_115200;
	}

	return BAUD_115200;
}

/**
  * @brief  Get HM10 role
  * @param  Current HM10 huart
  * @retval hm10_role
  */
hm10_role getRole(UART_HandleTypeDef *huart) {
	clearingBuf();

	HAL_UART_Receive_DMA(huart, (uint8_t *) dma_res, getResLength(ROLE_GET));
	HAL_UART_Transmit(huart, getCommand(ROLE_GET), strlen((char *) getCommand(ROLE_GET)), 0xFFFF);

	usDelay(delayUs);

	if (dma_res[getResLength(ROLE_GET) - 1] == SLAVE + '0') return SLAVE;

	return MASTER;
}

/**
  * @brief  Get HM10 Imme mode
  * @param  Current HM10 huart
  * @retval hm10_imme
  */
hm10_imme getImme(UART_HandleTypeDef *huart) {
	clearingBuf();

	HAL_UART_Receive_DMA(huart, (uint8_t *) dma_res, getResLength(IMME_GET));
	HAL_UART_Transmit(huart, getCommand(IMME_GET), strlen((char *) getCommand(IMME_GET)), 0xFFFF);

	usDelay(delayUs);

	if (dma_res[getResLength(IMME_GET) - 1] == BASE + '0') return BASE;

	return ONLY_AT;
}

/**
  * @brief  Get current device mac addr
  * @param  Current HM10 huart
  * @retval void
  */
void getAddr(UART_HandleTypeDef *huart) {
	clearingBuf();

	HAL_UART_Receive_DMA(huart, (uint8_t *) dma_res, getResLength(ADDR));
	HAL_UART_Transmit(huart, getCommand(ADDR), strlen((char *) getCommand(ADDR)), 0xFFFF);

	usDelay(delayUs);
}

/**
  * @brief  Get current power HM10 device
  * @param  Current HM10 huart
  * @retval hm10_power
  */
hm10_power getPower(UART_HandleTypeDef *huart) {
	clearingBuf();

	HAL_UART_Receive_DMA(huart, (uint8_t *) dma_res, getResLength(POWER_GET));
	HAL_UART_Transmit(huart, getCommand(POWER_GET), strlen((char *) getCommand(POWER_GET)), 0xFFFF);

	usDelay(delayUs);

	switch(dma_res[getResLength(POWER_GET) - 1]) {
		case dbm_m23 + '0':
			return dbm_m23;
		case dbm_m6 + '0':
			return dbm_m6;
		case dbm_0 + '0':
			return dbm_0;
		default:
			return dbm_6;
	}
	return dbm_0;
}

/**
  * @brief  Get HM10 working mode (when device connected)
  * @param  Current HM10 huart
  * @retval hm10_mode
  */
hm10_mode getMode(UART_HandleTypeDef *huart) {
	clearingBuf();

	HAL_UART_Receive_DMA(huart, (uint8_t *) dma_res, getResLength(MODE_GET));
	HAL_UART_Transmit(huart, getCommand(MODE_GET), strlen((char *) getCommand(MODE_GET)), 0xFFFF);

	usDelay(delayUs);

	if (dma_res[getResLength(MODE_GET) - 1] == BASE + '0') return MODE_0;
	else if (dma_res[getResLength(MODE_GET) - 1] == BASE + '1') return MODE_1;

	return MODE_2;
}

/**
  * @brief  Get connected HM10 temperature
  * @note   Need mode1 or mode2
  * @param  Current HM10 huart
  * @param  result temp value string
  * @retval void
  */
void getTemp(UART_HandleTypeDef *huart, char* temp_str) {
	clearingBuf();

	uint8_t dotFlag = 0;
	uint8_t currentChar = 0;

	HAL_UART_Receive_DMA(huart, (uint8_t *) dma_res, getResLength(TEMP_GET));
	HAL_UART_Transmit(huart, getCommand(TEMP_GET), strlen((char *) getCommand(TEMP_GET)), 0xFFFF);

	usDelay(delayUs);

	for (uint8_t i=0; i < strlen(dma_res); i++) {
		if (currentChar == strlen(temp_str) - 1) break;
		else if (i > 0 && dma_res[i - 1] == ':') dotFlag = 1;

		if (dotFlag) {
			if (dma_res[i] == '0' && currentChar == 0) {

			}
			else {
				temp_str[currentChar] = dma_res[i];
				currentChar++;
			}
		}
	}
}

/**
  * @brief  Get connected HM10 RSSI (received signal strength indicator)
  * @note   Need mode1 or mode2
  * @param  Current HM10 huart
  * @param  result rssi value string
  * @retval void
  */
void getRSSI(UART_HandleTypeDef *huart, char* rssi_str) {
	clearingBuf();

	uint8_t dotFlag = 0;
	uint8_t currentChar = 0;

	HAL_UART_Receive_DMA(huart, (uint8_t *) dma_res, getResLength(RSSI_GET));
	HAL_UART_Transmit(huart, getCommand(RSSI_GET), strlen((char *) getCommand(RSSI_GET)), 0xFFFF);

	usDelay(delayUs);

	for (uint8_t i=0; i < strlen(dma_res); i++) {
		if (currentChar == strlen(rssi_str) - 1) break;
		else if (dma_res[i] == ':') dotFlag = 1;

		if (dotFlag) {
			rssi_str[currentChar] = dma_res[i];
			currentChar++;
		}
	}
}

/**
  * @brief  Factory reset HM10
  * @param  Current HM10 huart
  * @retval setup_result
  */
setup_result renewDevice(UART_HandleTypeDef *huart) {
	clearingBuf();

	HAL_UART_Receive_DMA(huart, (uint8_t *) dma_res, getResLength(RENEW));
	HAL_UART_Transmit(huart, getCommand(RENEW), strlen((char*) getCommand(RENEW)), 0xFFFF);

	usDelay(delayUs);

	if (strcmp (dma_res, "OK+RENEW") != 0) return HM10_ERROR;

	return OK;
}

/**
  * @brief  Reboot (reset) HM10
  * @param  Current HM10 huart
  * @retval setup_result
  */
setup_result resetDevice(UART_HandleTypeDef *huart) {
	clearingBuf();

	HAL_UART_Receive_DMA(huart, (uint8_t *) dma_res, getResLength(RESET1));
	HAL_UART_Transmit(huart, getCommand(RESET1), strlen((char*) getCommand(RESET1)), 0xFFFF);

	usDelay(delayUs);

	if (strcmp (dma_res, "OK+RESET") != 0) return HM10_ERROR;

	return OK;
}

/**
  * @brief  Start HM10 to transmit/receive data
  * @param  Current HM10 huart
  * @retval setup_result
  */
setup_result startHM10(UART_HandleTypeDef *huart) {
	clearingBuf();

	HAL_UART_Receive_DMA(huart, (uint8_t *) dma_res, getResLength(START));
	HAL_UART_Transmit(huart, getCommand(START), strlen((char*) getCommand(START)), 0xFFFF);

	usDelay(delayUs);

	if (strcmp (dma_res, "OK") != 0) return HM10_ERROR;

	return OK;
}

/**
  * @brief  Concat string command and mode char
  * @param  current command
  * @param  current mode
  * @retval concat command
  */
char* concat_str(char * cmd, char mode) {
	const size_t len1 = strlen(cmd);

	char *result = (char *) malloc(len1 + 4);

	strcpy(result, cmd);
	result[len1] = mode;
	result[len1+1] = '\r';
	result[len1+2] = '\n';
	result[len1+3] = '\0';

	return result;
}

/**
  * @brief  Concat string command
  * @param  current command
  * @param  current command value
  * @retval concat command
  */
char* concat_cmd_str(char * cmd, char * str) {
	const size_t len_cmd = strlen(cmd);
	const size_t len_str = strlen(str);

	char *result = malloc(len_cmd + len_str + 4);

	strcpy(result, cmd);
	strcat(result, str);
	strcat(result, "\r");
	strcat(result, "\n");
	strcat(result, "\0");

	return result;
}

/**
  * @brief  Clearing DMA receive buffer
  * @param	void
  * @retval void
  */
void clearingBuf() {
	memset(dma_res, 0, 30);
}
