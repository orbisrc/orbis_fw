/*
 * stdispdriver.c
 *
 */

#include "gui/stdispdriver.h"
#include "gui/dispdriver/S6D0154X.h"
#include "gui/dispdriver/ILI9341.h"
#include "spi.h"
#include "tim.h"

#define LCD_BOARD				2					/* 1 -  ILI9341
 	 	 	 	 	 	 	 	 	 	 	 	 	   2 - 	S6D0154X
 	 	 	 	 	 	 	 	 	 	 	 	 	*/

void STLCDinit(void)
{

	STLCDsetBrightnessInit();

#if LCD_BOARD == 1

	ILI9341_Init();							// Display init

#elif 	LCD_BOARD == 2

	S6D0154X_Init();

#endif

	STFillScreen(0x42EE);

}

void STDrawPixel(uint16_t X, uint16_t Y, uint16_t Color) {
	/*
	 * ��������� ������� ���������� ����� � �������� �����������
	 */

#if LCD_BOARD == 1

	ILI9341_DrawPixel(X, Y, Color);

#elif 	LCD_BOARD == 2

	 S6D0154X_DrawPixel(X, Y, Color);

#endif
}
void STDrawHorizontalLine(uint16_t X, uint16_t Y, uint16_t Length,
		uint16_t Color) {
	/*
	 * ������������ ����� ������ Length �� �������� �����������, ���������� �����.
	 * �������� ������ ������ ���������. �������� ����� �������.
	 */
#if LCD_BOARD == 1

	ILI9341_DrawHorizontalLine(X, Y, Length, Color);

#elif 	LCD_BOARD == 2

	S6D0154X_DrawHorizontalLine(X, Y, Length, Color);

#endif

}

void STDrawVerticalLine(uint16_t X, uint16_t Y, uint16_t Length, uint16_t Color) {
	/*
	 * ������������ ����� ������ Length �� �������� �����������, ���������� �����.
	 * �������� ������ ������ ���������. �������� ������ ����.
	 */
#if LCD_BOARD == 1

	ILI9341_DrawVerticalLine(X, Y, Length, Color);

#elif 	LCD_BOARD == 2

	S6D0154X_DrawVerticalLine(X, Y, Length, Color);

#endif
}

void STDrawRectangle(uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height, uint16_t Color)
{
	STDrawHorizontalLine(X, Y, Width, Color);
	STDrawHorizontalLine(X, Y + Height - 1, Width, Color);
	STDrawVerticalLine(X, Y, Height, Color);
	STDrawVerticalLine(X + Width - 1, Y, Height, Color);
}
void STDrawFilledRectangle(uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height, uint16_t FillColor)
{

#if LCD_BOARD == 1

	ILI9341_DrawRectangle(X, Y, Width, Height, FillColor);

#elif 	LCD_BOARD == 2

	S6D0154X_DrawRectangle(X, Y, Width, Height, FillColor);

#endif
}

void STPutChar(const char Char, const FONT_INFO *Font, uint16_t X, uint16_t Y, uint16_t Color)
{
	uint8_t i, j, k;
	const FONT_CHAR_INFO *Char_Info;
	uint16_t Char_offset;

	Char_Info = Font->FontTable;
	Char_offset = (uint16_t) Char - 32;
	Char_Info = Char_Info + Char_offset;

	for (j = 0; j < Char_Info->width; j++) {
		for (i = 0; i < 8; i++) {
			for (k = 0; k < Font->Height; k++) {
				if (*(Font->FontBitmaps + Char_Info->start + j
						+ Char_Info->width * k) & (1 << i)) {
					STDrawPixel(X + j, Y + i + 8 * k, Color);
				}
			}

		}
	}

}

void STPutText(const char* String, const FONT_INFO *Font, uint16_t X,
		uint16_t Y, uint16_t Color, uint16_t BackgroundColor) {
	const FONT_CHAR_INFO *Char_Info;
	uint16_t Char_offset;

	while (*String >= Font->FirstChar && *String <= Font->LastChar) {
		Char_Info = Font->FontTable;
		if (*String <= ' ') {
			Char_offset = 0;
		} else {
			Char_offset = (uint16_t) *String - 32;
		}
		Char_Info = Char_Info + Char_offset;
		STPutChar(*String, Font, X, Y, Color);
		X += Char_Info->width + 2;
		String++;
	}
}

void STFillScreen(uint16_t Color)
{
#if LCD_BOARD == 1

	ILI9341_FillScreen(Color);

#elif 	LCD_BOARD == 2

	S6D0154X_FillScreen(Color);

#endif

}

void STFrame(uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height,uint16_t Thickness, uint16_t Color)
{
	uint8_t i;

	for (i = 0; i < Thickness; i++) {
		STDrawRectangle(X + i, Y + i, Width - 2 * i, Height - 2 * i, Color);
	}
}

void STRectangle(uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height, uint16_t BorderThickness, uint16_t BorderColor, uint16_t Color)
{
	if (BorderThickness != 0)
	{
		STFrame(X, Y, Width, Height, BorderThickness, BorderColor);
		STDrawFilledRectangle(X + BorderThickness, Y + BorderThickness,	Width - 2 * BorderThickness, Height - 2 * BorderThickness,	Color);

	}
	else
	{
		STDrawFilledRectangle(X, Y, Width, Height, Color);
	}

}

/*
 *
 * http://www.digole.com/tools/PicturetoC_Hex_converter.php
 *
 */


void STDrawPicture(uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height, const unsigned char *Picture)
{
#if LCD_BOARD == 1

	ILI9341_SetArea(X, Y, Width - 1, Height - 1);
	ILI9341_uDC();
	ILI9341_CS();
	HAL_SPI_Transmit(&ILI9341_SPI, (unsigned char*) Picture, 2*Width * Height, 1000);
	ILI9341_uCS();

#elif 	LCD_BOARD == 2

	unsigned char TxBuffer[2] = {0};

	S6D0154X_SetArea(X, Y, Width - 1, Height - 1);

	TxBuffer[0] = 0x72;
	TxBuffer[1] = 0;

	S6D0154X_CS();

	HAL_SPI_Transmit(&S6D0154X_SPI, TxBuffer, 2, 100);

	uint32_t i=0;

	for (i=0; i < Width * Height ;i++)
	{
		TxBuffer[1] = *(Picture + i*2);
		TxBuffer[0] = *(Picture + i*2 + 1);

		HAL_SPI_Transmit(&S6D0154X_SPI, TxBuffer, 2, 100);

	}

	S6D0154X_uCS();

#endif
}

void STLCDsetBrightness(uint16_t Value)
{
	TIM3->CCR4=Value;
}

uint16_t STLCDgetBrightness(void)
{
	return TIM3->CCR4;
}


void STLCDsetBrightnessInit(void)
{
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);

	STLCDsetBrightness(0);
}
