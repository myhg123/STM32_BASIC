/*
 * ultraSonic.h
 *
 *  Created on: Apr 14, 2024
 *      Author: kccistc
 */

#ifndef INC_ULTRASONIC_H_
#define INC_ULTRASONIC_H_

#include "main.h"
#define maxValue 1000
#define IIR_X 0.96f

uint16_t IIR_Filter(uint16_t data, uint16_t filtData);
double Kalman(double inData);
void ultraSonicInit(TIM_HandleTypeDef *htim1 , TIM_HandleTypeDef* htim2);
void delayUs(uint16_t time, TIM_HandleTypeDef *htim);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
uint32_t getDistance_L(TIM_HandleTypeDef *htim1, TIM_HandleTypeDef *htim2);
uint32_t getDistance_C(TIM_HandleTypeDef *htim1, TIM_HandleTypeDef *htim2);
uint32_t getDistance_R(TIM_HandleTypeDef *htim1, TIM_HandleTypeDef *htim2);
uint16_t movingAvgFilter(uint16_t inData);

#endif /* INC_ULTRASONIC_H_ */
