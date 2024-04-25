/*
 * led.c
 *
 *  Created on: Apr 15, 2024
 *      Author: kccistc
 */

#include "led.h"

void LED_init(){
	suddenlyModeLed();
}

void LED_OFF(){
	HAL_GPIO_WritePin(manual_LED_GPIO_Port, manual_LED_Pin, 0);
	HAL_GPIO_WritePin(auto_LED_GPIO_Port, auto_LED_Pin, 0);
	HAL_GPIO_WritePin(stop_LED_GPIO_Port, stop_LED_Pin, 0);
}

void manualModeLed(){
	HAL_GPIO_WritePin(manual_LED_GPIO_Port, manual_LED_Pin, 1);
	HAL_GPIO_WritePin(auto_LED_GPIO_Port, auto_LED_Pin, 0);
	HAL_GPIO_WritePin(stop_LED_GPIO_Port, stop_LED_Pin, 0);
}

void autoModeLed(){
	HAL_GPIO_WritePin(manual_LED_GPIO_Port, manual_LED_Pin, 1);
	HAL_GPIO_WritePin(auto_LED_GPIO_Port, auto_LED_Pin, 1);
	HAL_GPIO_WritePin(stop_LED_GPIO_Port, stop_LED_Pin, 0);
}

void suddenlyModeLed(){
	HAL_GPIO_WritePin(manual_LED_GPIO_Port, manual_LED_Pin, 0);
	HAL_GPIO_WritePin(auto_LED_GPIO_Port, auto_LED_Pin, 0);
	HAL_GPIO_WritePin(stop_LED_GPIO_Port, stop_LED_Pin, 1);
}


