/*
 * adcData.c
 *
 *  Created on: Apr 13, 2024
 *      Author: myhg1
 */
#include "adcData.h"

rxData_t controlData;

//void getADCdata(uint8_t *adcval, uint8_t size) {
//	if (adcData_Chksum(&adcval[0], size)) {
//		adcData_Pasing(&adcval[0]);
//	}
//}

void getADCdata(uint8_t *flag, uint8_t *adcval, uint8_t size) {
	if (*flag == 1) {
		*flag=0;
		if (adcData_Chksum(&adcval[0], size)) {
			adcData_Pasing(&adcval[0]);
		}
	}
}

uint8_t adcData_Chksum(uint8_t *data, uint8_t len) {
	uint8_t chksum = 0;
	for (int i = 0; i < len - 1; i++) {
		chksum += data[i];
	}

	return (chksum == data[6]);
}

void adcData_Pasing(uint8_t *data) {
	uint8_t oldState = 0;
	oldState = controlData.modeState;
	controlData.XData = ((data[2] << 8) | data[3]) & 0x0fff;
	controlData.YData = ((data[4] << 8) | data[5]) & 0x0fff;
	controlData.hornData = (data[4]>>4) & 0x0f;
	controlData.modeState = (data[2]>>4) & 0x0f;
	if(oldState != controlData.modeState) controlData.modeChangeFlag=1;
//reverse stick
	controlData.YData = 4096 - controlData.YData;
//remove Offset
}
