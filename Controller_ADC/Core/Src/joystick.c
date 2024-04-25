/*
 * joystick.c
 *
 *  Created on: Apr 12, 2024
 *      Author: myhg1
 */
#include "joystick.h"
void dataToBlue(uint8_t data){
	HAL_UART_Transmit_IT(&huart1, &data, 1);
	HAL_Delay(10);
}

void adcData_Filt(uint16_t* buf, uint16_t* Xfilt, uint16_t* Yfilt){

	#define X 0.60f

	*Xfilt = *Xfilt * X + buf[0] * (1.0f - X);
	*Yfilt = *Yfilt * X + buf[1] * (1.0f - X);

//	printf("%d,%d,%d,%d\n",buf[0],buf[1],*Xfilt,*Yfilt);

	adcDataMaxMin(&Xfilt);
	adcDataMaxMin(&Yfilt);
}
void dataTransmit(uint16_t Xdata, uint16_t Ydata, uint8_t mode, uint8_t horn){

		uint8_t tx_data1,tx_data2,tx_data3,tx_data4;
		tx_data1= Xdata>>8;
		tx_data2= Xdata;
		tx_data3= Ydata>>8;
		tx_data4= Ydata;
		switch (mode) {
			case STOP:
				tx_data1 |= 0x00;
				break;
			case MANUAL:
				tx_data1 |= 0x10;
				break;
			case AUTO:
				tx_data1 |= 0x20;
				break;
			case SAFEDRIVE:
				tx_data1 |= 0x30;
				break;
		}
		switch (horn){
		case HORN_OFF:
			tx_data3 |= 0x00;
			break;
		case HORN_ON:
			tx_data3 |= 0x10;
			break;
		}

		uint8_t chksum=0;
		chksum = STX + ETX + tx_data1 + tx_data2 + tx_data3 + tx_data4;

		dataToBlue(STX);
		dataToBlue(ETX);
		dataToBlue(tx_data1);
		dataToBlue(tx_data2);
		dataToBlue(tx_data3);
		dataToBlue(tx_data4);
		dataToBlue(chksum);

//  	printf("%c,%c,%d,%d,%d,%d,%d\n",STX,ETX,tx_data1,tx_data2,tx_data3,tx_data4,chksum);

}

void adcDataMaxMin(uint16_t *data) {
	if (*data > 4096) {
		*data = 4096;
	}
	if (*data < 15) {
		*data = 0;
	}
}


