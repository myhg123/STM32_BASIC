#ifndef ILI9341_GFX_H
#define ILI9341_GFX_H

#include "stm32f4xx_hal.h"
#include "fonts.h"
#include "stdbool.h"
#include "main.h"

#define HORIZONTAL_IMAGE	0
#define VERTICAL_IMAGE		1
	enum{AutoMove,Manual,Warning};
void speedLCD_ON(int16_t xAxis, int16_t yAxis,uint8_t state);
void LCD_INIT();
void LeftArrow();
void LeftArrowde();
void RightArrow();
void RightArrowde();
void ILI9341_fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,int16_t x2, int16_t y2, uint16_t color);
void ILI9341_DrawTriangle(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t color);
void ILI9341_DrawHollowCircle(uint16_t X, uint16_t Y, uint16_t radius, uint16_t color);
void ILI9341_DrawFilledCircle(uint16_t X, uint16_t Y, uint16_t radius, uint16_t color);
void ILI9341_DrawHollowRectangleCoord(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t color);
void ILI9341_DrawFilledRectangleCoord(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t color);
void ILI9341_DrawChar(char ch, const uint8_t font[], uint16_t X, uint16_t Y, uint16_t color, uint16_t bgcolor);
void ILI9341_DrawText(const char* str, const uint8_t font[], uint16_t X, uint16_t Y, uint16_t color, uint16_t bgcolor);
void ILI9341_DrawImage(const uint8_t* image, uint8_t orientation);
void ILI9341_DrawLine(int16_t x0,int16_t y0,int16_t x1,int16_t y1,uint16_t color);
#endif
