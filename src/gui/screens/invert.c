/*
 * invert.c
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
#include <gui/fonts/verdana_12pt.h>
#include <gui/fonts/verdana_18pt.h>
#include <gui/fonts/verdana_22pt.h>
#include <gui/fonts/verdana_26pt.h>
#include <gui/fonts/verdana_8pt.h>

#define CHI_NAME_X				110
#define CHI_CHECKBOX_X			160

#define CHI_LINE_Y				23
#define CHI_LINE_STEP			18


STScreenStyleTypeDef CHinvertStyle = { 0 };
STScreenTypeDef CHinvertST = { 0 };
STColorStyleTypeDef InvertText = { 0 };
STColorStyleTypeDef CHinvertButtonColor = { 0 };
STColorStyleTypeDef CHinvertCheckBoxColor = { 0 };
STbuttonTypeDef BackToModelMenuButtonCHI = { 0 };

STcheckBoxTypeDef CHinvertCheckBox[MAX_RC_CHANNEL] = { 0 };
STtextBoxTypeDef CHNameInvert[MAX_RC_CHANNEL]  = { 0 };

void _Invert()
{
	uint16_t i = 0 ;

	if (STscreenShowNow(&CHinvertST))
	{


		for (i = 0 ; i < MAX_RC_CHANNEL;i++)
		{
			if (STcheckBoxValueChange(&CHinvertCheckBox[i]))
			{
				RCChanelSetInvertState(STcheckBoxGetState(&CHinvertCheckBox[i]),&RCChanel[i]);
			}
		}

		if (STbuttonPressed(&BackToModelMenuButtonCHI)) {
			STappSetScreen(ModelMenu, &STApp);
/*
 *
 *
 * Добавить сохранение настроек
 *
 *
 */
		}
	}

	STscreenShow(&CHinvertST);

	if (STscreenOnShow(&CHinvertST)) {

		for (i = 0 ; i < MAX_RC_CHANNEL;i++)
		{
			STcheckBoxSetValue(RCChanelGetInvertState(&RCChanel[i]),	&CHinvertCheckBox[i]);
		}
	}

	for (i = 0 ; i < MAX_RC_CHANNEL;i++)
	{
		STcheckBoxShow(&CHinvertCheckBox[i]);
		STtextBoxShow(&CHNameInvert[i]);
	}

	STbuttonShow(&BackToModelMenuButtonCHI);
}
void _InvertInit() {

	uint16_t i = 0 ;

	CHinvertStyle.HeaderFont = &verdana_8pt_bold;
	CHinvertStyle.BackgroundColor = MAIN_SCREEN_BACK_COLOR;
	CHinvertStyle.HeaderColor = MAIN_HEADER_COLOR;
	CHinvertStyle.HeaderTextColor = MAIN_HEADER_TEXT_COLOR;
	CHinvertStyle.HeaderHeight = MAIN_HEADER_HEIGHT;

	CHinvertST.App = &STApp;
	CHinvertST.Style = &CHinvertStyle;
	CHinvertST.ScreenID = ChannelReverse;
	CHinvertST.FullScreen = Yes;
	CHinvertST.Width = GUI_MAX_WIDTH_PX;
	CHinvertST.Height = GUI_MAX_HEIGHT_PX;
	CHinvertST.Header = Yes;
	CHinvertST.HeaderText = "CH INVERT";
	CHinvertST.Footer = No;

	/*
	 *  ����� � ����� ����������
	 */



	CHinvertButtonColor.Font = &verdana_8pt_bold;
	CHinvertButtonColor.ParentScreen = &CHinvertST;
	CHinvertButtonColor.Color = BASE_BUTTON_COLOR;
	CHinvertButtonColor.TextColor = BASE_BUTTON_COLOR;
	CHinvertButtonColor.BackgroundColor = BASE_BUTTON_BACK_COLOR;
	CHinvertButtonColor.BorderThickness = BASE_BUTTON_BORDER_THICKNESS;
	CHinvertButtonColor.BorderColor = BASE_BUTTON_BORDER_COLOR;
	CHinvertButtonColor.FocusColor = FOCUS_BUTTON_COLOR;
	CHinvertButtonColor.FocusBackgroundColor = FOCUS_BUTTON_BACK_COLOR;
	CHinvertButtonColor.FocusBorderThickness = FOCUS_BUTTON_BORDER_THICKNESS;
	CHinvertButtonColor.FocusBorderColor = FOCUS_BUTTON_BORDER_COLOR;

	CHinvertCheckBoxColor.ParentScreen = &CHinvertST;
	CHinvertCheckBoxColor.Font = &verdana_8pt_bold;
	CHinvertCheckBoxColor.Color = BASE_CHECKBOX_OFF_COLOR;
	CHinvertCheckBoxColor.Color2 = BASE_CHECKBOX_ON_COLOR;
	CHinvertCheckBoxColor.TextColor = BASE_CHECKBOX_TEXT_COLOR;
	CHinvertCheckBoxColor.BorderThickness = BASE_CHECKBOX_BORDER_THICKNESS;
	CHinvertCheckBoxColor.BorderColor = BASE_CHECKBOX_BORDER_COLOR;
	CHinvertCheckBoxColor.FocusBorderThickness =FOCUS_CHECKBOX_BORDER_THICKNESS;
	CHinvertCheckBoxColor.FocusBorderColor = FOCUS_CHECKBOX_BORDER_COLOR;

	InvertText.ParentScreen = &CHinvertST;
	InvertText.Font = &verdana_8pt_bold;
	InvertText.Color = BASE_TEXT_COLOR;
	InvertText.TextColor = BASE_TEXT_COLOR;
	InvertText.BackgroundColor = BASE_TEXT_BACK_COLOR;
	InvertText.BorderThickness = BASE_TEXT_BORDER_THICKNESS;
	InvertText.BorderColor = BASE_TEXT_BORDER_COLOR;


	for (i = 0 ; i < MAX_RC_CHANNEL;i++)
	{
		CHinvertCheckBox[i].OnText = __buttonLabelCheckBoxOn;
		CHinvertCheckBox[i].OffText = __buttonLabelCheckBoxOff;
		CHinvertCheckBox[i].Style = &CHinvertCheckBoxColor;
		CHinvertCheckBox[i].Width = BASE_CHECKBOX_WIDTH;
		CHinvertCheckBox[i].Height = BASE_CHECKBOX_HEIGHT;
		CHinvertCheckBox[i].X = CHI_CHECKBOX_X;
		CHinvertCheckBox[i].Y = CHI_LINE_Y + i * CHI_LINE_STEP;
		CHinvertCheckBox[i].Border = Yes;
		CHinvertCheckBox[i].Dynamic = Yes;


		CHNameInvert[i].Text = __CHLabel[i];
		CHNameInvert[i].Style = &InvertText;
		CHNameInvert[i].Width = 30;
		CHNameInvert[i].Height = BASE_CHECKBOX_HEIGHT;
		CHNameInvert[i].X = CHI_NAME_X;
		CHNameInvert[i].Y = CHI_LINE_Y + i * CHI_LINE_STEP;
		CHNameInvert[i].Border = No;
		CHNameInvert[i].TextAlignment = Right;
		CHNameInvert[i].Dynamic = No;

		STtextBoxInit(&CHNameInvert[i]);
		STcheckBoxInit(&CHinvertCheckBox[i]);
	}


	BackToModelMenuButtonCHI.Text = __buttonLabelBack;
	BackToModelMenuButtonCHI.Style = &CHinvertButtonColor;
	BackToModelMenuButtonCHI.Width = MAIN_BACK_BUTTON_WIDTH;
	BackToModelMenuButtonCHI.Height = MAIN_BACK_BUTTON_HEIGHT;
	BackToModelMenuButtonCHI.X = MAIN_BACK_BUTTON_X;
	BackToModelMenuButtonCHI.Y = MAIN_BACK_BUTTON_Y;
	BackToModelMenuButtonCHI.Border = Yes;

	STbuttonInit(&BackToModelMenuButtonCHI);
}

