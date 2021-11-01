/*
 * trim.c
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

#define CHT_NAME_X				4
#define CHT_VALUE_X				206
#define CHT_BAR_X				45
#define CHT_LINE_Y				25
#define CHT_LINE_STEP			16
#define ST_TRIM_LINE_WIDTH		150
#define ST_TRIM_LINE_HEIGHT		12

STScreenStyleTypeDef SubTrimStyle = { 0 };
STScreenTypeDef SubTrimST = { 0 };
STColorStyleTypeDef TrimText = { 0 };
STColorStyleTypeDef SubTrimButtonColor = { 0 };			// �������� ����� ������
STbuttonTypeDef BackToModelMenuButtonST = { 0 };
STColorStyleTypeDef TrimStyleST = { 0 };

STtextBoxTypeDef CHNameTrim[MAX_RC_CHANNEL] = { 0 };
STtextBoxTypeDef CHValueTrim[MAX_RC_CHANNEL] = { 0 };
STtrimBarTypeDef CHtrimST[MAX_RC_CHANNEL] = { 0 };

void _Trim() {
	char CHtrim[MAX_RC_CHANNEL][8] = { 0 };
	uint16_t i = 0;


	if (STscreenShowNow(&SubTrimST))
	{


		for (i = 0 ; i < MAX_RC_CHANNEL;i++)
		{
			snprintf(CHtrim[i], 8, "%d", RCChanelGetTrim(&RCChanel[i]));
			STtextBoxSetValue(CHtrim[i], &CHValueTrim[i]);

			if (STtrimBarValueChange(&CHtrimST[i]))
			{
				RCChanelSetTrim(STtrimBarGetValue(&CHtrimST[i]), &RCChanel[i]);
			}
		}

		if (STbuttonPressed(&BackToModelMenuButtonST)) {

			STappSetScreen(ModelMenu, &STApp);

		}
	}

	STscreenShow(&SubTrimST);

	if (STscreenOnShow(&SubTrimST))
	{

		for (i = 0 ; i < MAX_RC_CHANNEL;i++)
		{
			STtrimBarSetValue(RCChanelGetTrim(&RCChanel[i]), &CHtrimST[i]);
		}
	}


	for (i = 0 ; i < MAX_RC_CHANNEL;i++)
	{
		STtextBoxShow(&CHNameTrim[i]);
		STtrimBarShow(&CHtrimST[i]);
		STtextBoxShow(&CHValueTrim[i]);
	}

	STbuttonShow(&BackToModelMenuButtonST);
}
void _TrimInit() {

	uint16_t i = 0 ;

	/*
	 *  ������������� �������� ������
	 */
	SubTrimStyle.HeaderFont = &verdana_8pt_bold;
	SubTrimStyle.BackgroundColor = MAIN_SCREEN_BACK_COLOR;
	SubTrimStyle.HeaderColor = MAIN_HEADER_COLOR;
	SubTrimStyle.HeaderTextColor = MAIN_HEADER_TEXT_COLOR;
	SubTrimStyle.HeaderHeight = MAIN_HEADER_HEIGHT;

	SubTrimST.App = &STApp;
	SubTrimST.Style = &SubTrimStyle;
	SubTrimST.ScreenID = ChannelTrim;
	SubTrimST.FullScreen = Yes;
	SubTrimST.Width = GUI_MAX_WIDTH_PX;
	SubTrimST.Height = GUI_MAX_HEIGHT_PX;
	SubTrimST.Header = Yes;
	SubTrimST.HeaderText = "SUB TRIM";
	SubTrimST.Footer = No;

	/*
	 *  ����� � ����� ����������
	 */

	SubTrimButtonColor.Font = &verdana_8pt_bold;
	SubTrimButtonColor.ParentScreen = &SubTrimST;
	SubTrimButtonColor.Color = BASE_BUTTON_COLOR;
	SubTrimButtonColor.TextColor = BASE_BUTTON_COLOR;
	SubTrimButtonColor.BackgroundColor = BASE_BUTTON_BACK_COLOR;
	SubTrimButtonColor.BorderThickness = BASE_BUTTON_BORDER_THICKNESS;
	SubTrimButtonColor.BorderColor = BASE_BUTTON_BORDER_COLOR;
	SubTrimButtonColor.FocusColor = FOCUS_BUTTON_COLOR;
	SubTrimButtonColor.FocusBackgroundColor = FOCUS_BUTTON_BACK_COLOR;
	SubTrimButtonColor.FocusBorderThickness = FOCUS_BUTTON_BORDER_THICKNESS;
	SubTrimButtonColor.FocusBorderColor = FOCUS_BUTTON_BORDER_COLOR;

	TrimText.ParentScreen = &SubTrimST;
	TrimText.Font = &verdana_8pt_bold;
	TrimText.Color = BASE_TEXT_COLOR;
	TrimText.TextColor = BASE_TEXT_COLOR;
	TrimText.BackgroundColor = BASE_TEXT_BACK_COLOR;
	TrimText.BorderThickness = BASE_TEXT_BORDER_THICKNESS;
	TrimText.BorderColor = BASE_TEXT_BORDER_COLOR;

	TrimStyleST.ParentScreen = &SubTrimST;
	TrimStyleST.Color = BASE_BAR_COLOR;
	TrimStyleST.Color2 = BASE_BAR_CENTER_COLOR;
	TrimStyleST.TextColor = BASE_TEXT_COLOR;
	TrimStyleST.BackgroundColor = BASE_BAR_BACK_COLOR;
	TrimStyleST.BorderThickness = BASE_BAR_BORDER_THICKNESS;
	TrimStyleST.BorderColor = BASE_BAR_BORDER_COLOR;
	TrimStyleST.FocusColor = FOCUS_BAR_COLOR;
	TrimStyleST.FocusColor2 = FOCUS_BAR_BACK_COLOR;
	TrimStyleST.FocusBackgroundColor = FOCUS_BAR_BACK_COLOR;
	TrimStyleST.FocusBorderThickness = FOCUS_BAR_BORDER_THICKNESS;
	TrimStyleST.FocusBorderColor = FOCUS_BAR_BORDER_COLOR;


	for (i = 0 ; i < MAX_RC_CHANNEL;i++)
	{

		CHNameTrim[i].Text = __CHLabelShort[i];;
		CHNameTrim[i].Style = &TrimText;
		CHNameTrim[i].Width = 30;
		CHNameTrim[i].Height = ST_TRIM_LINE_HEIGHT;
		CHNameTrim[i].X = CHT_NAME_X;
		CHNameTrim[i].Y = CHT_LINE_Y + CHT_LINE_STEP * i;
		CHNameTrim[i].Border = No;
		CHNameTrim[i].TextAlignment = Right;
		CHNameTrim[i].Dynamic = No;

		CHtrimST[i].Style = &TrimStyleST;
		CHtrimST[i].Width = ST_TRIM_LINE_WIDTH;
		CHtrimST[i].Height = ST_TRIM_LINE_HEIGHT;
		CHtrimST[i].X = CHT_BAR_X;
		CHtrimST[i].Y = CHT_LINE_Y + CHT_LINE_STEP * i;
		CHtrimST[i].HiLimit = 100;
		CHtrimST[i].LoLimit = -100;
		CHtrimST[i].Value = 20;
		CHtrimST[i].Dynamic = Yes;

		CHValueTrim[i].Text = "ST";
		CHValueTrim[i].Style = &TrimText;
		CHValueTrim[i].Width = 30;
		CHValueTrim[i].Height = ST_TRIM_LINE_HEIGHT;
		CHValueTrim[i].X = CHT_VALUE_X;
		CHValueTrim[i].Y = CHT_LINE_Y + CHT_LINE_STEP * i;
		CHValueTrim[i].Border = No;
		CHValueTrim[i].TextAlignment = Left;
		CHValueTrim[i].Dynamic = No;

		STtextBoxInit(&CHNameTrim[i]);
		STtextBoxInit(&CHValueTrim[i]);
		STtrimBarInit(&CHtrimST[i]);
	}


	BackToModelMenuButtonST.Text = __buttonLabelBack;
	BackToModelMenuButtonST.Style = &SubTrimButtonColor;
	BackToModelMenuButtonST.Width = 60;
	BackToModelMenuButtonST.Height = 26;
	BackToModelMenuButtonST.X = MAIN_BACK_BUTTON_X;
	BackToModelMenuButtonST.Y = MAIN_BACK_BUTTON_Y;
	BackToModelMenuButtonST.Border = Yes;

	STbuttonInit(&BackToModelMenuButtonST);
}
