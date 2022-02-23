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

#ifndef INC_STGUI_DISPDRIVER_S6D0154X_H_
#define INC_STGUI_DISPDRIVER_S6D0154X_H_

#include "main.h"
#include "periphery/gpio.h"

#define S6D0154X_SCREEN_HEIGHT   			320
#define S6D0154X_SCREEN_WIDTH 				240

#define S6D0154X_BUFF_SIZE	 				1024

#define S6D0154X_CS_PIN						SPI2_CS_Pin
#define S6D0154X_CS_PORT 					SPI2_CS_GPIO_Port
#define S6D0154X_SPI						hspi2

void S6D0154X_Init();

void S6D0154X_WR_REG(uint16_t Index, uint16_t Config);

void S6D0154X_WrireCommand(uint16_t Cmd);

void S6D0154X_WriteData(uint16_t Data);

void S6D0154X_SPI_Send(unsigned char Data);

void S6D0154X_SetArea (uint16_t X, uint16_t Y, uint16_t Length, uint16_t Height);

void S6D0154X_DrawPixel (uint16_t X, uint16_t Y, uint16_t Color);

void S6D0154X_FillScreen (uint16_t Color);

void S6D0154X_ColorStream (uint16_t Color, uint32_t Size);

void S6D0154X_DrawRectangle (uint16_t X, uint16_t Y, uint16_t Length, uint16_t Height, uint16_t Color);

void S6D0154X_DrawHorizontalLine (uint16_t X, uint16_t Y, uint16_t Length, uint16_t Color);

void S6D0154X_DrawVerticalLine (uint16_t X, uint16_t Y, uint16_t Height, uint16_t Color);

void S6D0154X_CS();

void S6D0154X_uCS();

void S6D0154X_Delay(uint32_t ms);

void S6D0154X_Draw_Area_DMA(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t *buff);

#endif /* INC_STGUI_DISPDRIVER_S6D0154X_H_ */
