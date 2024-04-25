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
#include	"uart.h"
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
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
typedef struct {
	uint8_t year;
	uint8_t month;
	uint8_t date;
	uint8_t day;
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
} DateTime_t;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t BCD2Decimal(uint8_t inData) {
	uint8_t upper = inData >> 4;
	uint8_t lower = inData & 0x0f;
	return upper * 10 + lower;
}

uint8_t Decimal2BCD(uint8_t inData) {
	uint8_t upper = inData / 10;
	uint8_t lower = inData % 10;
	return upper << 4 | lower;
}

#define RTC_ADD 0XD0
#define ROM_ADD 0XA0
#define MagicNumber	0x321fad16 //반드시 8개여야한다.
#define READ 1

//EEPROM map table
#define		eeMagicNumberBase		0
#define		eeMagicNumberSize		4



void setRTC(DateTime_t inData) {
	uint8_t txBuffer[7];
	txBuffer[7] = Decimal2BCD(inData.year);
	txBuffer[6] = Decimal2BCD(inData.month);
	txBuffer[5] = Decimal2BCD(inData.date);
	txBuffer[3] = Decimal2BCD(inData.hour);
	txBuffer[2] = Decimal2BCD(inData.min);
	txBuffer[1] = Decimal2BCD(inData.sec);
	txBuffer[0] = 0;
	HAL_I2C_Master_Transmit(&hi2c1, RTC_ADD, txBuffer, sizeof(txBuffer), 10);
}

DateTime_t getRTC() {
	DateTime_t result;
	uint8_t rxBuffer[7];
	uint8_t address = 0;
	HAL_I2C_Master_Transmit(&hi2c1, RTC_ADD, &address, 1, 10);
	HAL_I2C_Master_Receive(&hi2c1, RTC_ADD | READ, rxBuffer, 7, 10);
	result.year = BCD2Decimal(rxBuffer[6]);
	result.month = BCD2Decimal(rxBuffer[5]);
	result.date = BCD2Decimal(rxBuffer[4]);
	result.day = BCD2Decimal(rxBuffer[3]);
	result.hour = BCD2Decimal(rxBuffer[2]);
	result.min = BCD2Decimal(rxBuffer[1]);
	result.sec = BCD2Decimal(rxBuffer[0]);
	return result;
}

//address = 0 ~ 0x37
void writeRAM(uint8_t address, uint8_t data){
	HAL_I2C_Mem_Write(&hi2c1, RTC_ADD, address, 1, &data, 1, 10);
}

uint8_t readRAM(uint8_t address){
	uint8_t result;
	HAL_I2C_Mem_Read(&hi2c1, RTC_ADD, address, 1, &result, 1, 10);
	return result;
}

void writeEEPROM(uint16_t address, uint8_t data){
	HAL_I2C_Mem_Write(&hi2c1, ROM_ADD, address, 2, &data, 1, 10);
	HAL_Delay(5);
}
uint8_t readEEPROM(uint16_t address){
	uint8_t result;
	HAL_I2C_Mem_Write(&hi2c1, ROM_ADD, address, 2, &result, 1, 10);
	return result;
}

void write2ByteEEPROM(uint16_t address, uint16_t data){
	HAL_I2C_Mem_Write(&hi2c1, ROM_ADD, address, 2, &data, 2, 10);
	HAL_Delay(10);

}

uint16_t read2ByteEEPROM(uint16_t address){
	uint16_t result;
	HAL_I2C_Mem_Read(&hi2c1, ROM_ADD, address, 2, &result, 2, 10);
	return result;

}

void write4ByteEEPROM(uint16_t address, uint32_t data){
	HAL_I2C_Mem_Write(&hi2c1, ROM_ADD, address, 2, &data, 4, 10);
	HAL_Delay(20);
}

uint32_t read4ByteEEPROM(uint16_t address){
	uint32_t result;
	HAL_I2C_Mem_Read(&hi2c1, ROM_ADD, address, 2, &result, 4, 10);
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
	MX_I2C1_Init();
	MX_USART2_UART_Init();
	/* USER CODE BEGIN 2 */
	DateTime_t dateTime;
	initUart(&huart2);
	if(MagicNumber != read4ByteEEPROM(eeMagicNumberBase)){
		//initial setting
		dateTime.year = 24;
		dateTime.month = 3;
		dateTime.date = 29;
		dateTime.hour = 14;
		dateTime.min = 32;
		dateTime.sec = 0;
		setRTC(dateTime);
		// number recording
		write4ByteEEPROM(eeMagicNumberBase, MagicNumber);
	}
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	/*printf("i2c scan start\n");
	 for (int address = 0; address < 256; address++){
	 if(HAL_I2C_IsDeviceReady(&hi2c1, address, 0, 10) == HAL_OK){
	 printf("%02x is ready\n", address);
	 }
	 }
	 printf("i2c scan end\n");*/

//	uint8_t address = 0;
//	uint8_t buffer[0x40] = { 0, };
//	HAL_I2C_Master_Transmit(&hi2c1, 0xd0, &address, 1, 10);
//	HAL_I2C_Master_Receive(&hi2c1, 0xd1, buffer, 1, 10);
//	buffer[1] = buffer[0] & 0x7f; //MSB 1bit를
//	buffer[0] = 0;
//	HAL_I2C_Master_Transmit(&hi2c1, 0xd0, buffer, 2, 10);




	while (1) {
			dateTime = getRTC();
			printf("20%02d-%02d-%02d %02d:%02d:%02d\n",
					dateTime.year,
					dateTime.month,
					dateTime.date,
					dateTime.hour,
					dateTime.min,
					dateTime.sec);



//		buffer[0] = 0;
//		HAL_I2C_Master_Transmit(&hi2c1, 0xd0, buffer, 1, 10); // 주소?��?�� 2바이?�� ?��?��
//		HAL_I2C_Master_Receive(&hi2c1, 0xd1, buffer, 0x40, 100);
//		printf("\n20%02d-%02d-%02d %02d:%02d:%02d\n", BCD2Decimal(buffer[6]),
//				BCD2Decimal(buffer[5]), BCD2Decimal(buffer[4]), BCD2Decimal(buffer[2]),
//				BCD2Decimal(buffer[1]), BCD2Decimal(buffer[0]));
		HAL_Delay(1000);

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
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void) {

	/* USER CODE BEGIN I2C1_Init 0 */

	/* USER CODE END I2C1_Init 0 */

	/* USER CODE BEGIN I2C1_Init 1 */

	/* USER CODE END I2C1_Init 1 */
	hi2c1.Instance = I2C1;
	hi2c1.Init.ClockSpeed = 100000;
	hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN I2C1_Init 2 */

	/* USER CODE END I2C1_Init 2 */

}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void) {

	/* USER CODE BEGIN USART2_Init 0 */

	/* USER CODE END USART2_Init 0 */

	/* USER CODE BEGIN USART2_Init 1 */

	/* USER CODE END USART2_Init 1 */
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart2) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART2_Init 2 */

	/* USER CODE END USART2_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	/* USER CODE BEGIN MX_GPIO_Init_1 */
	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

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
