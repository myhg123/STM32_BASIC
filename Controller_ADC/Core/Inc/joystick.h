/*
 * joystick.h
 *
 *  Created on: Apr 12, 2024
 *      Author: myhg1
 */

#ifndef INC_JOYSTICK_H_
#define INC_JOYSTICK_H_

#include "main.h"
#include "usart.h"

#define STX 0x48

#define ETX 0x59



void adcData_Filt(uint16_t* buf, uint16_t* Xfilt, uint16_t* Yfilt);

void dataToBlue(uint8_t data);
void dataTransmit(uint16_t Xdata, uint16_t Ydata, uint8_t mode,uint8_t horn);
void adcDataMaxMin(uint16_t *data);
//long map(long x, long in_min, long in_max, long out_min, long out_max);

#endif /* INC_JOYSTICK_H_ */
