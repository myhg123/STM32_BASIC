/*
 * Motor.c
 *
 *  Created on: Apr 14, 2024
 *      Author: myhg1
 */
#include "Motor.h"

void manualDriveStart(TIM_HandleTypeDef *htim4, int16_t xAxis, int16_t yAxis) {
	xAxis = xAxis - 2118;
	yAxis = yAxis - 2068;
	//ccr1 left, ccr2 right
	//left wheel
	wheelGenerate(htim4, xAxis, yAxis, 0);
}

void safeDriveStart(TIM_HandleTypeDef *htim4, TIM_HandleTypeDef *htim2,
		TIM_HandleTypeDef *htim11, int16_t xAxis, int16_t yAxis) {
	uint16_t C_dist, L_dist, R_dist;
	//left wheel
	C_dist = getDistance_C(htim11, htim2);
	L_dist = getDistance_L(htim11, htim2);
	R_dist = getDistance_R(htim11, htim2);

	xAxis = xAxis - 2118;
	yAxis = yAxis - 2068;
	//ccr1 left, ccr2 right

	if (C_dist < 30) {
		if ((C_dist < 10) & (xAxis>100|xAxis<-100)&(yAxis>100|yAxis<-100)) {
			MoveStop(htim4);
			return 2;
		}
		wheelGenerate(htim4, xAxis, yAxis, 20000);
		return 1;
	}

	wheelGenerate(htim4, xAxis, yAxis, 0);
	return 0;
}

void wheelGenerate(TIM_HandleTypeDef *htim4, int16_t xAxis, int16_t yAxis,
		int16_t deceleration) {
	if (yAxis > 100) {
		rightDriveForward();
		htim4->Instance->CCR2 = map(abs(yAxis), 100, 2060 , 0, 65535- deceleration);
	} else if (yAxis < -100) {
		rightDriveBackward();
		htim4->Instance->CCR2 = map(abs(yAxis), 100, 2100 , 0, 65535- deceleration);
	}

	//Right wheel
	if (xAxis > 100) {
		leftDriveForward();
		htim4->Instance->CCR1 = map(abs(xAxis), 100, 2000 , 0, 53700- deceleration);
	} else if (xAxis < -100) {
		leftDriveBackward();
		htim4->Instance->CCR1 = map(abs(xAxis), 100, 2150 , 0, 53700- deceleration);
	}

}

void MoveStop(TIM_HandleTypeDef *htim) {
	htim->Instance->CCR1 = 0;
	htim->Instance->CCR2 = 0;
}

void goMove(TIM_HandleTypeDef *htim4) {
	rightDriveForward();
	leftDriveForward();
	htim4->Instance->CCR2 = 65535;
	htim4->Instance->CCR1 = 53700;
}

void backMove(TIM_HandleTypeDef *htim4) {
	rightDriveBackward();
	leftDriveBackward();
	htim4->Instance->CCR2 = 45000;
	htim4->Instance->CCR1 = 36900;
}

void rightDriveForward() {
	HAL_GPIO_WritePin(Left_IN3_GPIO_Port, Left_IN3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Left_IN4_GPIO_Port, Left_IN4_Pin, GPIO_PIN_SET);
}

void leftDriveForward() {
	HAL_GPIO_WritePin(Right_IN1_GPIO_Port, Right_IN1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Right_IN2_GPIO_Port, Right_IN2_Pin, GPIO_PIN_RESET);
}

void rightDriveBackward() {
	HAL_GPIO_WritePin(Left_IN3_GPIO_Port, Left_IN3_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Left_IN4_GPIO_Port, Left_IN4_Pin, GPIO_PIN_RESET);
}

void leftDriveBackward() {
	HAL_GPIO_WritePin(Right_IN1_GPIO_Port, Right_IN1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Right_IN2_GPIO_Port, Right_IN2_Pin, GPIO_PIN_SET);
}

