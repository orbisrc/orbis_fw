/*
 * S6D0154X.h
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

#include "gui/dispdriver//S6D0154X.h"
#include "stm32f4xx_hal.h"
#include "spi.h"


#define COMMAND_BYTE	       		0x70
#define DATA_BYTE		       		0x72

#define REG_DRIVER_OUTPUT       	0x01
#define REG_DRIVING_WAVE_FORM     	0x02
#define REG_ENTRY_MODE           	0x03
#define REG_DISPLAY_CONTROL      	0x07
#define REG_BLANK_PERIOD         	0x08
#define REG_FRAME_CYCLE          	0x0B
#define REG_EXT_DISPLAY        		0x0C
#define REG_OSC_START            	0x0F
#define REG_PC_1                	0x10
#define REG_PC_2                	0x11
#define REG_PC_3                	0x12
#define REG_PC_4                	0x13
#define REG_PC_5                	0x14
#define REG_VCI_RECYCLING       	0x15
#define REG_Y_SET              	 	0x20
#define REG_X_SET               	0x21
#define CMD_WRITE_TO_GRAM         	0x22
#define SOFTWARE_RESET	         	0x28
#define REG_GATE_SCAN_POSITION    	0x30
#define REG_PARTIAL_X_END       	0x34
#define REG_PARTIAL_X_START     	0x35
#define REG_HORIZONTAL_END          0x36
#define REG_HORIZONTAL_START        0x37
#define REG_VERTICAL_END            0x38
#define REG_VERTICAL_START          0x39



void S6D0154X_Init()
{
	HAL_Delay(120);
	S6D0154X_WR_REG(SOFTWARE_RESET, 0x00CE);
	HAL_Delay(120);
//
	S6D0154X_WR_REG(REG_PC_2,0x001c);  // Power Control 2
	S6D0154X_WR_REG(REG_PC_3,0x1212);  // Power Control 3  BT2-0, DC11-10, DC21-20, DC31-30
	S6D0154X_WR_REG(REG_PC_4,0x0041);  // Power Control 4  DCR_EX=0, DCR2-0, GVD6-0
	S6D0154X_WR_REG(REG_PC_5,0x526a);  // Power Control 5  VCOMG=0, VCM6-0, VCMR=0, VML6-0
	S6D0154X_WR_REG(REG_PC_1,0x0c00);  // Power Control 1
	S6D0154X_Delay(10);
//
	S6D0154X_WR_REG(REG_PC_2, 0x011c); // PON=1
	S6D0154X_Delay(10);
	S6D0154X_WR_REG(REG_PC_2, 0x031c); // PON1=1
	S6D0154X_Delay(10);
	S6D0154X_WR_REG(REG_PC_2, 0x071c); // PON2=1
	S6D0154X_Delay(10);
	S6D0154X_WR_REG(REG_PC_2, 0x0F1c); // PON3=1
	S6D0154X_Delay(10);
	S6D0154X_WR_REG(REG_PC_2, 0x0F3c); // AON=1
	S6D0154X_Delay(10);
//
	// Other mode set
	S6D0154X_WR_REG(REG_DRIVER_OUTPUT, 0x0128);			//0x1028
	S6D0154X_WR_REG(REG_DRIVING_WAVE_FORM, 0x0100);		//0x100
	S6D0154X_WR_REG(REG_BLANK_PERIOD, 0x0808);
	S6D0154X_WR_REG(REG_FRAME_CYCLE, 0x1100); //  <= 72Hz
//
	// SPI
	S6D0154X_WR_REG(REG_ENTRY_MODE, 0x1030);	//1030
	S6D0154X_WR_REG(REG_EXT_DISPLAY, 0x0002);

	S6D0154X_WR_REG(0x000E, 0x0200);
	S6D0154X_WR_REG(REG_OSC_START, 0x1801);
	S6D0154X_WR_REG(REG_VCI_RECYCLING, 0x0020);  // <= EQ Mode : Vcom

	S6D0154X_WR_REG(REG_Y_SET, 0x0000);
	S6D0154X_WR_REG(REG_X_SET, 0x0000);

	S6D0154X_Delay(5);

	S6D0154X_WR_REG(REG_GATE_SCAN_POSITION, 0x0000);
	S6D0154X_WR_REG(REG_PARTIAL_X_END, 0x013f);
	S6D0154X_WR_REG(REG_PARTIAL_X_START, 0x0000);

	S6D0154X_Delay(10);

	S6D0154X_WR_REG(REG_HORIZONTAL_END, 0x00ef);
	S6D0154X_WR_REG(REG_HORIZONTAL_START, 0x0000);
	S6D0154X_WR_REG(REG_VERTICAL_END, 0x013f);
	S6D0154X_WR_REG(REG_VERTICAL_START, 0x0000);

	S6D0154X_Delay(10);
//
	// Gamma set
	S6D0154X_WR_REG(0x50,0x0101);
	S6D0154X_WR_REG(0x51,0x0603);
	S6D0154X_WR_REG(0x52,0x0408);
	S6D0154X_WR_REG(0x53,0x0000);
	S6D0154X_WR_REG(0x54,0x0605);
	S6D0154X_WR_REG(0x55,0x0406);
	S6D0154X_WR_REG(0x56,0x0303);
	S6D0154X_WR_REG(0x57,0x0303);
	S6D0154X_WR_REG(0x58,0x0010);
	S6D0154X_WR_REG(0x59,0x1000);

	S6D0154X_WR_REG(REG_OSC_START , 0x1e01);

	// Display on sequence
	S6D0154X_WR_REG(REG_DISPLAY_CONTROL, 0x0013);		// D1-0=11
	S6D0154X_Delay(10);
	S6D0154X_WrireCommand(CMD_WRITE_TO_GRAM);
}


void S6D0154X_WR_REG(uint16_t Index, uint16_t Config)
{
	S6D0154X_WrireCommand(Index);
	S6D0154X_WriteData(Config);
}


void S6D0154X_WrireCommand(uint16_t Cmd)
{
	uint8_t buff[3] = {0};

	buff[0] = COMMAND_BYTE;
	buff[1] = 0;
	buff[2] = Cmd;

	S6D0154X_CS();

	HAL_SPI_Transmit(&S6D0154X_SPI, buff, 3, 100);

	S6D0154X_uCS();
}

void S6D0154X_WriteData(uint16_t Data)
{
	uint8_t buff[3] = {0};

	buff[0] = DATA_BYTE;
	buff[1] = Data>>8;
	buff[2] = Data;

	S6D0154X_CS();

	HAL_SPI_Transmit(&S6D0154X_SPI, buff, 3, 100);

	S6D0154X_uCS();
}

void S6D0154X_SPI_Send(unsigned char Data)
{
	HAL_SPI_Transmit(&S6D0154X_SPI, &Data, 1, 1);
}

void S6D0154X_SetArea (uint16_t X, uint16_t Y, uint16_t Length, uint16_t Height)
{
	S6D0154X_WR_REG(REG_Y_SET, X);
	S6D0154X_WR_REG(REG_X_SET, Y);

	S6D0154X_WR_REG(REG_HORIZONTAL_END,X + Length);
	S6D0154X_WR_REG(REG_HORIZONTAL_START, X);
	S6D0154X_WR_REG(REG_VERTICAL_END, Y + Height);
	S6D0154X_WR_REG(REG_VERTICAL_START, Y);

	S6D0154X_WrireCommand(CMD_WRITE_TO_GRAM);
}

void S6D0154X_FillScreen (uint16_t Color)
{
	S6D0154X_SetArea(0, 0, S6D0154X_SCREEN_WIDTH - 1, S6D0154X_SCREEN_HEIGHT - 1);

	S6D0154X_ColorStream(Color, S6D0154X_SCREEN_WIDTH * S6D0154X_SCREEN_HEIGHT);
}

void S6D0154X_ColorStream (uint16_t Color, uint32_t Size)
{
	unsigned char TxBuffer[S6D0154X_BUFF_SIZE * 2] = {0};
	unsigned char ColorSh = {0};
	uint32_t i;
	uint32_t TotalSize = {0};
	uint32_t CurrentSize = {0};

	TxBuffer[0] = DATA_BYTE;
	TxBuffer[1] = 0;

	S6D0154X_CS();

	HAL_SPI_Transmit(&S6D0154X_SPI, TxBuffer, 2, 100);

	ColorSh = Color>>8;

	TotalSize = Size*2;

	if (TotalSize > S6D0154X_BUFF_SIZE)
	{
		CurrentSize = S6D0154X_BUFF_SIZE;
	}
	else
	{
		CurrentSize = TotalSize;
	}

	while (TotalSize)
	{
		for(i=0;i<CurrentSize;i++)
		{
			TxBuffer[i*2] = Color;
			TxBuffer[i*2 + 1] = ColorSh;
		}


		if (TotalSize >= CurrentSize)
		{
			TotalSize -= CurrentSize;
		}
		else
		{
			CurrentSize = TotalSize;
			TotalSize = 0;
		}

		HAL_SPI_Transmit(&S6D0154X_SPI, TxBuffer, CurrentSize * 2, 100);

	}

	S6D0154X_uCS();
}

void S6D0154X_DrawRectangle (uint16_t X, uint16_t Y, uint16_t Length, uint16_t Height, uint16_t Color)
{
	S6D0154X_SetArea(X, Y, Length - 1, Height - 1);

	S6D0154X_ColorStream(Color, Length*Height);
}

void S6D0154X_DrawHorizontalLine (uint16_t X, uint16_t Y, uint16_t Length, uint16_t Color)
{
	S6D0154X_SetArea(X, Y, Length - 1, 0);

	S6D0154X_ColorStream(Color, Length);
}

void S6D0154X_DrawVerticalLine (uint16_t X, uint16_t Y, uint16_t Height, uint16_t Color)
{
	S6D0154X_SetArea(X, Y, 0, Height - 1);

	S6D0154X_ColorStream(Color, Height);
}

void S6D0154X_DrawPixel (uint16_t X, uint16_t Y, uint16_t Color)
{
	S6D0154X_WR_REG(REG_Y_SET, X);
	S6D0154X_WR_REG(REG_X_SET, Y);
	S6D0154X_WrireCommand(CMD_WRITE_TO_GRAM);
	S6D0154X_WriteData(Color);
}

void S6D0154X_CS()
{
	HAL_GPIO_WritePin(S6D0154X_CS_PORT, S6D0154X_CS_PIN, GPIO_PIN_RESET);
}

void S6D0154X_uCS()
{
	HAL_GPIO_WritePin(S6D0154X_CS_PORT, S6D0154X_CS_PIN, GPIO_PIN_SET);
}

void S6D0154X_Delay(uint32_t ms)
{
	HAL_Delay(ms);
}
