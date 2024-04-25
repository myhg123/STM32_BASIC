/*
 * buzzer.c
 *
 *  Created on: Apr 15, 2024
 *      Author: kccistc
 */

#include "buzzer.h"

int soundLUT[] = {1046, 1175, 1318, 1397, 1568, 1760, 1975, 2092};

void initSound(TIM_HandleTypeDef *htim3){
	HAL_TIM_PWM_Start(htim3, TIM_CHANNEL_4);
	stopSound(htim3);
}

void setSound(int freq , TIM_HandleTypeDef *htim3){
	htim3->Instance->ARR = 1000000/freq - 1;
	htim3->Instance->CCR4 = htim3->Instance->ARR / 2;
}

void hornSound(TIM_HandleTypeDef *htim3){
	setSound(400, htim3);
}

void safeSound(TIM_HandleTypeDef *htim3, uint8_t state){
	static uint32_t before_Tick=0;
	static uint8_t flag = 0;
	switch (state) {
		case WARNING:
			if(HAL_GetTick() - before_Tick>=100){
				before_Tick = HAL_GetTick();
				if(flag==0){
					setSound(600, htim3);
					flag = 1;
				}
				else{
					stopSound(htim3);
					flag = 0;
				}
			}
			break;
		case DANGER:
			if(HAL_GetTick() - before_Tick>=10){
				before_Tick = HAL_GetTick();
				if(flag==0){
					setSound(2200, htim3);
					flag = 1;
				}
				else{
					stopSound(htim3);
					flag = 0;
				}
			}
			break;
		default:
			stopSound(htim3);
			break;
	}
}

void stopSound(TIM_HandleTypeDef *htim3){
	htim3->Instance->CCR4 = 0;
}

void manualModeSound(TIM_HandleTypeDef *htim3){
	setSound(soundLUT[0], htim3);
	HAL_Delay(150);
	setSound(soundLUT[2], htim3);
	HAL_Delay(150);
	setSound(soundLUT[4], htim3);
	HAL_Delay(150);
	setSound(soundLUT[7], htim3);
	HAL_Delay(150);
}

void autoModeSound(TIM_HandleTypeDef *htim3){
	setSound(soundLUT[7], htim3);
	HAL_Delay(150);
	setSound(soundLUT[4], htim3);
	HAL_Delay(150);
	setSound(soundLUT[2], htim3);
	HAL_Delay(150);
	setSound(soundLUT[0], htim3);
	HAL_Delay(150);
}

void suddenlyStopSound(TIM_HandleTypeDef *htim3){
	setSound(soundLUT[2], htim3);
	HAL_Delay(100);
	setSound(soundLUT[0], htim3);
	HAL_Delay(100);
	setSound(soundLUT[2], htim3);
	HAL_Delay(100);
	setSound(soundLUT[0], htim3);
	HAL_Delay(100);
	setSound(soundLUT[2], htim3);
	HAL_Delay(100);
	setSound(soundLUT[0], htim3);
	HAL_Delay(100);
}
