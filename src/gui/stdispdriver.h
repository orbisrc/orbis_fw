/*
 * stdispdriver.h
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

#ifndef STGUI_STDISPDRIVER_H_
#define STGUI_STDISPDRIVER_H_

#include "gui/fonts/font.h"
#include "lvgl.h"

void STDrawPicture(uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height, const unsigned char *Picture);

void STDrawPixel(uint16_t X, uint16_t Y, uint16_t Color);

void STDrawHorizontalLine(uint16_t X, uint16_t Y, uint16_t Length,	uint16_t Color);

void STDrawVerticalLine(uint16_t X, uint16_t Y, uint16_t Length, uint16_t Color);

void STDrawRectangle(uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height, uint16_t Color);

void STDrawFilledRectangle(uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height, uint16_t FillColor);

void STFillScreen(uint16_t Color);

/*
 *  ������� ��������� ������� � ������
 */
void STPutChar(const char Char, const FONT_INFO *Font, uint16_t X, uint16_t Y, uint16_t Color);

void STPutText(const char* String, const FONT_INFO *Font, uint16_t X, uint16_t Y, uint16_t Color, uint16_t BackgroundColor);

/*
 *  ����������� ��������� ��� ����������� ����������
 */

void STFrame(uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height, uint16_t Thickness, uint16_t Color);

void STRectangle(uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height, uint16_t BorderThickness, uint16_t BorderColor, uint16_t Color);

void STLCDinit(void);

void STLCDsetBrightness(uint16_t Value);

uint16_t STLCDgetBrightness(void);

void STLCDsetBrightnessInit(void);

void lv_draw_area(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, lv_color_t *buff);

#endif /* STGUI_STDISPDRIVER_H_ */
