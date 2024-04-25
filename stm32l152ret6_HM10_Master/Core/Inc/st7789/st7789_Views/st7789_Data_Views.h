/*
 * st7789_Data_Views.h
 *
 *  Created on: Mar 11, 2024
 *      Author: user
 */

#ifndef INC_ST7789_ST7789_VIEWS_ST7789_DATA_VIEWS_H_
#define INC_ST7789_ST7789_VIEWS_ST7789_DATA_VIEWS_H_

#include "st7789/st7789.h"

//--------------------------------------------------------------------------------//
	void st7789_DrawStartScreen();		// draw start screen info (init ble)
	void st7789_DrawErrScreen();		// draw error message (if ble init fault)
	void st7789_DrawDataScreen();		// draw data info screen
//--------------------------------------------------------------------------------//

#endif /* INC_ST7789_ST7789_VIEWS_ST7789_DATA_VIEWS_H_ */
