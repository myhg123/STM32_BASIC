/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim11;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM11_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t font[] = { 0b00111111, //0
		0b00000110, //1
		0b01011011, //2
		0b01001111, //3
		0b01100110, //4
		0b01101101, //5
		0b01111101, //6
		0b00100111, //7
		0b01111111, //8
		0b01101111,  //9
		0b00000000  //off
		};

uint8_t reverseFont[] = { 0b00111111,  //0
		0b00110000,  //1
		0b01011011,  //2
		0b01111001,  //3
		0b01110100,  //4
		0b01101101,  //5
		0b01101111,  //6
		0b00111100,  //7
		0b01111111,  //8
		0b01111101  //9
		};

uint8_t digit[4] = { 0b00000110, //1
		0b01011011, //2
		0b01001111, //3
		0b01100110, //4
		};

void SystickCallback() {
	static uint16_t msec = 0;
	static uint8_t vpos = 0;
	static uint8_t mask = 0;
	msec++;
	if (msec > 20) {
		msec = 0;
		vpos++;
		vpos %= 5;
		if (vpos == 0)
			mask = 0xff - 0b11111110; // a segment on
		if (vpos == 1)
			mask = 0xff - 0b11011101; // b,f seg on
		if (vpos == 2)
			mask = 0xff - 0b10111111; // g seg on
		if (vpos == 3)
			mask = 0xff - 0b11101011; //c, e seg on
		if (vpos == 4)
			mask = 0xff - 0b11110111; // d seg on
	}

	static int pos = 0;
	GPIOC->ODR = 0b1111;
	GPIOB->ODR = digit[pos];// & mask;
	GPIOC->ODR = 0b1111 - (1 << pos);
	pos++;
	pos %= 4;
}

void delayUs(uint16_t time) {
	htim11.Instance->CNT = 0;
	while (htim11.Instance->CNT < time)
		;
}

uint32_t IC_Val1 = 0;
uint32_t IC_Val2 = 0;
uint32_t Difference = 0;
uint8_t IsFirstCaptured = 0;
uint32_t Distance = 0;

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {
		//?��?�� ?��?��?��?��
		if (IsFirstCaptured == 0) {
			//???���??? 값을 ?��?��
			IC_Val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
			IsFirstCaptured = 1;
			//?��?�� ?��?��?��?���??? ?��강해?�� 발생?��?���??? ?��?��
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1,
					TIM_INPUTCHANNELPOLARITY_FALLING);
		}
		//?���??? ?��?��?��?��
		else if (IsFirstCaptured == 1) {
			//???���??? 값을 ?��?��
			IC_Val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
			htim->Instance->CNT = 0;
			//2개의 값의 차이�??? 계산
			if (IC_Val2 > IC_Val1)
				Difference = IC_Val2 - IC_Val1;
			else if (IC_Val1 > IC_Val2)
				Difference = (0xffff - IC_Val1) + IC_Val2;
			//공기중의 ?��?��?��?���??? 고려?�� ?��?�� 계산
			Distance = Difference * 0.34 / 2;
			IsFirstCaptured = 0;
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1,
					TIM_INPUTCHANNELPOLARITY_RISING);
			__HAL_TIM_DISABLE_IT(htim, TIM_IT_CC1);
		}
	}
}

uint32_t getDistance() {
	HAL_GPIO_WritePin(trigger_GPIO_Port, trigger_Pin, 1);
	delayUs(10);
	HAL_GPIO_WritePin(trigger_GPIO_Port, trigger_Pin, 0);
	//?��?��?��?�� ?��?��
	__HAL_TIM_ENABLE_IT(&htim1, TIM_IT_CC1);
	return Distance;
}
typedef struct {
	uint8_t Temperature;
	uint8_t Humidity;
}dht11_t;


int wait_pulse(int state) { //wait edge!
	//htim11.Instance->CNT = 0;
	__HAL_TIM_SET_COUNTER(&htim11, 0);

	while (HAL_GPIO_ReadPin(dht11_GPIO_Port, dht11_Pin) != state) {
		if (__HAL_TIM_GET_COUNTER(&htim11) >= 100)
			return 0;
	}
	return 1;
}

dht11_t dht11_read(){
	dht11_t result;
	// ---------------------send start signal
	// change pin mode
	GPIO_InitTypeDef GPIO_InitStruct = { 0};
	GPIO_InitStruct.Pin = dht11_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(dht11_GPIO_Port, &GPIO_InitStruct);
	//Low 18ms, high 20us
	HAL_GPIO_WritePin(dht11_GPIO_Port, dht11_Pin, 0);
	HAL_Delay(18);
	HAL_GPIO_WritePin(dht11_GPIO_Port, dht11_Pin, 1);
	delayUs(20);
	//change pin mode input
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;	//Setting mode
	GPIO_InitStruct.Pull = GPIO_NOPULL;	//Setting mode
	HAL_GPIO_Init(dht11_GPIO_Port, &GPIO_InitStruct);	//Setting apply
	//------------------------------check answer dht11
	delayUs(40);
	if (!HAL_GPIO_ReadPin(dht11_GPIO_Port, dht11_Pin)) { //dht11 response check(low)
		delayUs(80);
		if (!HAL_GPIO_ReadPin(dht11_GPIO_Port, dht11_Pin)) //timeout
			return;
	}
	if (wait_pulse(GPIO_PIN_RESET) == 0)
		return;
	//---------------------------------read Data
	uint8_t out[5], i, j;
	for (i = 0; i < 5; i++) { // humidity integral, decimal, temperature integral, decimal, check sum
		for (j = 0; j < 8; j++) { // 8bit
			if (!wait_pulse(GPIO_PIN_SET)) // start bit
				return;
			delayUs(40);
			if (!HAL_GPIO_ReadPin(dht11_GPIO_Port, dht11_Pin))
				out[i] &= ~(1 << (7 - j)); //state 0
			else
				out[i] |= (1 << (7 - j)); // state 1
			if (!wait_pulse(GPIO_PIN_RESET))
				return;
		}
	}
	//-----------------------check sum
	if (out[4] != (out[0] + out[1] + out[2] + out[3]))
		return;
	result.Temperature = out[2];
	result.Humidity = out[0];
	return result;

}


/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_TIM1_Init();
	MX_TIM11_Init();
	/* USER CODE BEGIN 2 */

	//	for delay us
	HAL_TIM_Base_Start(&htim11);
	//	for Interrupt capture
	HAL_TIM_IC_Start(&htim1, TIM_CHANNEL_1);
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		//int dist = getDistance();
		dht11_t dht11Value = dht11_read();
		digit[0] = font[dht11Value.Temperature / 10];
		digit[1] = font[dht11Value.Temperature % 10];
		digit[2] = font[dht11Value.Humidity /10];
		digit[3] = font[dht11Value.Humidity % 10];

		HAL_Delay(1000);


//  	if(dist < 500){
//    	digit[0] = font[dist / 1000];
//    	digit[1] = font[dist % 1000 / 100];
//    	digit[2] = font[dist % 1000 % 100 / 10];
//    	digit[3] = font[dist % 1000 % 100 % 10];
//  	}
//  	else{
//  		memset(digit,0,4);
//  	}

//  	HAL_Delay(1);
//  	static int count = 0;
		//2.11K, 12.23K
//  	char str[5];
//  	sprintf(str,"%04d", count);
//  	for(int i =0; i<4; i++){
//  		digit[i] = font[str[i] - 0x30];
//  	}
		//sprintf 보다?�� ?�� 코드�?? 많이 ?��?�� 그이?��?�� sprintf?�� ?��?�� 많이 ?��?��먹기?���??
		//1.71k, 9.75k
//  	static int count;
//  	digit[0] = reverseFont[count / 1000];
//  	digit[1] = reverseFont[count % 1000 / 100];
//  	digit[2] = reverseFont[count % 1000 % 100 / 10];
//  	digit[3] = reverseFont[count % 1000 % 100 % 10];
//  	count++;
//  	count %= 10000;
//  	HAL_Delay(50);

		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief TIM1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM1_Init(void) {

	/* USER CODE BEGIN TIM1_Init 0 */

	/* USER CODE END TIM1_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };
	TIM_IC_InitTypeDef sConfigIC = { 0 };

	/* USER CODE BEGIN TIM1_Init 1 */

	/* USER CODE END TIM1_Init 1 */
	htim1.Instance = TIM1;
	htim1.Init.Prescaler = 16 - 1;
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.Period = 65535;
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.RepetitionCounter = 0;
	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim1) != HAL_OK) {
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_TIM_IC_Init(&htim1) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK) {
		Error_Handler();
	}
	sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
	sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
	sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
	sConfigIC.ICFilter = 0;
	if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM1_Init 2 */

	/* USER CODE END TIM1_Init 2 */

}

/**
 * @brief TIM11 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM11_Init(void) {

	/* USER CODE BEGIN TIM11_Init 0 */

	/* USER CODE END TIM11_Init 0 */

	/* USER CODE BEGIN TIM11_Init 1 */

	/* USER CODE END TIM11_Init 1 */
	htim11.Instance = TIM11;
	htim11.Init.Prescaler = 16 - 1;
	htim11.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim11.Init.Period = 65535;
	htim11.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim11.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim11) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM11_Init 2 */

	/* USER CODE END TIM11_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	/* USER CODE BEGIN MX_GPIO_Init_1 */
	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOC, D1_Pin | D2_Pin | D3_Pin | D4_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB,
			SA_Pin | SB_Pin | SC_Pin | SD_Pin | SE_Pin | SF_Pin | SG_Pin | SDP_Pin,
			GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(trigger_GPIO_Port, trigger_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pins : D1_Pin D2_Pin D3_Pin D4_Pin */
	GPIO_InitStruct.Pin = D1_Pin | D2_Pin | D3_Pin | D4_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pin : dht11_Pin */
	GPIO_InitStruct.Pin = dht11_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(dht11_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : SA_Pin SB_Pin SC_Pin SD_Pin
	 SE_Pin SF_Pin SG_Pin SDP_Pin */
	GPIO_InitStruct.Pin = SA_Pin | SB_Pin | SC_Pin | SD_Pin | SE_Pin | SF_Pin
			| SG_Pin | SDP_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin : trigger_Pin */
	GPIO_InitStruct.Pin = trigger_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(trigger_GPIO_Port, &GPIO_InitStruct);

	/* USER CODE BEGIN MX_GPIO_Init_2 */
	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
