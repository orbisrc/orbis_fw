/*
 * modelmenu2.c
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
#include <gui/fonts/verdana_8pt.h>

#define MODEL_SELECT_BUTTON_Y				50																		// ���������� ������ ������ ������
#define END_POINT_BUTTON_Y					(MODEL_SELECT_BUTTON_Y+MAIN_MENU_BUTTON_HEIGHT+MAIN_MENU_BUTTON_STEP)	// ���������� ������ ������ ������
#define CH_TRIM_BUTTON_Y					(END_POINT_BUTTON_Y+MAIN_MENU_BUTTON_HEIGHT+MAIN_MENU_BUTTON_STEP)		// ���������� ������� ������ ������
#define ST_CURV_BUTTON_Y					(CH_TRIM_BUTTON_Y+MAIN_MENU_BUTTON_HEIGHT+MAIN_MENU_BUTTON_STEP)		// ���������� ��������� ������ ������
#define TH_CURV_BUTTON_Y					(ST_CURV_BUTTON_Y+MAIN_MENU_BUTTON_HEIGHT+MAIN_MENU_BUTTON_STEP)		// ���������� ����� ������ ������
#define CH_REV_BUTTON_Y						(TH_CURV_BUTTON_Y+MAIN_MENU_BUTTON_HEIGHT+MAIN_MENU_BUTTON_STEP)		// ���������� ������ ������ ������
#define DUAL_RATE_BUTTON_Y					(CH_REV_BUTTON_Y+MAIN_MENU_BUTTON_HEIGHT+MAIN_MENU_BUTTON_STEP)			// ���������� ������� ������ ������

STScreenStyleTypeDef ModelMenu2Style = { 0 };
STScreenTypeDef ModelMenu2ST = { 0 };

STColorStyleTypeDef ModelMenu2ButtonColor = { 0 };		// �������� ����� ������
STbuttonTypeDef BackToMainlMenu2Button = { 0 };		//

void _ModelMenu2() {
	if (STscreenShowNow(&ModelMenu2ST)) {
		if (STbuttonPressed(&BackToMainlMenu2Button)) {
			STappSetScreen(MainMenuScr, &STApp);
		}
	}
}

void _ModelMenuInit2() {
	ModelMenu2Style.HeaderFont = &verdana_8pt_bold;
	ModelMenu2Style.BackgroundColor = MAIN_MENU_BACK_COLOR;
	ModelMenu2Style.BorderColor = MAIN_MENU_BORDER_COLOR;
	ModelMenu2Style.BorderThickness = MAIN_MENU_BORDER_THICKNESS;
	ModelMenu2Style.HeaderColor = MAIN_MENU_HEADER_COLOR;
	ModelMenu2Style.HeaderTextColor = MAIN_MENU_HEADER_TEXT_COLOR;

	ModelMenu2ST.App = &STApp;
	ModelMenu2ST.Style = &ModelMenu2Style;
	ModelMenu2ST.ScreenID = ModelMenu2;
	ModelMenu2ST.FullScreen = No;
	ModelMenu2ST.Width = MAIN_MENU_WIDTH;
	ModelMenu2ST.Height = MAIN_MENU_HEIGHT;
	ModelMenu2ST.Header = Yes;
	ModelMenu2ST.HeaderText = "MODEL MENU";
	ModelMenu2ST.Footer = No;

	ModelMenu2ButtonColor.Font = &verdana_8pt_bold;
	ModelMenu2ButtonColor.ParentScreen = &ModelMenu2ST;
	ModelMenu2ButtonColor.Color = BASE_BUTTON_COLOR;
	ModelMenu2ButtonColor.TextColor = BASE_BUTTON_COLOR;
	ModelMenu2ButtonColor.BackgroundColor = BASE_BUTTON_BACK_COLOR;
	ModelMenu2ButtonColor.BorderThickness = BASE_BUTTON_BORDER_THICKNESS;
	ModelMenu2ButtonColor.BorderColor = BASE_BUTTON_BORDER_COLOR;
	ModelMenu2ButtonColor.FocusColor = FOCUS_BUTTON_COLOR;
	ModelMenu2ButtonColor.FocusBackgroundColor = FOCUS_BUTTON_BACK_COLOR;
	ModelMenu2ButtonColor.FocusBorderThickness = FOCUS_BUTTON_BORDER_THICKNESS;
	ModelMenu2ButtonColor.FocusBorderColor = FOCUS_BUTTON_BORDER_COLOR;

	BackToMainlMenu2Button.Text = __buttonLabelBack;
	BackToMainlMenu2Button.Style = &ModelMenu2ButtonColor;
	BackToMainlMenu2Button.Width = 60;
	BackToMainlMenu2Button.Height = 26;
	BackToMainlMenu2Button.X = BACK_BUTTON_X;
	BackToMainlMenu2Button.Y = BACK_BUTTON_Y;
	BackToMainlMenu2Button.Border = Yes;
}
