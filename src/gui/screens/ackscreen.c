/*
 * ackscreen.c
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

STScreenStyleTypeDef AckScreenStyle = { 0 };
STScreenTypeDef AckScreenST = { 0 };

STColorStyleTypeDef AckScreenButtonColor = { 0 };		// �������� ����� ������
STbuttonTypeDef AckScreenYesButton = { 0 };		//
STbuttonTypeDef AckScreenNoButton = { 0 };		//

void _AckMenu() {
	if (STscreenShowNow(&AckScreenST)) {
		if (STbuttonPressed(&AckScreenYesButton)) {
			STappSetAckState(&STApp);
			STappSetScreen(STappGetPrevScreen(&STApp), &STApp);
		}

		if (STbuttonPressed(&AckScreenNoButton)) {
			STappSetScreen(STappGetPrevScreen(&STApp), &STApp);
		}
	}

	STscreenShow(&AckScreenST);

	STbuttonShow(&AckScreenYesButton);
	STbuttonShow(&AckScreenNoButton);
}

void _AckMenuInit() {
	AckScreenStyle.HeaderFont = &verdana_8pt_bold;
	AckScreenStyle.BackgroundColor = MAIN_MENU_BACK_COLOR;
	AckScreenStyle.BorderColor = MAIN_MENU_BORDER_COLOR;
	AckScreenStyle.BorderThickness = MAIN_MENU_BORDER_THICKNESS;
	AckScreenStyle.HeaderColor = MAIN_MENU_HEADER_COLOR;
	AckScreenStyle.HeaderTextColor = MAIN_MENU_HEADER_TEXT_COLOR;

	AckScreenST.App = &STApp;
	AckScreenST.Style = &AckScreenStyle;
	AckScreenST.ScreenID = AckScreen;
	AckScreenST.FullScreen = No;
	AckScreenST.Width = ACK_SCREEN_WIDTH;
	AckScreenST.Height = ACK_SCREEN_HEIGHT;
	AckScreenST.Header = Yes;
	AckScreenST.HeaderText = "Are you sure?";
	AckScreenST.Footer = No;

	AckScreenButtonColor.Font = &verdana_8pt_bold;
	AckScreenButtonColor.ParentScreen = &AckScreenST;
	AckScreenButtonColor.Color = BASE_BUTTON_COLOR;
	AckScreenButtonColor.TextColor = BASE_BUTTON_COLOR;
	AckScreenButtonColor.BackgroundColor = BASE_BUTTON_BACK_COLOR;
	AckScreenButtonColor.BorderThickness = BASE_BUTTON_BORDER_THICKNESS;
	AckScreenButtonColor.BorderColor = BASE_BUTTON_BORDER_COLOR;
	AckScreenButtonColor.FocusColor = FOCUS_BUTTON_COLOR;
	AckScreenButtonColor.FocusBackgroundColor = FOCUS_BUTTON_BACK_COLOR;
	AckScreenButtonColor.FocusBorderThickness = FOCUS_BUTTON_BORDER_THICKNESS;
	AckScreenButtonColor.FocusBorderColor = FOCUS_BUTTON_BORDER_COLOR;

	AckScreenYesButton.Text = __buttonLabelYes;
	AckScreenYesButton.Style = &AckScreenButtonColor;
	AckScreenYesButton.Width = ACK_BUTTON_WIDTH;
	AckScreenYesButton.Height = ACK_BUTTON_HEIGHT;
	AckScreenYesButton.X = ACK_YES_BUTTON_X;
	AckScreenYesButton.Y = ACK_BUTTON_Y;
	AckScreenYesButton.Border = Yes;

	AckScreenNoButton.Text = __buttonLabelNo;
	AckScreenNoButton.Style = &AckScreenButtonColor;
	AckScreenNoButton.Width = ACK_BUTTON_WIDTH;
	AckScreenNoButton.Height = ACK_BUTTON_HEIGHT;
	AckScreenNoButton.X = ACK_NO_BUTTON_X;
	AckScreenNoButton.Y = ACK_BUTTON_Y;
	AckScreenNoButton.Border = Yes;

	STbuttonInit(&AckScreenYesButton);
	STbuttonInit(&AckScreenNoButton);
}
