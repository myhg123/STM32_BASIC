/*
 * st7789_Data_Views.c
 *
 *  Created on: Mar 11, 2024
 *      Author: Andrianov Vitaly
 */

#include "st7789/st7789_Views/st7789_Data_Views.h"

/**
  * @brief  Draw start screen info (init ble)
  * @retval void
 */
void st7789_DrawStartScreen() {
	st7789_PrintString(20, 70, BLUE_st7789, WHITE_st7789, 1, &font_11x18, 2, "Инициализация");
	st7789_PrintString(125, 120, BLUE_st7789, WHITE_st7789, 1, &font_11x18, 2, "BLE");
}

/**
  * @brief  Draw error message (if ble init fault)
  * @retval void
 */
void st7789_DrawErrScreen() {
	st7789_FillRect(0, 0,  320, 240, WHITE_st7789);
	st7789_PrintString(50, 90, RED_st7789, WHITE_st7789, 1, &font_11x18, 2, "Ошибка BLE");
}

void st7789_DrawBleConnScreen() {
	st7789_FillRect(0, 0,  320, 240, WHITE_st7789);
	st7789_PrintString(20, 70, BLUE_st7789, WHITE_st7789, 1, &font_11x18, 2, "Подключение к");
	st7789_PrintString(105, 120, BLUE_st7789, WHITE_st7789, 1, &font_11x18, 2, "Slave");
}

/**
  * @brief  Draw data info screen
  * @retval void
 */
void st7789_DrawDataScreen() {
	st7789_FillRect(0, 0,  320, 240, WHITE_st7789);
	st7789_PrintString(20, 10, CYAN_st7789, WHITE_st7789, 1, &font_11x18, 1, "Master mode");
	st7789_PrintString(220, 10, BLACK_st7789, GREEN_st7789, 1, &font_11x18, 1, "Сопряжен");

	st7789_PrintString(20, 35, BLACK_st7789, WHITE_st7789, 1, &font_11x18, 1, "Текущая темп.C:");
	st7789_PrintString(20, 55, BLACK_st7789, WHITE_st7789, 1, &font_11x18, 1, "Минимальная темп.C:");
	st7789_PrintString(20, 75, BLACK_st7789, WHITE_st7789, 1, &font_11x18, 1, "Максимальная темп.C:");

	st7789_PrintString(20, 100, BLACK_st7789, WHITE_st7789, 1, &font_11x18, 1, "Текущий RSSI,dbm:");
	st7789_PrintString(20, 120, BLACK_st7789, WHITE_st7789, 1, &font_11x18, 1, "Минимальный RSSI,dbm:");
	st7789_PrintString(20, 140, BLACK_st7789, WHITE_st7789, 1, &font_11x18, 1, "Максимальная RSSI,dbm:");

	st7789_PrintString(20, 165, BLACK_st7789, WHITE_st7789, 1, &font_11x18, 1, "RX/TX/Loss:");
	st7789_PrintString(20, 185, BLACK_st7789, WHITE_st7789, 1, &font_11x18, 1, "Последнее сообщение:");
}
