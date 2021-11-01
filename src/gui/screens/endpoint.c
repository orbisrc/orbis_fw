/*
 * endpoint.c
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
#include "gui/stapp.h"
#include <core/iosettings.h>
#include <core/rcchannel.h>
#include <gui/fonts/verdana_18pt.h>
#include <gui/fonts/verdana_22pt.h>
#include <gui/fonts/verdana_26pt.h>
#include <gui/fonts/verdana_8pt.h>
#include <gui/fonts/verdana_8pt.h>

#define CHET_NAME_X					80
#define CHET_LOWRATE_X				120
#define CHET_HIGHTRATE_X			180
#define CHET_LINE_Y					23
#define CHET_LINE_STEP				18

STScreenStyleTypeDef EndPointStyle = { 0 };
STScreenTypeDef EndPointST = { 0 };
STColorStyleTypeDef EndPointButtonColor = { 0 };		// �������� ����� ������
STColorStyleTypeDef EndPointInputBoxColor = { 0 };// �������� ����� ����� �����
STColorStyleTypeDef EndPointText = { 0 };
STbuttonTypeDef BackToModelMenuButtonEP = { 0 };
STinputBoxTypeDef CHLowRate[MAX_RC_CHANNEL] = { 0 };
STinputBoxTypeDef CHHightRate[MAX_RC_CHANNEL] = { 0 };
STtextBoxTypeDef CHNameEndPoint[MAX_RC_CHANNEL] = { 0 };

void _EndPoint()
{
	uint16_t i = 0;

	if (STscreenShowNow(&EndPointST)) {
		if (STbuttonPressed(&BackToModelMenuButtonEP)) {
			STappSetScreen(ModelMenu, &STApp);
		}

		for(i = 0; i < MAX_RC_CHANNEL; i++)
		{
			if (STinputBoxValueChange(&CHLowRate[i])) {
				RCChanelSetLowRate((uint16_t) STinputBoxGetValue(&CHLowRate[i]), &RCChanel[i]);
			}

			if (STinputBoxValueChange(&CHHightRate[i]))
			{
				RCChanelSetHightRate((uint16_t) STinputBoxGetValue(&CHHightRate[i]), &RCChanel[i]);
			}
		}
	}

	STscreenShow(&EndPointST);

	if (STscreenOnShow(&EndPointST))
	{
		for(i = 0; i < MAX_RC_CHANNEL; i++)
		{
			STinputBoxSetValue(RCChanelGetLowRate(&RCChanel[i]), &CHLowRate[i]);
			STinputBoxSetValue(RCChanelGetHighRate(&RCChanel[i]), &CHHightRate[i]);
		}
	}


	for(i = 0; i < MAX_RC_CHANNEL; i++)
	{
		STtextBoxShow(&CHNameEndPoint[i]);
		STinputBoxShow(&CHLowRate[i]);
		STinputBoxShow(&CHHightRate[i]);
	}

	STbuttonShow(&BackToModelMenuButtonEP);
}
void _EndPointInit()
{
	uint8_t i = 0;

	EndPointStyle.HeaderFont = &verdana_8pt_bold;
	EndPointStyle.BackgroundColor = MAIN_SCREEN_BACK_COLOR;
	EndPointStyle.HeaderColor = MAIN_HEADER_COLOR;
	EndPointStyle.HeaderTextColor = MAIN_HEADER_TEXT_COLOR;
	EndPointStyle.HeaderHeight = MAIN_HEADER_HEIGHT;

	EndPointST.App = &STApp;
	EndPointST.Style = &EndPointStyle;
	EndPointST.ScreenID = EndPoint;
	EndPointST.FullScreen = Yes;
	EndPointST.Width = GUI_MAX_WIDTH_PX;
	EndPointST.Height = GUI_MAX_HEIGHT_PX;
	EndPointST.Header = Yes;
	EndPointST.HeaderText = "END POINT";
	EndPointST.Footer = No;

	/*
	 *  ����� � ����� ����������
	 */

	EndPointButtonColor.Font = &verdana_8pt_bold;
	EndPointButtonColor.ParentScreen = &EndPointST;
	EndPointButtonColor.Color = BASE_BUTTON_COLOR;
	EndPointButtonColor.TextColor = BASE_BUTTON_COLOR;
	EndPointButtonColor.BackgroundColor = BASE_BUTTON_BACK_COLOR;
	EndPointButtonColor.BorderThickness = BASE_BUTTON_BORDER_THICKNESS;
	EndPointButtonColor.BorderColor = BASE_BUTTON_BORDER_COLOR;
	EndPointButtonColor.FocusColor = FOCUS_BUTTON_COLOR;
	EndPointButtonColor.FocusBackgroundColor = FOCUS_BUTTON_BACK_COLOR;
	EndPointButtonColor.FocusBorderThickness = FOCUS_BUTTON_BORDER_THICKNESS;
	EndPointButtonColor.FocusBorderColor = FOCUS_BUTTON_BORDER_COLOR;

	EndPointInputBoxColor.ParentScreen = &EndPointST;
	EndPointInputBoxColor.Font = &verdana_8pt_bold;
	EndPointInputBoxColor.Color = BASE_INPUT_COLOR;
	EndPointInputBoxColor.TextColor = BASE_INPUT_TEXT_COLOR;
	EndPointInputBoxColor.BackgroundColor = BASE_INPUT_BACK_COLOR;
	EndPointInputBoxColor.BorderThickness = BASE_INPUT_BORDER_THICKNESS;
	EndPointInputBoxColor.BorderColor = BASE_INPUT_BORDER_COLOR;
	EndPointInputBoxColor.FocusTextColor = FOCUS_INPUT_TEXT_COLOR;
	EndPointInputBoxColor.FocusColor = FOCUS_INPUT_COLOR;
	EndPointInputBoxColor.FocusBackgroundColor = FOCUS_INPUT_BACK_COLOR;
	EndPointInputBoxColor.FocusBorderThickness = FOCUS_INPUT_BORDER_THICKNESS;
	EndPointInputBoxColor.FocusBorderColor = FOCUS_INPUT_BORDER_COLOR;

	EndPointText.ParentScreen = &EndPointST;
	EndPointText.Font = &verdana_8pt_bold;
	EndPointText.Color = BASE_TEXT_COLOR;
	EndPointText.TextColor = BASE_TEXT_COLOR;
	EndPointText.BackgroundColor = BASE_TEXT_BACK_COLOR;
	EndPointText.BorderThickness = BASE_TEXT_BORDER_THICKNESS;
	EndPointText.BorderColor = BASE_TEXT_BORDER_COLOR;



	for(i = 0; i < MAX_RC_CHANNEL; i++)
	{
		CHNameEndPoint[i].Text = __CHLabel[i];
		CHNameEndPoint[i].Style = &EndPointText;
		CHNameEndPoint[i].Width = 30;
		CHNameEndPoint[i].Height = INPUT_BASE_HEIGHT;
		CHNameEndPoint[i].X = CHET_NAME_X;
		CHNameEndPoint[i].Y = CHET_LINE_Y + CHET_LINE_STEP * i;
		CHNameEndPoint[i].Border = No;
		CHNameEndPoint[i].TextAlignment = Right;
		CHNameEndPoint[i].Dynamic = No;

		CHHightRate[i].Style = &EndPointInputBoxColor;
		CHHightRate[i].Width = INPUT_BASE_WIDTH;
		CHHightRate[i].Height = INPUT_BASE_HEIGHT;
		CHHightRate[i].X = CHET_HIGHTRATE_X;
		CHHightRate[i].Y = CHET_LINE_Y + CHET_LINE_STEP * i;
		CHHightRate[i].HiLimit = 100;
		CHHightRate[i].LoLimit = 0;
		CHHightRate[i].TextAlignment = Centre;
		CHHightRate[i].Border = Yes;
		CHHightRate[i].Dynamic = Yes;
		CHHightRate[i].Units = "%";
		CHHightRate[i].UnitsShow = Yes;

		CHLowRate[i].Style = &EndPointInputBoxColor;
		CHLowRate[i].Width = INPUT_BASE_WIDTH;
		CHLowRate[i].Height = INPUT_BASE_HEIGHT;
		CHLowRate[i].X = CHET_LOWRATE_X;
		CHLowRate[i].Y = CHET_LINE_Y + CHET_LINE_STEP * i;
		CHLowRate[i].HiLimit = 100;
		CHLowRate[i].LoLimit = 0;
		CHLowRate[i].TextAlignment = Centre;
		CHLowRate[i].Border = Yes;
		CHLowRate[i].Dynamic = Yes;
		CHLowRate[i].Units = "%";
		CHLowRate[i].UnitsShow = Yes;

		STtextBoxInit(&CHNameEndPoint[i]);
		STinputBoxInit(&CHLowRate[i]);
		STinputBoxInit(&CHHightRate[i]);
	}


	BackToModelMenuButtonEP.Text = __buttonLabelBack;
	BackToModelMenuButtonEP.Style = &EndPointButtonColor;
	BackToModelMenuButtonEP.Width = 60;
	BackToModelMenuButtonEP.Height = 26;
	BackToModelMenuButtonEP.X = MAIN_BACK_BUTTON_X;
	BackToModelMenuButtonEP.Y = MAIN_BACK_BUTTON_Y;
	BackToModelMenuButtonEP.Border = Yes;

	STbuttonInit(&BackToModelMenuButtonEP);
}
