/*
 * autoMove.c
 *
 *  Created on: Apr 15, 2024
 *      Author: kccistc
 */

#include "autoMove.h"

void initAutoMove(TIM_HandleTypeDef *htim4) {
	MoveStop(htim4);
}

void ultraSonicAutoMove(TIM_HandleTypeDef *htim4, TIM_HandleTypeDef *htim11,
		TIM_HandleTypeDef *htim2, TIM_HandleTypeDef *htim3) {
	uint16_t L_dist, C_dist, R_dist;
	L_dist = getDistance_L(htim11, htim2);
	C_dist = getDistance_C(htim11, htim2);
	R_dist = getDistance_R(htim11, htim2);

	if (C_dist > 100) {
		goMove(htim4);
	}

	if (C_dist <= 40) {
		if (L_dist < R_dist && R_dist > 30) {
			leftDriveForward();
			rightDriveBackward();
			htim4->Instance->CCR1 = 28700;
			htim4->Instance->CCR2 = 35000;
		} else if (L_dist >= R_dist && L_dist > 30) {
			rightDriveForward();
			leftDriveBackward();
			htim4->Instance->CCR1 = 28700;
			htim4->Instance->CCR2 = 35000;
		}
	}
	if (C_dist <= 10) {
		backMove(htim4);
		hornSound(htim3);
	}else stopSound(htim3);



//	if (getDistance_C(htim11, htim2) <= 15) {
//		while (getDistance_C(htim11, htim2) < 50) {
//			leftDriveBackward();
//			rightDriveBackward();
//			htim4->Instance->CCR1 = 32800;
//			htim4->Instance->CCR2 = 40000;
//		}
//		//left object
//		while (getDistance_C(htim11, htim2) < 130) {
//			if (getDistance_L(htim11, htim2) < getDistance_R(htim11, htim2)) {
//				leftDriveForward();
//				rightDriveBackward();
//			} else {
//				leftDriveBackward();
//				rightDriveForward();
//			}
//			htim4->Instance->CCR1 = 16400;
//			htim4->Instance->CCR2 = 20000;
//		}
//	}
//	else
//	goMove(htim4);
//
//}

//	if (C_dist <= 130 && C_dist > 30) {
//		//left object
//		if (L_dist < R_dist && R_dist > 50) {
//
//		}
//		//right object
//		else if (L_dist >= R_dist && L_dist > 50) {
//			leftDriveForward();
//			htim4->Instance->CCR1 = 45000;
//		}
//	}
//}

}
