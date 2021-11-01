/*
 * servoview.c
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
#include <core/rcchannel.h>
#include <gui/fonts/verdana_12pt.h>
#include <gui/fonts/verdana_18pt.h>
#include <gui/fonts/verdana_22pt.h>
#include <gui/fonts/verdana_26pt.h>
#include <gui/fonts/verdana_8pt.h>

#define CHSV_NAME_X				4
#define CHSV_VALUE_X			206
#define SV_BAR_X				45

#define CHSV_LINE_Y				25
#define CHSV_LINE_STEP			16
#define ST_SERVO_LINE_WIDTH		150
#define ST_SERVO_LINE_HEIGHT	12

STScreenStyleTypeDef ServoViewStyle = { 0 };
STScreenTypeDef ServoViewST = { 0 };
STColorStyleTypeDef BarStyleServoViewST = { 0 };
STColorStyleTypeDef ServoViewText = { 0 };

STColorStyleTypeDef ServoViewButtonColor = { 0 };
STbuttonTypeDef BackToModelMenuButtonSV = { 0 };

STtextBoxTypeDef CHNameServoView[MAX_RC_CHANNEL] = { 0 };
STtextBoxTypeDef CHValueServoView[MAX_RC_CHANNEL]  = { 0 };
STprogressBarTypeDef CHbarSV[MAX_RC_CHANNEL] = { 0 };


void _ServoView() {
	char CHT[MAX_RC_CHANNEL][8] = { 0 };

	uint16_t i = 0;

	for(i = 0;i < MAX_RC_CHANNEL;i++)
	{
		snprintf(CHT[i], 8, "%d", RCChanelGetValue(&RCChanel[i])); //
		STtextBoxSetValue(CHT[i], & CHValueServoView[i]);
	}

	if (STscreenShowNow(&ServoViewST)) {
		if (STbuttonPressed(&BackToModelMenuButtonSV)) {
		//	STappSetScreen(MainMenuScr, &STApp);

			STappSetScreen(STappGetPrevScreen(&STApp),&STApp);
		}

		for(i = 0;i < MAX_RC_CHANNEL;i++)
		{
			STprogressBarSetValue(RCChanelGetValue(&RCChanel[i]), &CHbarSV[i]);
		}
	}

	STscreenShow(&ServoViewST);

	for(i = 0;i < MAX_RC_CHANNEL;i++)
	{
		STtextBoxShow(&CHNameServoView[i]);
		STtextBoxShow(&CHValueServoView[i]);
		STprogressBarShow(&CHbarSV[i]);
	}

	STbuttonShow(&BackToModelMenuButtonSV);
}
void _ServoViewInit() {


	uint16_t i = 0;



	/*
	 *  ������������� �������� ������
	 */
	ServoViewStyle.HeaderFont = &verdana_8pt_bold;
	ServoViewStyle.BackgroundColor = MAIN_SCREEN_BACK_COLOR;
	ServoViewStyle.HeaderColor = MAIN_HEADER_COLOR;
	ServoViewStyle.HeaderTextColor = MAIN_HEADER_TEXT_COLOR;
	ServoViewStyle.HeaderHeight = MAIN_HEADER_HEIGHT;

	ServoViewST.App = &STApp;
	ServoViewST.Style = &ServoViewStyle;
	ServoViewST.ScreenID = ServoView;
	ServoViewST.FullScreen = Yes;
	ServoViewST.Width = GUI_MAX_WIDTH_PX;
	ServoViewST.Height = GUI_MAX_HEIGHT_PX;
	ServoViewST.Header = Yes;
	ServoViewST.HeaderText = "SERVO VIEW";
	ServoViewST.Footer = No;

	/*
	 *  ����� � ����� ����������
	 */
	ServoViewButtonColor.Font = &verdana_8pt_bold;
	ServoViewButtonColor.ParentScreen = &ServoViewST;
	ServoViewButtonColor.Color = BASE_BUTTON_COLOR;
	ServoViewButtonColor.TextColor = BASE_BUTTON_COLOR;
	ServoViewButtonColor.BackgroundColor = BASE_BUTTON_BACK_COLOR;
	ServoViewButtonColor.BorderThickness = BASE_BUTTON_BORDER_THICKNESS;
	ServoViewButtonColor.BorderColor = BASE_BUTTON_BORDER_COLOR;
	ServoViewButtonColor.FocusColor = FOCUS_BUTTON_COLOR;
	ServoViewButtonColor.FocusBackgroundColor = FOCUS_BUTTON_BACK_COLOR;
	ServoViewButtonColor.FocusBorderThickness = FOCUS_BUTTON_BORDER_THICKNESS;
	ServoViewButtonColor.FocusBorderColor = FOCUS_BUTTON_BORDER_COLOR;

	BarStyleServoViewST.ParentScreen = &ServoViewST;
	BarStyleServoViewST.Color = BASE_BAR_COLOR;
	BarStyleServoViewST.Color2 = BASE_BAR_CENTER_COLOR;
	BarStyleServoViewST.TextColor = BASE_TEXT_COLOR;
	BarStyleServoViewST.BackgroundColor = BASE_BAR_BACK_COLOR;
	BarStyleServoViewST.BorderThickness = BASE_BAR_BORDER_THICKNESS;
	BarStyleServoViewST.BorderColor = BASE_BAR_BORDER_COLOR;
	BarStyleServoViewST.Color2 = FOCUS_BAR_CENTER_COLOR;
	BarStyleServoViewST.FocusColor = FOCUS_BAR_COLOR;
	BarStyleServoViewST.FocusBackgroundColor = FOCUS_BAR_BACK_COLOR;
	BarStyleServoViewST.FocusBorderThickness = FOCUS_BAR_BORDER_THICKNESS;
	BarStyleServoViewST.FocusBorderColor = FOCUS_BAR_BORDER_COLOR;

	ServoViewText.ParentScreen = &ServoViewST;
	ServoViewText.Font = &verdana_8pt_bold;
	ServoViewText.Color = BASE_TEXT_COLOR;
	ServoViewText.TextColor = BASE_TEXT_COLOR;
	ServoViewText.BackgroundColor = BASE_TEXT_BACK_COLOR;
	ServoViewText.BorderThickness = BASE_TEXT_BORDER_THICKNESS;
	ServoViewText.BorderColor = BASE_TEXT_BORDER_COLOR;


	for(i = 0;i < MAX_RC_CHANNEL;i++)
	{
		CHNameServoView[i].Text = __CHLabelShort[i];
		CHNameServoView[i].Style = &ServoViewText;
		CHNameServoView[i].Width = 30;
		CHNameServoView[i].Height = ST_SERVO_LINE_HEIGHT;
		CHNameServoView[i].X = CHSV_NAME_X;
		CHNameServoView[i].Y = CHSV_LINE_Y + CHSV_LINE_STEP*i;
		CHNameServoView[i].Border = No;
		CHNameServoView[i].TextAlignment = Right;
		CHNameServoView[i].Dynamic = No;

		CHValueServoView[i].Text = "CH";
		CHValueServoView[i].Style = &ServoViewText;
		CHValueServoView[i].Width = 30;
		CHValueServoView[i].Height = ST_SERVO_LINE_HEIGHT;
		CHValueServoView[i].X = CHSV_VALUE_X;
		CHValueServoView[i].Y = CHSV_LINE_Y + CHSV_LINE_STEP*i;
		CHValueServoView[i].Border = No;
		CHValueServoView[i].TextAlignment = Left;
		CHValueServoView[i].Dynamic = No;

		CHbarSV[i].Style = &BarStyleServoViewST;
		CHbarSV[i].Width = ST_SERVO_LINE_WIDTH;
		CHbarSV[i].Height = ST_SERVO_LINE_HEIGHT;
		CHbarSV[i].X = SV_BAR_X;
		CHbarSV[i].Y = CHSV_LINE_Y + CHSV_LINE_STEP*i;;
		CHbarSV[i].HiLimit = GUI_CH_MAX;
		CHbarSV[i].LoLimit = GUI_CH_MIN;

		STtextBoxInit(&CHNameServoView[i]);
		STtextBoxInit(&CHValueServoView[i]);
		STprogressBarInit(&CHbarSV[i]);
	}

	BackToModelMenuButtonSV.Text = __buttonLabelBack;
	BackToModelMenuButtonSV.Style = &ServoViewButtonColor;
	BackToModelMenuButtonSV.Width = 60;
	BackToModelMenuButtonSV.Height = 26;
	BackToModelMenuButtonSV.X = MAIN_BACK_BUTTON_X;
	BackToModelMenuButtonSV.Y = MAIN_BACK_BUTTON_Y;
	BackToModelMenuButtonSV.Border = Yes;

	STbuttonInit(&BackToModelMenuButtonSV);
}
