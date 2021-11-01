/*
 * analoginput.c
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

#include <stdio.h>
#include <string.h>
#include "gui/screens.h"
#include "gui/stgui.h"
#include "gui/gui.h"
#include "gui/stapp.h"
#include <core/analog.h>
#include <gui/fonts/verdana_12pt.h>
#include <gui/fonts/verdana_18pt.h>
#include <gui/fonts/verdana_22pt.h>
#include <gui/fonts/verdana_26pt.h>
#include <gui/fonts/verdana_8pt.h>
#include "core/auxiliary.h"
#include "core/filter.h"

#define AI_NAME_X					4
#define AI_VALUE_X					40
#define AI_INPUT_VALUE_X			90
#define AI_LIST_X					1
#define	AI_LIST_X_STEP				120
#define AI_LIST_Y					40
#define	AI_LIST_Y_STEP				20


STScreenStyleTypeDef AnalogInputStyle = { 0 };
STScreenTypeDef AnalogInputST = { 0 };

STColorStyleTypeDef AIButtonColor = { 0 };
STbuttonTypeDef BackToModelMenuButtonAI = { 0 };

STColorStyleTypeDef AITextColor = { 0 };
STtextBoxTypeDef AIValue[ADC1_CH_NUM] = { 0 };
STtextBoxTypeDef AIinputValue[ADC1_CH_NUM] = { 0 };
STtextBoxTypeDef AIName[ADC1_CH_NUM] = { 0 };

void _AnalogInput() {
	uint16_t i = 0;

	char Value[ADC1_CH_NUM][32] = { 0 };
	char InputValue[ADC1_CH_NUM][32] = { 0 };

	if (STscreenShowNow(&AnalogInputST)) {

		/*
		 * ������� �� ���������� �����
		 */
		if (STbuttonPressed(&BackToModelMenuButtonAI)) {
			STappSetScreen(SystemMenu, &STApp);
		}

		for (i = 0; i < ADC1_CH_NUM; i++)
		{
			snprintf(Value[i], 32, "%d", AIgetADCValue(&AnalogChannel[i]));
			STtextBoxSetValue(Value[i], &AIValue[i]);

			snprintf(InputValue[i], 32, "%d", (uint16_t)ADCbuff[i]); //(uint16_t)AIgetInputValue(&AnalogChannel[i])
			STtextBoxSetValue(InputValue[i], &AIinputValue[i]);
		}
	}

	STscreenShow(&AnalogInputST);

	if (STscreenOnShow(&AnalogInputST)) {

	}

	for (i = 0; i < ADC1_CH_NUM; i++)
	{
		STtextBoxShow(&AIName[i]);
		STtextBoxShow(&AIValue[i]);
		STtextBoxShow(&AIinputValue[i]);
	}

	STbuttonShow(&BackToModelMenuButtonAI);
}
void _AnalogInputInit() {

	uint16_t i=0;

	char valNum[32] = { 0 };

	/*
	 *  ������������� �������� ������
	 */
	AnalogInputStyle.HeaderFont = &verdana_8pt_bold;
	AnalogInputStyle.BackgroundColor = MAIN_SCREEN_BACK_COLOR;
	AnalogInputStyle.HeaderColor = MAIN_HEADER_COLOR;
	AnalogInputStyle.HeaderTextColor = MAIN_HEADER_TEXT_COLOR;
	AnalogInputStyle.HeaderHeight = MAIN_HEADER_HEIGHT;

	AnalogInputST.App = &STApp;
	AnalogInputST.Style = &AnalogInputStyle;
	AnalogInputST.ScreenID = AnalogInputScr;
	AnalogInputST.FullScreen = Yes;
	AnalogInputST.Width = GUI_MAX_WIDTH_PX;
	AnalogInputST.Height = GUI_MAX_HEIGHT_PX;
	AnalogInputST.Header = Yes;
	AnalogInputST.HeaderText = "ANALOG INPUT";
	AnalogInputST.Footer = No;

	/*
	 *  ����� � ����� ����������
	 */
	AIButtonColor.Font = &verdana_8pt_bold;
	AIButtonColor.ParentScreen = &AnalogInputST;
	AIButtonColor.Color = BASE_BUTTON_COLOR;
	AIButtonColor.TextColor = BASE_BUTTON_COLOR;
	AIButtonColor.BackgroundColor = BASE_BUTTON_BACK_COLOR;
	AIButtonColor.BorderThickness = BASE_BUTTON_BORDER_THICKNESS;
	AIButtonColor.BorderColor = BASE_BUTTON_BORDER_COLOR;
	AIButtonColor.FocusColor = FOCUS_BUTTON_COLOR;
	AIButtonColor.FocusBackgroundColor = FOCUS_BUTTON_BACK_COLOR;
	AIButtonColor.FocusBorderThickness = FOCUS_BUTTON_BORDER_THICKNESS;
	AIButtonColor.FocusBorderColor = FOCUS_BUTTON_BORDER_COLOR;

	AITextColor.ParentScreen = &AnalogInputST;
	AITextColor.Font = &verdana_8pt_bold;
	AITextColor.Color = BASE_TEXT_COLOR;
	AITextColor.TextColor = BASE_TEXT_COLOR;
	AITextColor.BackgroundColor = BASE_TEXT_BACK_COLOR;
	AITextColor.BorderThickness = BASE_TEXT_BORDER_THICKNESS;
	AITextColor.BorderColor = BASE_TEXT_BORDER_COLOR;

	for (i = 0; i < ADC1_CH_NUM; i++) {
		snprintf(valNum, 32, "AI%d", i);

		AIValue[i].Text = "----";
		AIValue[i].Style = &AITextColor;
		AIValue[i].Width = 60;
		AIValue[i].Height = 26;
		AIValue[i].X = AI_VALUE_X;
		AIValue[i].Y = AI_LIST_Y + AI_LIST_Y_STEP * i;
		AIValue[i].Border = No;
		AIValue[i].TextAlignment = Left;
		AIValue[i].Dynamic = No;

		AIinputValue[i].Text = "----";
		AIinputValue[i].Style = &AITextColor;
		AIinputValue[i].Width = 60;
		AIinputValue[i].Height = 26;
		AIinputValue[i].X = AI_INPUT_VALUE_X;
		AIinputValue[i].Y = AI_LIST_Y + AI_LIST_Y_STEP * i;
		AIinputValue[i].Border = No;
		AIinputValue[i].TextAlignment = Left;
		AIinputValue[i].Dynamic = No;

		AIName[i].Text = valNum;
		AIName[i].Style = &AITextColor;
		AIName[i].Width = 40;
		AIName[i].Height = 26;
		AIName[i].X = AI_NAME_X;
		AIName[i].Y = AI_LIST_Y + AI_LIST_Y_STEP * i;
		AIName[i].Border = No;
		AIName[i].TextAlignment = Left;
		AIName[i].Dynamic = No;

		STtextBoxInit(&AIValue[i]);
		STtextBoxInit(&AIName[i]);
	}

	BackToModelMenuButtonAI.Text = __buttonLabelBack;
	BackToModelMenuButtonAI.Style = &AIButtonColor;
	BackToModelMenuButtonAI.Width = 60;
	BackToModelMenuButtonAI.Height = 26;
	BackToModelMenuButtonAI.X = MAIN_BACK_BUTTON_X;
	BackToModelMenuButtonAI.Y = MAIN_BACK_BUTTON_Y;
	BackToModelMenuButtonAI.Border = Yes;

	STbuttonInit(&BackToModelMenuButtonAI);
}
