/*
 * delay.c
 *
 *  Created on: Feb 13, 2024
 *      Author: Andrianov Vitaly
 */

#include "utils/delay.h"

/**
  * @brief  Setting the delay (in microseconds) using tim2
  * @note   Check tim2 sec, for 32MHz mode: Prescaler = 31, Count Period = 65535
  * @param  Delay us (microseconds)
  * @retval Void
  */
void usDelay(uint32_t us) {
	__HAL_TIM_SET_COUNTER(&htim2, 0);
	__HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_UPDATE);
	while(__HAL_TIM_GET_COUNTER(&htim2) < us) {
		if (__HAL_TIM_GET_FLAG(&htim2, TIM_FLAG_UPDATE) != RESET) {
			break;
		}
	}
}


