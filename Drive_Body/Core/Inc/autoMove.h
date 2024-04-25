/*
 * autoMove.h
 *
 *  Created on: Apr 15, 2024
 *      Author: kccistc
 */

#ifndef INC_AUTOMOVE_H_
#define INC_AUTOMOVE_H_

#include "main.h"
#include <stdlib.h>
#include "ultraSonic.h"
#include "Motor.h"

void initAutoMove(TIM_HandleTypeDef* htim4);
void ultraSonicAutoMove(TIM_HandleTypeDef* htim4, TIM_HandleTypeDef* htim11, TIM_HandleTypeDef* htim2, TIM_HandleTypeDef *htim3);

#endif /* INC_AUTOMOVE_H_ */
