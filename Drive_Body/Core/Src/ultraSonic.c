/*
 * ultraSonic.c
 *
 *  Created on: Apr 14, 2024
 *      Author: kccistc
 */

#include "ultraSonic.h"
#include <stdio.h>

uint32_t IC_Val1 = 0;
uint32_t IC_Val2 = 0;
uint32_t Difference1 = 0;
uint32_t Distance1 = 0;
uint8_t IsFirstCaptured1 = 0;

uint32_t IC_Val3 = 0;
uint32_t IC_Val4 = 0;
uint32_t Difference2 = 0;
uint32_t Distance2 = 0;
uint8_t IsFirstCaptured2 = 0;

uint32_t IC_Val5 = 0;
uint32_t IC_Val6 = 0;
uint32_t Difference3 = 0;
uint32_t Distance3 = 0;
uint8_t IsFirstCaptured3 = 0;

uint8_t IsAroundCaptured = 0;

void ultraSonicInit(TIM_HandleTypeDef *htim1 , TIM_HandleTypeDef* htim2){
	HAL_TIM_IC_Start_IT(htim1, TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(htim1, TIM_CHANNEL_2);
	HAL_TIM_IC_Start_IT(htim1, TIM_CHANNEL_3);

	HAL_TIM_Base_Start(htim2);
}

void delayUs(uint16_t time, TIM_HandleTypeDef *htim){
	htim->Instance->CNT = 0;
	while(htim->Instance->CNT < time);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1){
		//rising interrupt mode
		if(IsFirstCaptured1 == 0 && IsAroundCaptured == 0){
			//Read Timer
			IC_Val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // time to rising interrupt mode
				IsFirstCaptured1 = 1;
			//Next interrupt falling setting
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
		}
		//falling interrupt mode
		else if(IsFirstCaptured1 == 1){
			//read Timer
			IC_Val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // time to falling interrupt mode
			htim->Instance->CNT = 0;
			// subtract to IC_Val1 and IC_Val2
			if(IC_Val2 > IC_Val1) Difference1 = IC_Val2 - IC_Val1;
			else if(IC_Val1 > IC_Val2) Difference1 = (0xffff - IC_Val1) + IC_Val2; // one arround --> max - IC_Val1
			//Calculation to Distance
			Distance1 = Difference1 * 0.034 / 2;
			if(Distance1 > 150) Distance1 = 150;
			IsFirstCaptured1 = 0;
			IsAroundCaptured = 1;
			//Next interrupt rising setting
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
			//Disable to interrupt
			__HAL_TIM_DISABLE_IT(htim, TIM_IT_CC1);
		}
	}
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2){
				//rising interrupt mode
				if(IsFirstCaptured2 == 0 && IsAroundCaptured == 1){
					//Read Timer
					IC_Val3 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2); // time to rising interrupt mode
						IsFirstCaptured2 = 1;
					//Next interrupt falling setting
					__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_FALLING);
				}
				//falling interrupt mode
				else if(IsFirstCaptured2 == 1){
					//read Timer
					IC_Val4 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2); // time to falling interrupt mode
					htim->Instance->CNT = 0;
					// subtract to IC_Val1 and IC_Val2
					if(IC_Val4 > IC_Val3) Difference2 = IC_Val4 - IC_Val3;
					else if(IC_Val3 > IC_Val4) Difference2 = (0xffff - IC_Val3) + IC_Val4; // one arround --> max - IC_Val1
					//Calculation to Distance
					Distance2 = Difference2 * 0.034 / 2;
					if(Distance2 > 150) Distance2 = 150;
					IsFirstCaptured2 = 0;
					IsAroundCaptured = 2;
					//Next interrupt rising setting
					__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_RISING);
					//Disable to interrupt
					__HAL_TIM_DISABLE_IT(htim, TIM_IT_CC2);
				}
		}
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3){
				//rising interrupt mode
				if(IsFirstCaptured3 == 0 && IsAroundCaptured == 2){
					//Read Timer
					IC_Val5 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3); // time to rising interrupt mode
						IsFirstCaptured3 = 1;
					//Next interrupt falling setting
					__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_FALLING);
				}
				//falling interrupt mode
				else if(IsFirstCaptured3 == 1){
					//read Timer
					IC_Val6 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3); // time to falling interrupt mode
					htim->Instance->CNT = 0;
					// subtract to IC_Val1 and IC_Val2
					if(IC_Val6 > IC_Val5) Difference3 = IC_Val6 - IC_Val5;
					else if(IC_Val5 > IC_Val6) Difference3 = (0xffff - IC_Val5) + IC_Val6; // one arround --> max - IC_Val1
					//Calculation to Distance
					Distance3 = Difference3 * 0.034 / 2;
					if(Distance3 > 150) Distance3 = 150;
					IsFirstCaptured3 = 0;
					IsAroundCaptured = 0;
					//Next interrupt rising setting
					__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_RISING);
					//Disable to interrupt
					__HAL_TIM_DISABLE_IT(htim, TIM_IT_CC3);
				}
			}
}

uint32_t getDistance_L(TIM_HandleTypeDef *htim1, TIM_HandleTypeDef *htim2){
	HAL_GPIO_WritePin(trigger1_GPIO_Port, trigger1_Pin, 1);
//	HAL_GPIO_WritePin(GPIOC, left_light_Pin, 1);
	delayUs(10 , htim1);
	HAL_GPIO_WritePin(trigger1_GPIO_Port, trigger1_Pin, 0);
//	HAL_GPIO_WritePin(GPIOC, left_light_Pin, 0);
	//Enable to interrupt
	__HAL_TIM_ENABLE_IT(htim2, TIM_IT_CC1);
	return Distance1;
}

uint32_t getDistance_C(TIM_HandleTypeDef *htim1, TIM_HandleTypeDef *htim2){
	HAL_GPIO_WritePin(trigger1_GPIO_Port, trigger1_Pin, 1);
	delayUs(10, htim1);
	HAL_GPIO_WritePin(trigger1_GPIO_Port, trigger1_Pin, 0);
	//Enable to interrupt
	__HAL_TIM_ENABLE_IT(htim2, TIM_IT_CC2);
	return Distance2;
}

uint32_t getDistance_R(TIM_HandleTypeDef *htim1, TIM_HandleTypeDef *htim2){
	HAL_GPIO_WritePin(trigger1_GPIO_Port, trigger1_Pin, 1);
//	HAL_GPIO_WritePin(GPIOC, right_light_Pin, 1);
//	HAL_GPIO_WritePin(GPIOC, right_light_Pin, 1);
	delayUs(10, htim1);
	HAL_GPIO_WritePin(trigger1_GPIO_Port, trigger1_Pin, 0);
//	HAL_GPIO_WritePin(GPIOC, right_light_Pin, 0);
	//Enable to interrupt
	__HAL_TIM_ENABLE_IT(htim2, TIM_IT_CC3);
	return Distance3;
}

uint16_t movingAvgFilter(uint16_t inData){
   static uint16_t filterBuffer[maxValue];
   static uint32_t sumValue = 0;
   static uint16_t bufPos = 0;
   static _Bool isFirst = 0;
   if(isFirst == 0){
      isFirst = 1;
      for(int i = 0; i<maxValue; i++){
         filterBuffer[i] = inData;
      }
   }
   //Algorithm method
   // sumValue - bufPos's value
   sumValue -= filterBuffer[bufPos];
   // bufPos's value calculation
   filterBuffer[bufPos] = inData;
   // sumValue + bufPos's value
   sumValue += filterBuffer[bufPos];
   //bufPos ++
   bufPos++;
   // reset bufPos
   bufPos %= maxValue;
   return sumValue / maxValue;
}

uint16_t IIR_Filter(uint16_t data, uint16_t filtData){
	filtData = filtData * IIR_X + data * (1.0f - IIR_X);
	return filtData;
}

double Kalman(double inData) {
// Kalman filter setup
static double P = 1.0;
static double varP = 0.0001; // pow(0.01, 2)
static double R = 0.25;//pow(0.5, 2);
static double K = 1.0;
static double X = 20.0;
// Kalman Simple Filter
P = P + varP;
K = P / (P + R);
X = (K * inData) + (1 - K) * X;
P = (1 - K) * P;
return X;
}


