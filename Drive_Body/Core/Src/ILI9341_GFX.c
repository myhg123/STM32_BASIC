#include "ILI9341_STM32_Driver.h"
#include "ILI9341_GFX.h"

/* imprecise small delay */
__STATIC_INLINE void DelayUs(volatile uint32_t us)
{
	us *= (SystemCoreClock / 1000000);
	while (us--);
}

#ifndef _swap_int16_t
#define _swap_int16_t(a, b)                                                    \
  {                                                                            \
    int16_t t = a;                                                             \
    a = b;                                                                     \
    b = t;                                                                     \
  }
#endif
void LCD_INIT(){
	 ILI9341_Init();
	  ILI9341_SetRotation(SCREEN_HORIZONTAL_2);
	  ILI9341_FillScreen(WHITE);  // fill screen WhitE
	  ILI9341_DrawHollowCircle(160,	240, 100,BLACK);

}
void CLCD_INIT(){
	 ILI9341_Init();
	  ILI9341_SetRotation(SCREEN_VERTICAL_1);
	  ILI9341_FillScreen(WHITE);  // fill screen WhitE


}
void LCD_controller(int16_t xAxis, int16_t yAxis,uint8_t state){
					int16_t tAxis = 0;
					CtextOn(xAxis,yAxis,state);
			  	if(xAxis>2040)xAxis=2040;
			  	if(yAxis>2040)yAxis=2040;
					tAxis = sqrt((xAxis*xAxis)+(yAxis*yAxis));

}
void car(uint16_t CarBody){

	char CarBodystr[40];
itoa(CarBody,CarBodystr);
	ILI9341_DrawText(CarBodystr, FONT4, 100, 120, GREEN,WHITE);
}
void CtextOn(int16_t xAxis,int16_t yAxis, uint8_t state) {
	int32_t tAxis = 0;
	char tAxisStr[40];
	if(abs(xAxis)>2040)xAxis=2040;
			  	if(abs(yAxis)>2040)yAxis=2040;
	tAxis = sqrt((xAxis*xAxis)+(yAxis*yAxis));

  itoa(tAxis, tAxisStr, 10);
	ILI9341_DrawText("Motorcar controller", FONT4, 0, 0, BLUE, WHITE);
	ILI9341_DrawText("PWMVALUE:", FONT4, 0, 40, BLUE, WHITE);

	ILI9341_DrawText(tAxisStr, FONT4, 110, 40, BLUE, WHITE);
	HAL_Delay(1);
	if (state == AutoMove) {
		ILI9341_DrawText("mode:autoMove", FONT4, 0, 20, BLUE, WHITE);
	} else if (state == Manual) {
		ILI9341_DrawText("mode:manual", FONT4, 0, 20, BLUE, WHITE);
	} else if (state == Warning) {
		ILI9341_DrawText("mode:warning", FONT4, 0, 20, BLUE, WHITE);
		ILI9341_FillScreen(RED);
		ILI9341_FillScreen(WHITE);

	}
}
void textOn(uint8_t state, int16_t yAxis){
	if(state == 0x01){
	ILI9341_DrawText("Mode:AutoMove", FONT4, 0	, 15, BLUE, WHITE);
	}
	else if (state == 0x02){
		ILI9341_DrawText("Mode:Manual", FONT4, 0, 15, BLUE, WHITE);
	}
	else if (state == 0x00){
		ILI9341_DrawText("mode:Stop", FONT4, 0, 15, BLUE, WHITE);
	}

}
void speedLCD_ON(int16_t xAxis, int16_t yAxis,uint8_t state ){
				int16_t pwmvalue;
		  	int16_t tAxis = 0;
		  	textOn(state, yAxis);
		  	if(xAxis>2040)xAxis=2040;
		  	if(yAxis>2040)yAxis=2040;
				tAxis = sqrt((xAxis*xAxis)+(yAxis*yAxis));
		  	if(xAxis>100){
		  		RightArrow();
		  		LeftArrowde();
		    	  	}
		    	  	else if(xAxis<-100){
		    	  		LeftArrow();
		    	  		RightArrowde();
		    	    	}
		    	  	else
		    	  	{
		    	  		LeftArrowde();
		    	  		RightArrowde();
		    	  	}

		  		pwmvalue = map(abs(tAxis),0,2048*sqrt(2),0,160);
		  		ILI9341_DrawLine(160, 240, 160-(95*cos(pwmvalue*3.14/180)),240-95*sin(pwmvalue*3.14/180), RED);
		  		HAL_Delay(15);
		  		ILI9341_DrawLine(160, 240, 160-(95*cos(pwmvalue*3.14/180)),240-95*sin(pwmvalue*3.14/180), WHITE);


}
void ILI9341_DrawHollowCircle(uint16_t X, uint16_t Y, uint16_t radius, uint16_t color)
{
	int x = radius-1;
	int y = 0;
	int dx = 1;
	int dy = 1;
	int err = dx - (radius << 1);

	while (x >= y)
	{
		ILI9341_DrawPixel(X + x, Y + y, color);
		ILI9341_DrawPixel(X + y, Y + x, color);
		ILI9341_DrawPixel(X - y, Y + x, color);
		ILI9341_DrawPixel(X - x, Y + y, color);
		ILI9341_DrawPixel(X - x, Y - y, color);
		ILI9341_DrawPixel(X - y, Y - x, color);
		ILI9341_DrawPixel(X + y, Y - x, color);
		ILI9341_DrawPixel(X + x, Y - y, color);

		if (err <= 0)
		{
			y++;
			err += dy;
			dy += 2;
		}

		if (err > 0)
		{
			x--;
			dx += 2;
			err += (-radius << 1) + dx;
		}
	}
}



void ILI9341_DrawFilledCircle(uint16_t X, uint16_t Y, uint16_t radius, uint16_t color)
{

	int x = radius;
	int y = 0;
	int xChange = 1 - (radius << 1);
	int yChange = 0;
	int radiusError = 0;

	while (x >= y)
	{
		for (int i = X - x; i <= X + x; i++)
		{
			ILI9341_DrawPixel(i, Y + y,color);
			ILI9341_DrawPixel(i, Y - y,color);
		}

		for (int i = X - y; i <= X + y; i++)
		{
			ILI9341_DrawPixel(i, Y + x,color);
			ILI9341_DrawPixel(i, Y - x,color);
		}

		y++;
		radiusError += yChange;
		yChange += 2;

		if (((radiusError << 1) + xChange) > 0)
		{
			x--;
			radiusError += xChange;
			xChange += 2;
		}
	}
}

void ILI9341_DrawHollowRectangleCoord(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t color)
{
	uint16_t xLen = 0;
	uint16_t yLen = 0;
	uint8_t negX = 0;
	uint8_t negY = 0;
	float negCalc = 0;

	negCalc = X1 - X0;
	if(negCalc < 0) negX = 1;
	negCalc = 0;

	negCalc = Y1 - Y0;
	if(negCalc < 0) negY = 1;

	//DRAW HORIZONTAL!
	if(!negX)
	{
		xLen = X1 - X0;
	}
	else
	{
		xLen = X0 - X1;
	}
	ILI9341_DrawHLine(X0, Y0, xLen, color);
	ILI9341_DrawHLine(X0, Y1, xLen, color);

	//DRAW VERTICAL!
	if(!negY)
	{
		yLen = Y1 - Y0;
	}
	else
	{
		yLen = Y0 - Y1;
	}

	ILI9341_DrawVLine(X0, Y0, yLen, color);
	ILI9341_DrawVLine(X1, Y0, yLen, color);

	if((xLen > 0)||(yLen > 0))
	{
		ILI9341_DrawPixel(X1, Y1, color);
	}
}

void LeftArrow(){
	//LEFT

	  	ILI9341_DrawFilledRectangleCoord(50, 100, 100, 120, BLACK);
	    ILI9341_fillTriangle(0, 110, 50, 90, 50, 130,BLACK);



}
void LeftArrowde(){
	//LEFT

	ILI9341_DrawFilledRectangleCoord(50, 100, 100, 120, WHITE);
  ILI9341_fillTriangle(0, 110, 50, 90, 50, 130,WHITE);

}
void RightArrow(){
  //RIGHT

  	ILI9341_DrawFilledRectangleCoord(220, 100, 270, 120, BLACK);
  	ILI9341_fillTriangle(320, 110, 270, 90, 270, 130,BLACK);

}

void RightArrowde(){
  //RIGHT

	ILI9341_DrawFilledRectangleCoord(220, 100, 270, 120, WHITE);
	  	ILI9341_fillTriangle(320, 110, 270, 90, 270, 130,WHITE);
}
void Leftblink(){
	LeftArrow();
	LeftArrowde();


}
void Rightblink(){
	RightArrow();
	RightArrowde();


}

void ILI9341_fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,int16_t x2, int16_t y2, uint16_t color) {

  int16_t a, b, y, last;


  if (y0 > y1) {
    _swap_int16_t(y0, y1);
    _swap_int16_t(x0, x1);
  }
  if (y1 > y2) {
    _swap_int16_t(y2, y1);
    _swap_int16_t(x2, x1);
  }
  if (y0 > y1) {
    _swap_int16_t(y0, y1);
    _swap_int16_t(x0, x1);
  }

  if (y0 == y2) {
    a = b = x0;
    if (x1 < a)
      a = x1;
    else if (x1 > b)
      b = x1;
    if (x2 < a)
      a = x2;
    else if (x2 > b)
      b = x2;
    ILI9341_DrawHLine(a, y0, b - a + 1, color);
    return;
  }

  int16_t dx01 = x1 - x0, dy01 = y1 - y0, dx02 = x2 - x0, dy02 = y2 - y0,
          dx12 = x2 - x1, dy12 = y2 - y1;
  int32_t sa = 0, sb = 0;


  if (y1 == y2)
    last = y1; //
  else
    last = y1 - 1; //

  for (y = y0; y <= last; y++) {
    a = x0 + sa / dy01;
    b = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;

    if (a > b)
      _swap_int16_t(a, b);
    ILI9341_DrawHLine(a, y, b - a + 1, color);
  }

  sa = (int32_t)dx12 * (y - y1);
  sb = (int32_t)dx02 * (y - y0);
  for (; y <= y2; y++) {
    a = x1 + sa / dy12;
    b = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;
    if (a > b)
      _swap_int16_t(a, b);
    ILI9341_DrawHLine(a, y, b - a + 1, color);
  }
}



void ILI9341_DrawTriangle(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t color) {

    ILI9341_DrawLine(X0, Y0, X1, Y1, color);
    ILI9341_DrawLine(X1, Y1, X2, Y2, color);
    ILI9341_DrawLine(X2, Y2, X0, Y0, color);

}



void ILI9341_DrawFilledRectangleCoord(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t color)
{
	uint16_t xLen = 0;
	uint16_t yLen = 0;
	uint8_t negX = 0;
	uint8_t negY = 0;
	int32_t negCalc = 0;
	uint16_t X0True = 0;
	uint16_t Y0True = 0;

	negCalc = X1 - X0;
	if(negCalc < 0) negX = 1;
	negCalc = 0;

	negCalc = Y1 - Y0;
	if(negCalc < 0) negY = 1;

	if(!negX)
	{
		xLen = X1 - X0;
		X0True = X0;
	}
	else
	{
		xLen = X0 - X1;
		X0True = X1;
	}

	if(!negY)
	{
		yLen = Y1 - Y0;
		Y0True = Y0;
	}
	else
	{
		yLen = Y0 - Y1;
		Y0True = Y1;
	}

	ILI9341_DrawRectangle(X0True, Y0True, xLen, yLen, color);
}


void ILI9341_DrawChar(char ch, const uint8_t font[], uint16_t X, uint16_t Y, uint16_t color, uint16_t bgcolor)
{
	if ((ch < 31) || (ch > 127)) return;

	uint8_t fOffset, fWidth, fHeight, fBPL;
	uint8_t *tempChar;

	fOffset = font[0];
	fWidth = font[1];
	fHeight = font[2];
	fBPL = font[3];

	tempChar = (uint8_t*)&font[((ch - 0x20) * fOffset) + 4]; /* Current Character = Meta + (Character Index * Offset) */

	/* Clear background first */
	ILI9341_DrawRectangle(X, Y, fWidth, fHeight, bgcolor);

	for (int j=0; j < fHeight; j++)
	{
		for (int i=0; i < fWidth; i++)
		{
			uint8_t z =  tempChar[fBPL * i + ((j & 0xF8) >> 3) + 1]; /* (j & 0xF8) >> 3, increase one by 8-bits */
			uint8_t b = 1 << (j & 0x07);
			if (( z & b ) != 0x00)
			{
				ILI9341_DrawPixel(X+i, Y+j, color);
			}
		}
	}
}

#define _swap_int16_t(a, b)                                                    \
  {                                                                            \
    int16_t t = a;                                                             \
    a = b;                                                                     \
    b = t;                                                                     \
  }


void ILI9341_DrawLine(int16_t x0,int16_t y0,int16_t x1,int16_t y1,uint16_t color){

	#if defined(ESP8266)
	  yield();
	#endif
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






}

void ILI9341_DrawText(const char* str, const uint8_t font[], uint16_t X, uint16_t Y, uint16_t color, uint16_t bgcolor)
{
	uint8_t charWidth;			/* Width of character */
	uint8_t fOffset = font[0];	/* Offset of character */
	uint8_t fWidth = font[1];	/* Width of font */

	while (*str)
	{
		ILI9341_DrawChar(*str, font, X, Y, color, bgcolor);

		/* Check character width and calculate proper position */
		uint8_t *tempChar = (uint8_t*)&font[((*str - 0x20) * fOffset) + 4];
		charWidth = tempChar[0];

		if(charWidth + 2 < fWidth)
		{
			/* If character width is smaller than font width */
			X += (charWidth + 2);
		}
		else
		{
			X += fWidth;
		}

		str++;
	}
}

void ILI9341_DrawImage(const uint8_t* image, uint8_t orientation)
{
	if(orientation == SCREEN_HORIZONTAL_1)
	{
		ILI9341_SetRotation(SCREEN_HORIZONTAL_1);
		ILI9341_SetAddress(0,0,ILI9341_SCREEN_WIDTH,ILI9341_SCREEN_HEIGHT);
	}
	else if(orientation == SCREEN_HORIZONTAL_2)
	{
		ILI9341_SetRotation(SCREEN_HORIZONTAL_2);
		ILI9341_SetAddress(0,0,ILI9341_SCREEN_WIDTH,ILI9341_SCREEN_HEIGHT);
	}
	else if(orientation == SCREEN_VERTICAL_2)
	{
		ILI9341_SetRotation(SCREEN_VERTICAL_2);
		ILI9341_SetAddress(0,0,ILI9341_SCREEN_HEIGHT,ILI9341_SCREEN_WIDTH);
	}
	else if(orientation == SCREEN_VERTICAL_1)
	{
		ILI9341_SetRotation(SCREEN_VERTICAL_1);
		ILI9341_SetAddress(0,0,ILI9341_SCREEN_HEIGHT,ILI9341_SCREEN_WIDTH);
	}

	uint32_t counter = 0;
	for(uint32_t i = 0; i < ILI9341_SCREEN_WIDTH*ILI9341_SCREEN_HEIGHT*2/BURST_MAX_SIZE; i++)
	{
		ILI9341_WriteBuffer((uint8_t*)(image + counter), BURST_MAX_SIZE);
		counter += BURST_MAX_SIZE;

		/* DMA Tx is too fast, It needs some delay */
		DelayUs(1);
	}
}
