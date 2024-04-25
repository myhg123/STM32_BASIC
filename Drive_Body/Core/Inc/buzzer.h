/*
 * buzzer.h
 *
 *  Created on: Apr 15, 2024
 *      Author: kccistc
 */

#include "main.h"
#include <stdio.h>

#ifndef INC_BUZZER_H_
#define INC_BUZZER_H_
void safeSound(TIM_HandleTypeDef *htim3, uint8_t state);
void initSound(TIM_HandleTypeDef *htim3);
void setSound(int freq, TIM_HandleTypeDef *htim3);
void stopSound(TIM_HandleTypeDef *htim3);
void manualModeSound(TIM_HandleTypeDef *htim3);
void autoModeSound(TIM_HandleTypeDef *htim3);
void suddenlyStopSound(TIM_HandleTypeDef *htim3);
void hornSound(TIM_HandleTypeDef *htim3);
#endif /* INC_BUZZER_H_ */
