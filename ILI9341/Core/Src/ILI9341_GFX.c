#include "ILI9341_STM32_Driver.h"
#include "ILI9341_GFX.h"

/* imprecise small delay */
__STATIC_INLINE void DelayUs(volatile uint32_t us) {
	us *= (SystemCoreClock / 1000000);
	while (us--)
		;
}

void ILI9341_DrawHollowCircle(uint16_t X, uint16_t Y, uint16_t radius,
		uint16_t color) {
	int x = radius - 1;
	int y = 0;
	int dx = 1;
	int dy = 1;
	int err = dx - (radius << 1);

	while (x >= y) {
		ILI9341_DrawPixel(X + x, Y + y, color);
		ILI9341_DrawPixel(X + y, Y + x, color);
		ILI9341_DrawPixel(X - y, Y + x, color);
		ILI9341_DrawPixel(X - x, Y + y, color);
		ILI9341_DrawPixel(X - x, Y - y, color);
		ILI9341_DrawPixel(X - y, Y - x, color);
		ILI9341_DrawPixel(X + y, Y - x, color);
		ILI9341_DrawPixel(X + x, Y - y, color);

		if (err <= 0) {
			y++;
			err += dy;
			dy += 2;
		}

		if (err > 0) {
			x--;
			dx += 2;
			err += (-radius << 1) + dx;
		}
	}
}

void ILI9341_DrawFilledCircle(uint16_t X, uint16_t Y, uint16_t radius,
		uint16_t color) {

	int x = radius;
	int y = 0;
	int xChange = 1 - (radius << 1);
	int yChange = 0;
	int radiusError = 0;

	while (x >= y) {
		for (int i = X - x; i <= X + x; i++) {
			ILI9341_DrawPixel(i, Y + y, color);
			ILI9341_DrawPixel(i, Y - y, color);
		}

		for (int i = X - y; i <= X + y; i++) {
			ILI9341_DrawPixel(i, Y + x, color);
			ILI9341_DrawPixel(i, Y - x, color);
		}

		y++;
		radiusError += yChange;
		yChange += 2;

		if (((radiusError << 1) + xChange) > 0) {
			x--;
			radiusError += xChange;
			xChange += 2;
		}
	}
}

void ILI9341_DrawHollowRectangleCoord(uint16_t X0, uint16_t Y0, uint16_t X1,
		uint16_t Y1, uint16_t color) {
	uint16_t xLen = 0;
	uint16_t yLen = 0;
	uint8_t negX = 0;
	uint8_t negY = 0;
	float negCalc = 0;

	negCalc = X1 - X0;
	if (negCalc < 0)
		negX = 1;
	negCalc = 0;

	negCalc = Y1 - Y0;
	if (negCalc < 0)
		negY = 1;

	//DRAW HORIZONTAL!
	if (!negX) {
		xLen = X1 - X0;
	} else {
		xLen = X0 - X1;
	}
	ILI9341_DrawHLine(X0, Y0, xLen, color);
	ILI9341_DrawHLine(X0, Y1, xLen, color);

	//DRAW VERTICAL!
	if (!negY) {
		yLen = Y1 - Y0;
	} else {
		yLen = Y0 - Y1;
	}

	ILI9341_DrawVLine(X0, Y0, yLen, color);
	ILI9341_DrawVLine(X1, Y0, yLen, color);

	if ((xLen > 0) || (yLen > 0)) {
		ILI9341_DrawPixel(X1, Y1, color);
	}
}

void ILI9341_DrawFilledRectangleCoord(uint16_t X0, uint16_t Y0, uint16_t X1,
		uint16_t Y1, uint16_t color) {
	uint16_t xLen = 0;
	uint16_t yLen = 0;
	uint8_t negX = 0;
	uint8_t negY = 0;
	int32_t negCalc = 0;
	uint16_t X0True = 0;
	uint16_t Y0True = 0;

	negCalc = X1 - X0;
	if (negCalc < 0)
		negX = 1;
	negCalc = 0;

	negCalc = Y1 - Y0;
	if (negCalc < 0)
		negY = 1;

	if (!negX) {
		xLen = X1 - X0;
		X0True = X0;
	} else {
		xLen = X0 - X1;
		X0True = X1;
	}

	if (!negY) {
		yLen = Y1 - Y0;
		Y0True = Y0;
	} else {
		yLen = Y0 - Y1;
		Y0True = Y1;
	}

	ILI9341_DrawRectangle(X0True, Y0True, xLen, yLen, color);
}

void ILI9341_DrawChar(char ch, const uint8_t font[], uint16_t X, uint16_t Y,
		uint16_t color, uint16_t bgcolor) {
	if ((ch < 31) || (ch > 127))
		return;

	uint8_t fOffset, fWidth, fHeight, fBPL;
	uint8_t *tempChar;

	fOffset = font[0];
	fWidth = font[1];
	fHeight = font[2];
	fBPL = font[3];

	tempChar = (uint8_t*) &font[((ch - 0x20) * fOffset) + 4]; /* Current Character = Meta + (Character Index * Offset) */

	/* Clear background first */
	ILI9341_DrawRectangle(X, Y, fWidth, fHeight, bgcolor);

	for (int j = 0; j < fHeight; j++) {
		for (int i = 0; i < fWidth; i++) {
			uint8_t z = tempChar[fBPL * i + ((j & 0xF8) >> 3) + 1]; /* (j & 0xF8) >> 3, increase one by 8-bits */
			uint8_t b = 1 << (j & 0x07);
			if ((z & b) != 0x00) {
				ILI9341_DrawPixel(X + i, Y + j, color);
			}
		}
	}
}

void ILI9341_DrawText(const char *str, const uint8_t font[], uint16_t X,
		uint16_t Y, uint16_t color, uint16_t bgcolor) {
	uint8_t charWidth; /* Width of character */
	uint8_t fOffset = font[0]; /* Offset of character */
	uint8_t fWidth = font[1]; /* Width of font */

	while (*str) {
		ILI9341_DrawChar(*str, font, X, Y, color, bgcolor);

		/* Check character width and calculate proper position */
		uint8_t *tempChar = (uint8_t*) &font[((*str - 0x20) * fOffset) + 4];
		charWidth = tempChar[0];

		if (charWidth + 2 < fWidth) {
			/* If character width is smaller than font width */
			X += (charWidth + 2);
		} else {
			X += fWidth;
		}

		str++;
	}
}
#define _swap_int16_t(a, b)                                                    \
  {                                                                            \
    int16_t t = a;                                                             \
    a = b;                                                                     \
    b = t;                                                                     \
  }

//곡선 표현
void ILI9341_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
		uint16_t color) {
	int16_t steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep) {
		_swap_int16_t(x0, y0);
		_swap_int16_t(x1, y1);
	}

	if (x0 > x1) {
		_swap_int16_t(x0, x1);
		_swap_int16_t(y0, y1);
	}

	int16_t dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);

	int16_t err = dx / 2;
	int16_t ystep;

	if (y0 < y1) {
		ystep = 1;
	} else {
		ystep = -1;
	}

	for (; x0 <= x1; x0++) {
		if (steep) {
			ILI9341_DrawPixel(y0, x0, color);
		} else {
			ILI9341_DrawPixel(x0, y0, color);
		}
		err -= dy;
		if (err < 0) {
			y0 += ystep;
			err += dx;
		}
	}

	//x를 기준으로 시작 위치 찾기 왼쪽(작은 값)을 시작위치로 통일 시켜줘야한다.
//	if (x0 > x1) {
//		int16_t xtmp = x0;
//		int16_t ytmp = y0;
//		x0 = x1;
//		y0 = y1;
//		x1 = xtmp;
//		y1 = ytmp;
//	}
//	//기울기 계산
//	int16_t deltaX = x1 - x0;
//	int16_t deltaY = abs(y0 - y1); //abs는 절대값
//
//	//경사가 1이상이냐 이하냐에 따라 갈린다. 기울기가 작으면 x가 1증가할때의 y를 구하는게 낫고 반대의 경우는 반대로 하면된다.
//	//그리기
//	if (deltaX > deltaY) {
//		//45도 이하
//		float slope = (float)deltaY / (float)deltaX;
//		//그리기
//		for (int i = 0; i <= deltaX; i++) {
//			ILI9341_DrawPixel(x0 + i, y0 + (i * slope), color);
//		}
//	}
//
//	else {
//		//45도 이상
//		float slope = (float)deltaX / (float)deltaY;
//		// 그리기
//		if (y0 < y1) {
//			for (int i = 0; i <= deltaY; i++) {
//				ILI9341_DrawPixel(x0+(i*slope), y0 + i, color);
//			}
//		}
//		else {
//			for (int i = 0; i <= deltaY; i++) {
//				ILI9341_DrawPixel(x0+(i*slope), y0 - i, color);
//			}
//		}
//	}
}

void ILI9341_DrawImage(const uint8_t *image, uint8_t orientation) {
	if (orientation == SCREEN_HORIZONTAL_1) {
		ILI9341_SetRotation(SCREEN_HORIZONTAL_1);
		ILI9341_SetAddress(0, 0, ILI9341_SCREEN_WIDTH, ILI9341_SCREEN_HEIGHT);
	} else if (orientation == SCREEN_HORIZONTAL_2) {
		ILI9341_SetRotation(SCREEN_HORIZONTAL_2);
		ILI9341_SetAddress(0, 0, ILI9341_SCREEN_WIDTH, ILI9341_SCREEN_HEIGHT);
	} else if (orientation == SCREEN_VERTICAL_2) {
		ILI9341_SetRotation(SCREEN_VERTICAL_2);
		ILI9341_SetAddress(0, 0, ILI9341_SCREEN_HEIGHT, ILI9341_SCREEN_WIDTH);
	} else if (orientation == SCREEN_VERTICAL_1) {
		ILI9341_SetRotation(SCREEN_VERTICAL_1);
		ILI9341_SetAddress(0, 0, ILI9341_SCREEN_HEIGHT, ILI9341_SCREEN_WIDTH);
	}

	uint32_t counter = 0;
	for (uint32_t i = 0;
			i < ILI9341_SCREEN_WIDTH * ILI9341_SCREEN_HEIGHT * 2 / BURST_MAX_SIZE;
			i++) {
		ILI9341_WriteBuffer((uint8_t*) (image + counter), BURST_MAX_SIZE);
		counter += BURST_MAX_SIZE;

		/* DMA Tx is too fast, It needs some delay */
		DelayUs(1);
	}
}
