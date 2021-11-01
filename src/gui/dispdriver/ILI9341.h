/*
 * ILI9341.h
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

#ifndef INC_STGUI_DISPDRIVER_ILI9341_H_
#define INC_STGUI_DISPDRIVER_ILI9341_H_

#include "main.h"
#include "periphery/gpio.h"

#define ILI9341_SCREEN_HEIGHT   			320
#define ILI9341_SCREEN_WIDTH 				240

#define ILI9341_BUFF_SIZE	 				1024

#define SCREEN_VERTICAL_1					0
#define SCREEN_HORIZONTAL_1					1
#define SCREEN_VERTICAL_2					2
#define SCREEN_HORIZONTAL_2					3

#define ILI9341_CS_PIN						SPI2_CS_Pin
#define ILI9341_CS_PORT 					SPI2_CS_GPIO_Port
#define ILI9341_DC_PIN						SPI1_DC_Pin
#define ILI9341_DC_PORT 					SPI1_DC_GPIO_Port
#define ILI9341_SPI							hspi2

void ILI9341_Init();

void ILI9341_WR_REG(uint16_t Index, uint16_t Config);

void ILI9341_WrireCommand(uint8_t Cmd);

void ILI9341_WriteData(uint8_t Data);

void ILI9341_SPI_Send(unsigned char Data);

void ILI9341_SetArea (uint16_t X, uint16_t Y, uint16_t Length, uint16_t Height);

void ILI9341_DrawPixel (uint16_t X, uint16_t Y, uint16_t Color);

void ILI9341_FillScreen (uint16_t Color);

void ILI9341_ColorStream (uint16_t Color, uint32_t Size);

void ILI9341_DrawRectangle (uint16_t X, uint16_t Y, uint16_t Length, uint16_t Height, uint16_t Color);

void ILI9341_DrawHorizontalLine (uint16_t X, uint16_t Y, uint16_t Length, uint16_t Color);

void ILI9341_DrawVerticalLine (uint16_t X, uint16_t Y, uint16_t Height, uint16_t Color);

void ILI9341_SetRotation(uint8_t Rotation);

void ILI9341_CS();

void ILI9341_uCS();

void ILI9341_DC();

void ILI9341_uDC();

void ILI9341_Delay(uint32_t ms);

#endif /* INC_STGUI_DISPDRIVER_ILI9341_H_ */
