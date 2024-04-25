/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "utils.h"
#include "types.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define manual_LED_Pin GPIO_PIN_13
#define manual_LED_GPIO_Port GPIOC
#define LCD_CS_Pin GPIO_PIN_1
#define LCD_CS_GPIO_Port GPIOC
#define LCD_DC_Pin GPIO_PIN_2
#define LCD_DC_GPIO_Port GPIOC
#define LCD_RST_Pin GPIO_PIN_3
#define LCD_RST_GPIO_Port GPIOC
#define echo_left_Pin GPIO_PIN_0
#define echo_left_GPIO_Port GPIOA
#define echo_center_Pin GPIO_PIN_1
#define echo_center_GPIO_Port GPIOA
#define buzzer_Pin GPIO_PIN_1
#define buzzer_GPIO_Port GPIOB
#define trigger1_Pin GPIO_PIN_2
#define trigger1_GPIO_Port GPIOB
#define echo_right_Pin GPIO_PIN_10
#define echo_right_GPIO_Port GPIOB
#define L_Laser_Pin GPIO_PIN_8
#define L_Laser_GPIO_Port GPIOA
#define C_Laser_Pin GPIO_PIN_11
#define C_Laser_GPIO_Port GPIOA
#define R_Laser_Pin GPIO_PIN_12
#define R_Laser_GPIO_Port GPIOA
#define stop_LED_Pin GPIO_PIN_11
#define stop_LED_GPIO_Port GPIOC
#define auto_LED_Pin GPIO_PIN_12
#define auto_LED_GPIO_Port GPIOC
#define Right_IN1_Pin GPIO_PIN_3
#define Right_IN1_GPIO_Port GPIOB
#define Right_IN2_Pin GPIO_PIN_4
#define Right_IN2_GPIO_Port GPIOB
#define Right_Wheel_Pin GPIO_PIN_6
#define Right_Wheel_GPIO_Port GPIOB
#define Left_Wheel_Pin GPIO_PIN_7
#define Left_Wheel_GPIO_Port GPIOB
#define Left_IN3_Pin GPIO_PIN_8
#define Left_IN3_GPIO_Port GPIOB
#define Left_IN4_Pin GPIO_PIN_9
#define Left_IN4_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
