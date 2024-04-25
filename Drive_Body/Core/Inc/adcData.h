/*
 * adcData.h
 *
 *  Created on: Apr 13, 2024
 *      Author: myhg1
 */

#ifndef INC_ADCDATA_H_
#define INC_ADCDATA_H_

#include "main.h"

typedef struct _rxData_t{
	uint16_t XData;
	uint16_t YData;
	uint8_t modeState;
	uint8_t modeChangeFlag;
	uint8_t hornData;
}rxData_t;


extern rxData_t controlData;

void getADCdata(uint8_t* flag,uint8_t *adcval, uint8_t size);
uint8_t adcData_Chksum(uint8_t* data, uint8_t len);
void adcData_Pasing(uint8_t* data);

#endif /* INC_ADCDATA_H_ */
