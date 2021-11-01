/*
 * Discreteinput.c
 *
 *	Copyright 2019 Denis Davydov
 *
 *	Licensed under the Apache License, Version 2.0 (the "License");
 *	you may not use this file except in compliance with the License.
 *	You may obtDIn a copy of the License at
 *
 *		http://www.apache.org/licenses/LICENSE-2.0
 *
 *	Unless required by applicable law or agreed to in writing, software
 *	distributed under the License is distributed on an "AS IS" BASIS,
 *	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *	See the License for the specific language governing permissions and
 *	limitations under the License.
 */

#include <stdio.h>
#include <string.h>
#include "gui/screens.h"
#include "gui/stgui.h"
#include "gui/stapp.h"
#include <core/discrete.h>
#include <gui/fonts/verdana_12pt.h>
#include <gui/fonts/verdana_18pt.h>
#include <gui/fonts/verdana_22pt.h>
#include <gui/fonts/verdana_26pt.h>
#include <gui/fonts/verdana_8pt.h>
#include "core/auxiliary.h"

#define DI_NAME_X					4
#define DI_VALUE_X					40
#define DI_LIST_X					1
#define	DI_LIST_X_STEP				120
#define DI_LIST_Y					40
#define	DI_LIST_Y_STEP				20


STScreenStyleTypeDef DiscreteInputStyle = { 0 };
STScreenTypeDef DiscreteInputST = { 0 };

STColorStyleTypeDef DIButtonColor = { 0 };
STbuttonTypeDef BackToModelMenuButtonDI = { 0 };

STColorStyleTypeDef DITextColor = { 0 };
STtextBoxTypeDef DIValue[DISCRETE_CH_NUM] = { 0 };
STtextBoxTypeDef DIName[DISCRETE_CH_NUM] = { 0 };

void _DiscreteInput() {
	uint16_t i = 0;

	char Value[DISCRETE_CH_NUM][32] = { 0 };



	if (STscreenShowNow(&DiscreteInputST)) {

		/*
		 * ������� �� ���������� �����
		 */
		if (STbuttonPressed(&BackToModelMenuButtonDI)) {
			STappSetScreen(SystemMenu, &STApp);
		}

		for (i = 0; i < DISCRETE_CH_NUM; i++)
		{
			switch (DiscreteChannelGetState(&AuxSW[i])) {
				case SW_M:
					snprintf(Value[i], 32, "%s", "o");
					break;

				case SW_H:
					snprintf(Value[i], 32, "%s", "+");
					break;

				case SW_L:
					snprintf(Value[i], 32, "%s", "-");
					break;

				default:
					break;
			}

			STtextBoxSetValue(Value[i], &DIValue[i]);
		}
	}

	STscreenShow(&DiscreteInputST);

	if (STscreenOnShow(&DiscreteInputST)) {

	}

	for (i = 0; i < DISCRETE_CH_NUM; i++)
	{
		STtextBoxShow(&DIName[i]);
		STtextBoxShow(&DIValue[i]);
	}

	STbuttonShow(&BackToModelMenuButtonDI);
}
void _DiscreteInputInit() {

	uint16_t i=0;

//	char valNum[32] = { 0 };
	char Name[DISCRETE_CH_NUM][32] = { 	"SWA",
										"SWB",
										"SWC",
										"SWD",
										"SWE",
										"SWF"};

	/*
	 *  ������������� �������� ������
	 */
	DiscreteInputStyle.HeaderFont = &verdana_8pt_bold;
	DiscreteInputStyle.BackgroundColor = MAIN_SCREEN_BACK_COLOR;
	DiscreteInputStyle.HeaderColor = MAIN_HEADER_COLOR;
	DiscreteInputStyle.HeaderTextColor = MAIN_HEADER_TEXT_COLOR;
	DiscreteInputStyle.HeaderHeight = MAIN_HEADER_HEIGHT;

	DiscreteInputST.App = &STApp;
	DiscreteInputST.Style = &DiscreteInputStyle;
	DiscreteInputST.ScreenID = DiscreteInputScr;
	DiscreteInputST.FullScreen = Yes;
	DiscreteInputST.Width = GUI_MAX_WIDTH_PX;
	DiscreteInputST.Height = GUI_MAX_HEIGHT_PX;
	DiscreteInputST.Header = Yes;
	DiscreteInputST.HeaderText = "DISCRETE INPUT";
	DiscreteInputST.Footer = No;

	/*
	 *  ����� � ����� ����������
	 */
	DIButtonColor.Font = &verdana_8pt_bold;
	DIButtonColor.ParentScreen = &DiscreteInputST;
	DIButtonColor.Color = BASE_BUTTON_COLOR;
	DIButtonColor.TextColor = BASE_BUTTON_COLOR;
	DIButtonColor.BackgroundColor = BASE_BUTTON_BACK_COLOR;
	DIButtonColor.BorderThickness = BASE_BUTTON_BORDER_THICKNESS;
	DIButtonColor.BorderColor = BASE_BUTTON_BORDER_COLOR;
	DIButtonColor.FocusColor = FOCUS_BUTTON_COLOR;
	DIButtonColor.FocusBackgroundColor = FOCUS_BUTTON_BACK_COLOR;
	DIButtonColor.FocusBorderThickness = FOCUS_BUTTON_BORDER_THICKNESS;
	DIButtonColor.FocusBorderColor = FOCUS_BUTTON_BORDER_COLOR;

	DITextColor.ParentScreen = &DiscreteInputST;
	DITextColor.Font = &verdana_8pt_bold;
	DITextColor.Color = BASE_TEXT_COLOR;
	DITextColor.TextColor = BASE_TEXT_COLOR;
	DITextColor.BackgroundColor = BASE_TEXT_BACK_COLOR;
	DITextColor.BorderThickness = BASE_TEXT_BORDER_THICKNESS;
	DITextColor.BorderColor = BASE_TEXT_BORDER_COLOR;

	for (i = 0; i < DISCRETE_CH_NUM; i++) {
//		snprintf(valNum, 32, "DI%d", i);

		DIValue[i].Text = "----";
		DIValue[i].Style = &DITextColor;
		DIValue[i].Width = 60;
		DIValue[i].Height = 26;
		DIValue[i].X = DI_VALUE_X;
		DIValue[i].Y = DI_LIST_Y + DI_LIST_Y_STEP * i;
		DIValue[i].Border = No;
		DIValue[i].TextAlignment = Left;
		DIValue[i].Dynamic = No;

		DIName[i].Text = Name[i];
		DIName[i].Style = &DITextColor;
		DIName[i].Width = 40;
		DIName[i].Height = 26;
		DIName[i].X = DI_NAME_X;
		DIName[i].Y = DI_LIST_Y + DI_LIST_Y_STEP * i;
		DIName[i].Border = No;
		DIName[i].TextAlignment = Left;
		DIName[i].Dynamic = No;

		STtextBoxInit(&DIValue[i]);
		STtextBoxInit(&DIName[i]);
	}

	BackToModelMenuButtonDI.Text = __buttonLabelBack;
	BackToModelMenuButtonDI.Style = &DIButtonColor;
	BackToModelMenuButtonDI.Width = 60;
	BackToModelMenuButtonDI.Height = 26;
	BackToModelMenuButtonDI.X = MAIN_BACK_BUTTON_X;
	BackToModelMenuButtonDI.Y = MAIN_BACK_BUTTON_Y;
	BackToModelMenuButtonDI.Border = Yes;

	STbuttonInit(&BackToModelMenuButtonDI);
}
