/*
 * ILI9341.c
 *
 *	Copyright 2019 Denis Davydov
 *
 *	Licensed under the Apache License, Version 2.0 (the "License");
 *	you may not use this file except in compliance with the License.
 *	You may obtain a copy of the License at
 *
 *		http://www.apache.org/licenses/LICENSE-2.0
 *
 *	Unless required by applicable law or agreed to in writing, software
 *	distributed under the License is distributed on an "AS IS" BASIS,
 *	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *	See the License for the specific language governing permissions and
 *	limitations under the License.
 */


#include "gui/dispdriver/ILI9341.h"
#include "stm32f4xx_hal.h"
#include "periphery/spi.h"
#include "periphery/gpio.h"

#define LCD_WIDTH					240
#define LCD_HEIGHT					320


void ILI9341_Init()
{


	ILI9341_CS();
	ILI9341_Delay(120);

//SOFTWARE RESET
	ILI9341_WrireCommand(0x01);
	ILI9341_Delay(1000);

//POWER CONTROL A
	ILI9341_WrireCommand(0xCB);
	ILI9341_WriteData(0x39);
	ILI9341_WriteData(0x2C);
	ILI9341_WriteData(0x00);
	ILI9341_WriteData(0x34);
	ILI9341_WriteData(0x02);

//POWER CONTROL B
	ILI9341_WrireCommand(0xCF);
	ILI9341_WriteData(0x00);
	ILI9341_WriteData(0xC1);
	ILI9341_WriteData(0x30);

//DRIVER TIMING CONTROL A
	ILI9341_WrireCommand(0xE8);
	ILI9341_WriteData(0x85);
	ILI9341_WriteData(0x00);
	ILI9341_WriteData(0x78);

//DRIVER TIMING CONTROL B
	ILI9341_WrireCommand(0xEA);
	ILI9341_WriteData(0x00);
	ILI9341_WriteData(0x00);

//POWER ON SEQUENCE CONTROL
	ILI9341_WrireCommand(0xED);
	ILI9341_WriteData(0x64);
	ILI9341_WriteData(0x03);
	ILI9341_WriteData(0x12);
	ILI9341_WriteData(0x81);

//PUMP RATIO CONTROL
	ILI9341_WrireCommand(0xF7);
	ILI9341_WriteData(0x20);

//POWER CONTROL,VRH[5:0]
	ILI9341_WrireCommand(0xC0);
	ILI9341_WriteData(0x23);

//POWER CONTROL,SAP[2:0];BT[3:0]
	ILI9341_WrireCommand(0xC1);
	ILI9341_WriteData(0x10);

//VCM CONTROL
	ILI9341_WrireCommand(0xC5);
	ILI9341_WriteData(0x3E);
	ILI9341_WriteData(0x28);

//VCM CONTROL 2
	ILI9341_WrireCommand(0xC7);
	ILI9341_WriteData(0x86);

//MEMORY ACCESS CONTROL
	ILI9341_WrireCommand(0x36);
	ILI9341_WriteData(0x48);

//PIXEL FORMAT
	ILI9341_WrireCommand(0x3A);
	ILI9341_WriteData(0x55);

//FRAME RATIO CONTROL, STANDARD RGB COLOR
	ILI9341_WrireCommand(0xB1);
	ILI9341_WriteData(0x00);
	ILI9341_WriteData(0x18);

//DISPLAY FUNCTION CONTROL
	ILI9341_WrireCommand(0xB6);
	ILI9341_WriteData(0x08);
	ILI9341_WriteData(0x82);
	ILI9341_WriteData(0x27);

//3GAMMA FUNCTION DISABLE
	ILI9341_WrireCommand(0xF2);
	ILI9341_WriteData(0x00);

//GAMMA CURVE SELECTED
	ILI9341_WrireCommand(0x26);
	ILI9341_WriteData(0x01);

//POSITIVE GAMMA CORRECTION
	ILI9341_WrireCommand(0xE0);
	ILI9341_WriteData(0x0F);
	ILI9341_WriteData(0x31);
	ILI9341_WriteData(0x2B);
	ILI9341_WriteData(0x0C);
	ILI9341_WriteData(0x0E);
	ILI9341_WriteData(0x08);
	ILI9341_WriteData(0x4E);
	ILI9341_WriteData(0xF1);
	ILI9341_WriteData(0x37);
	ILI9341_WriteData(0x07);
	ILI9341_WriteData(0x10);
	ILI9341_WriteData(0x03);
	ILI9341_WriteData(0x0E);
	ILI9341_WriteData(0x09);
	ILI9341_WriteData(0x00);

//NEGATIVE GAMMA CORRECTION
	ILI9341_WrireCommand(0xE1);
	ILI9341_WriteData(0x00);
	ILI9341_WriteData(0x0E);
	ILI9341_WriteData(0x14);
	ILI9341_WriteData(0x03);
	ILI9341_WriteData(0x11);
	ILI9341_WriteData(0x07);
	ILI9341_WriteData(0x31);
	ILI9341_WriteData(0xC1);
	ILI9341_WriteData(0x48);
	ILI9341_WriteData(0x08);
	ILI9341_WriteData(0x0F);
	ILI9341_WriteData(0x0C);
	ILI9341_WriteData(0x31);
	ILI9341_WriteData(0x36);
	ILI9341_WriteData(0x0F);

//EXIT SLEEP
	ILI9341_WrireCommand(0x11);
	ILI9341_Delay(120);

//TURN ON DISPLAY
	ILI9341_WrireCommand(0x29);

//STARTING ROTATION
	ILI9341_SetRotation(SCREEN_VERTICAL_2);
}

void ILI9341_WR_REG(uint16_t Index, uint16_t Config)
{

}

void ILI9341_WrireCommand(uint8_t Cmd)
{
	ILI9341_CS();
	ILI9341_DC();
	ILI9341_SPI_Send(Cmd);
	ILI9341_uDC();
	ILI9341_uCS();
}

void ILI9341_WriteData(uint8_t Data)
{
	ILI9341_CS();
	ILI9341_uDC();
	ILI9341_SPI_Send(Data);
	ILI9341_uCS();
}

void ILI9341_SPI_Send(unsigned char Data)
{
	HAL_SPI_Transmit(&ILI9341_SPI, &Data, 1, 1);
}

void ILI9341_SetArea (uint16_t X, uint16_t Y, uint16_t Length, uint16_t Height)
{
	ILI9341_WrireCommand(0x2A);
	ILI9341_WriteData(X >> 8);
	ILI9341_WriteData(X);
	ILI9341_WriteData((X + Length)  >> 8);
	ILI9341_WriteData((X + Length));

	ILI9341_WrireCommand(0x2B);
	ILI9341_WriteData(Y >> 8);
	ILI9341_WriteData(Y);
	ILI9341_WriteData((Y + Height) >> 8);
	ILI9341_WriteData((Y + Height));

	ILI9341_WrireCommand(0x2C);
}

void ILI9341_DrawPixel (uint16_t X, uint16_t Y, uint16_t Color)
{
	//ADDRESS
		ILI9341_CS();
		ILI9341_DC();
		ILI9341_SPI_Send(0x2A);
		ILI9341_uDC();
		ILI9341_uCS();

	//XDATA
		ILI9341_CS();
		unsigned char Temp_Buffer[4] = { X >> 8, X, (X + 1) >> 8, (X + 1) };
		HAL_SPI_Transmit(&ILI9341_SPI, Temp_Buffer, 4, 1);
		ILI9341_uCS();

	//ADDRESS
		ILI9341_CS();
		ILI9341_DC();
		ILI9341_SPI_Send(0x2B);
		ILI9341_uDC();
		ILI9341_uCS();

	//YDATA
		ILI9341_CS();
		unsigned char Temp_Buffer1[4] = { Y >> 8, Y, (Y + 1) >> 8, (Y + 1) };
		HAL_SPI_Transmit(&ILI9341_SPI, Temp_Buffer1, 4, 1);
		ILI9341_uCS();

	//ADDRESS
		ILI9341_CS();
		ILI9341_DC();
		ILI9341_SPI_Send(0x2C);
		ILI9341_uDC();
		ILI9341_uCS();

	//COLOUR
		ILI9341_CS();
		unsigned char Temp_Buffer2[2] = { Color >> 8, Color };
		HAL_SPI_Transmit(&ILI9341_SPI, Temp_Buffer2, 2, 1);
		ILI9341_uCS();
}

void ILI9341_FillScreen (uint16_t Color)
{
	ILI9341_SetArea(0, 0, LCD_WIDTH, LCD_HEIGHT);

	ILI9341_ColorStream(Color, LCD_WIDTH * LCD_HEIGHT);
}

void ILI9341_ColorStream (uint16_t Color, uint32_t Size)
{
	uint32_t Buffer_Size = 0;
	if ((Size * 2) < ILI9341_BUFF_SIZE) {
		Buffer_Size = Size;
	} else {
		Buffer_Size = ILI9341_BUFF_SIZE;
	}

	ILI9341_uDC();
	ILI9341_CS();

//	HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET);

	unsigned char chifted = Color >> 8;
	;
	unsigned char burst_buffer[ILI9341_BUFF_SIZE]; //unsigned char burst_buffer[Buffer_Size];
	for (uint32_t j = 0; j < Buffer_Size; j += 2) {
		burst_buffer[j] = chifted;
		burst_buffer[j + 1] = Color;
	}

	uint32_t Sending_Size = Size * 2;
	uint32_t Sending_in_Block = Sending_Size / Buffer_Size;
	uint32_t Remainder_from_block = Sending_Size % Buffer_Size;

	if (Sending_in_Block != 0) {
		for (uint32_t j = 0; j < (Sending_in_Block); j++) {
			HAL_SPI_Transmit(&ILI9341_SPI, (unsigned char *) burst_buffer,	Buffer_Size, 10);
			//HAL_SPI_Transmit_DMA(HSPI_INSTANCE, (unsigned char *)burst_buffer, Buffer_Size);
		}
	}

//REMAINDER!
	HAL_SPI_Transmit(&ILI9341_SPI, (unsigned char *) burst_buffer,	Remainder_from_block, 10);

	ILI9341_uCS();

//	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET);
}

void ILI9341_DrawRectangle (uint16_t X, uint16_t Y, uint16_t Length, uint16_t Height, uint16_t Color)
{
	ILI9341_SetArea(X, Y, Length - 1, Height - 1);

	ILI9341_ColorStream(Color, Length*Height);
}

void ILI9341_DrawHorizontalLine (uint16_t X, uint16_t Y, uint16_t Length, uint16_t Color)
{
	ILI9341_SetArea(X, Y, Length - 1, 0);

	ILI9341_ColorStream(Color, Length);
}

void ILI9341_DrawVerticalLine (uint16_t X, uint16_t Y, uint16_t Height, uint16_t Color)
{
	ILI9341_SetArea(X, Y, 0, Height - 1);

	ILI9341_ColorStream(Color, Height);
}

void ILI9341_SetRotation(uint8_t Rotation)
{

	uint8_t screen_rotation = Rotation;

	ILI9341_WrireCommand(0x36);
	ILI9341_Delay(1);

	switch (screen_rotation) {
	case SCREEN_VERTICAL_1:
		ILI9341_WriteData(0x40 | 0x08);
		break;
	case SCREEN_HORIZONTAL_1:
		ILI9341_WriteData(0x20 | 0x08);
		break;
	case SCREEN_VERTICAL_2:
		ILI9341_WriteData(0x80 | 0x08);
		break;
	case SCREEN_HORIZONTAL_2:
		ILI9341_WriteData(0x40 | 0x80 | 0x20 | 0x08);
		break;
	default:
		//EXIT IF SCREEN ROTATION NOT VALID!
		break;
	}
}

void ILI9341_CS()
{
	HAL_GPIO_WritePin(ILI9341_CS_PORT, ILI9341_CS_PIN, GPIO_PIN_RESET);
}

void ILI9341_uCS()
{
	HAL_GPIO_WritePin(ILI9341_CS_PORT, ILI9341_CS_PIN, GPIO_PIN_SET);
}

void ILI9341_DC()
{
	HAL_GPIO_WritePin(ILI9341_DC_PORT, ILI9341_DC_PIN, GPIO_PIN_RESET);
}

void ILI9341_uDC()
{
	HAL_GPIO_WritePin(ILI9341_DC_PORT, ILI9341_DC_PIN, GPIO_PIN_SET);
}

void ILI9341_Delay(uint32_t ms)
{
	HAL_Delay(ms);
}

void ILI9341_Draw_Area_DMA(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t *buff)
{
	ILI9341_WrireCommand(0x2A);
	ILI9341_WriteData(X1 >> 8);
	ILI9341_WriteData(X1);
	ILI9341_WriteData((X2) >> 8);
	ILI9341_WriteData((X2));

	ILI9341_WrireCommand(0x2B);
	ILI9341_WriteData(Y1 >> 8);
	ILI9341_WriteData(Y1);
	ILI9341_WriteData((Y2) >> 8);
	ILI9341_WriteData((Y2));

	ILI9341_WrireCommand(0x2C);

	ILI9341_uDC();
	ILI9341_CS();

	HAL_SPI_Transmit_DMA(&ILI9341_SPI, (uint8_t *)buff, (X2 - X1 + 1) * (Y2 - Y1 + 1) * 2);
}