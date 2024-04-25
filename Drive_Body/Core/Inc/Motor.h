/*
 * Motor.h
 *
 *  Created on: Apr 14, 2024
 *      Author: myhg1
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_
#include "main.h"
#include <stdlib.h>

void manualDriveStart(TIM_HandleTypeDef* htim4, int16_t xAxis, int16_t yAxis);
void MoveStop(TIM_HandleTypeDef* htim);
void goMove(TIM_HandleTypeDef* htim4);
void backMove(TIM_HandleTypeDef* htim4);
void safeDriveStart(TIM_HandleTypeDef *htim4,TIM_HandleTypeDef *htim2,TIM_HandleTypeDef *htim11, int16_t xAxis, int16_t yAxis);
void wheelGenerate(TIM_HandleTypeDef *htim4,int16_t xAxis, int16_t yAxis, int16_t deceleration);

void rightDriveForward();
void leftDriveForward();
void rightDriveBackward();
void leftDriveBackward();
long map(long x, long in_min, long in_max, long out_min, long out_max);


#endif /* INC_MOTOR_H_ */
