/*
 * dualrate.c
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
#include <gui/fonts/verdana_12pt.h>
#include <gui/fonts/verdana_18pt.h>
#include <gui/fonts/verdana_22pt.h>
#include <gui/fonts/verdana_26pt.h>
#include <gui/fonts/verdana_8pt.h>
#include "gui/screens.h"
#include "gui/stgui.h"
#include "gui/stapp.h"

STScreenStyleTypeDef DualRateStyle = { 0 };
STScreenTypeDef DualRateST = { 0 };

STColorStyleTypeDef DualRateButtonColor = { 0 };		// �������� ����� ������
STbuttonTypeDef BackToModelMenuButtonDR = { 0 };

void _DualRate() {
	if (STscreenShowNow(&DualRateST)) {
		if (STbuttonPressed(&BackToModelMenuButtonDR)) {
			STappSetScreen(ModelMenu, &STApp);
		}
	}

	STscreenShow(&DualRateST);

	STbuttonShow(&BackToModelMenuButtonDR);
}
void _DualRateInit() {
	/*
	 *  ������������� �������� ������
	 */
	DualRateStyle.HeaderFont = &verdana_8pt_bold;
	DualRateStyle.BackgroundColor = MAIN_SCREEN_BACK_COLOR;
	DualRateStyle.HeaderColor = MAIN_HEADER_COLOR;
	DualRateStyle.HeaderTextColor = MAIN_HEADER_TEXT_COLOR;
	DualRateStyle.HeaderHeight = MAIN_HEADER_HEIGHT;

	DualRateST.App = &STApp;
	DualRateST.Style = &DualRateStyle;
	DualRateST.ScreenID = DualRate;
	DualRateST.FullScreen = Yes;
	DualRateST.Width = GUI_MAX_WIDTH_PX;
	DualRateST.Height = GUI_MAX_HEIGHT_PX;
	DualRateST.Header = Yes;
	DualRateST.HeaderText = "DUAL RATE";
	DualRateST.Footer = No;

	/*
	 *  ����� � ����� ����������
	 */
	DualRateButtonColor.Font = &verdana_8pt_bold;
	DualRateButtonColor.ParentScreen = &DualRateST;
	DualRateButtonColor.Color = BASE_BUTTON_COLOR;
	DualRateButtonColor.TextColor = BASE_BUTTON_COLOR;
	DualRateButtonColor.BackgroundColor = BASE_BUTTON_BACK_COLOR;
	DualRateButtonColor.BorderThickness = BASE_BUTTON_BORDER_THICKNESS;
	DualRateButtonColor.BorderColor = BASE_BUTTON_BORDER_COLOR;
	DualRateButtonColor.FocusColor = FOCUS_BUTTON_COLOR;
	DualRateButtonColor.FocusBackgroundColor = FOCUS_BUTTON_BACK_COLOR;
	DualRateButtonColor.FocusBorderThickness = FOCUS_BUTTON_BORDER_THICKNESS;
	DualRateButtonColor.FocusBorderColor = FOCUS_BUTTON_BORDER_COLOR;

	BackToModelMenuButtonDR.Text = __buttonLabelBack;
	BackToModelMenuButtonDR.Style = &DualRateButtonColor;
	BackToModelMenuButtonDR.Width = 60;
	BackToModelMenuButtonDR.Height = 26;
	BackToModelMenuButtonDR.X = MAIN_BACK_BUTTON_X;
	BackToModelMenuButtonDR.Y = MAIN_BACK_BUTTON_Y;
	BackToModelMenuButtonDR.Border = Yes;

	STbuttonInit(&BackToModelMenuButtonDR);
}
