/*
 * thcalibrate.c
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

STScreenStyleTypeDef THcalibrateStyle = { 0 };
STScreenTypeDef THcalibrateST = { 0 };

STColorStyleTypeDef THcalibrateButtonColor = { 0 };		// �������� ����� ������
STbuttonTypeDef BackButtonTHC = { 0 };
STbuttonTypeDef StepConfirmTHC = { 0 };

STColorStyleTypeDef THcalibrateTextColor = { 0 };
STtextBoxTypeDef THcalibrateStepLabel = { 0 };

void _THcalibrate() {

	static uint16_t CalibrateStep = 0;

	char CalibrateStepLabel[32] = {0};

	if (STscreenShowNow(&THcalibrateST))
	{
		if (STbuttonPressed(&BackButtonTHC))
		{
			STappSetScreen(SystemMenu, &STApp);
		}

		if (STbuttonPressed(&StepConfirmTHC))
		{


			CalibrateStep++;
		}

		/*
		 * ������� ����� � ������� ����� ����������
		 */
		switch (CalibrateStep)
		{
			case 0:
				snprintf(CalibrateStepLabel, 32, "%s", "Full brake?");
				break;

			case 1:
				snprintf(CalibrateStepLabel, 32, "%s", "Full throttle?");
				break;

			case 2:
				snprintf(CalibrateStepLabel, 32, "%s", "Center position?");
				break;

			default:
				snprintf(CalibrateStepLabel, 32, "%s", "Calibration done!");
				break;
		}

		STtextBoxSetValue(CalibrateStepLabel, &THcalibrateStepLabel);

	}

	STscreenShow(&THcalibrateST);

	if (STscreenOnShow(&THcalibrateST))
	{
		CalibrateStep = 0;
	}


	STtextBoxShow(&THcalibrateStepLabel);
	STbuttonShow(&StepConfirmTHC);
	STbuttonShow(&BackButtonTHC);
}
void _THcalibrateInit() {
	/*
	 *  ������������� �������� ������
	 */
	THcalibrateStyle.HeaderFont = &verdana_8pt_bold;
	THcalibrateStyle.BackgroundColor = MAIN_SCREEN_BACK_COLOR;
	THcalibrateStyle.HeaderColor = MAIN_HEADER_COLOR;
	THcalibrateStyle.HeaderTextColor = MAIN_HEADER_TEXT_COLOR;
	THcalibrateStyle.HeaderHeight = MAIN_HEADER_HEIGHT;

	THcalibrateST.App = &STApp;
	THcalibrateST.Style = &THcalibrateStyle;
	THcalibrateST.ScreenID = THcalibrateScreen;
	THcalibrateST.FullScreen = Yes;
	THcalibrateST.Width = GUI_MAX_WIDTH_PX;
	THcalibrateST.Height = GUI_MAX_HEIGHT_PX;
	THcalibrateST.Header = Yes;
	THcalibrateST.HeaderText = "STEERING CALIBRATE";
	THcalibrateST.Footer = No;

	/*
	 *  ����� � ����� ����������
	 */
	THcalibrateButtonColor.Font = &verdana_8pt_bold;
	THcalibrateButtonColor.ParentScreen = &THcalibrateST;
	THcalibrateButtonColor.Color = BASE_BUTTON_COLOR;
	THcalibrateButtonColor.TextColor = BASE_BUTTON_COLOR;
	THcalibrateButtonColor.BackgroundColor = BASE_BUTTON_BACK_COLOR;
	THcalibrateButtonColor.BorderThickness = BASE_BUTTON_BORDER_THICKNESS;
	THcalibrateButtonColor.BorderColor = BASE_BUTTON_BORDER_COLOR;
	THcalibrateButtonColor.FocusColor = FOCUS_BUTTON_COLOR;
	THcalibrateButtonColor.FocusBackgroundColor = FOCUS_BUTTON_BACK_COLOR;
	THcalibrateButtonColor.FocusBorderThickness = FOCUS_BUTTON_BORDER_THICKNESS;
	THcalibrateButtonColor.FocusBorderColor = FOCUS_BUTTON_BORDER_COLOR;

	THcalibrateTextColor.ParentScreen = &THcalibrateST;
	THcalibrateTextColor.Font = &verdana_8pt_bold;
	THcalibrateTextColor.Color = BASE_TEXT_COLOR;
	THcalibrateTextColor.TextColor = BASE_TEXT_COLOR;
	THcalibrateTextColor.BackgroundColor = BASE_TEXT_BACK_COLOR;
	THcalibrateTextColor.BorderThickness = BASE_TEXT_BORDER_THICKNESS;
	THcalibrateTextColor.BorderColor = BASE_TEXT_BORDER_COLOR;

	THcalibrateStepLabel.Text = "Calibrate Step";
	THcalibrateStepLabel.Style = &THcalibrateTextColor;
	THcalibrateStepLabel.Width = 140;
	THcalibrateStepLabel.Height = 26 ;
	THcalibrateStepLabel.X = 50;
	THcalibrateStepLabel.Y = 116;
	THcalibrateStepLabel.Border = No;
	THcalibrateStepLabel.TextAlignment = Centre;
	THcalibrateStepLabel.Dynamic = No;

	StepConfirmTHC.Text = __buttonLabelYes;
	StepConfirmTHC.Style = &THcalibrateButtonColor;
	StepConfirmTHC.Width = 60;
	StepConfirmTHC.Height = 26;
	StepConfirmTHC.X = 90;
	StepConfirmTHC.Y = 146;
	StepConfirmTHC.Border = Yes;

	BackButtonTHC.Text = __buttonLabelBack;
	BackButtonTHC.Style = &THcalibrateButtonColor;
	BackButtonTHC.Width = 60;
	BackButtonTHC.Height = 26;
	BackButtonTHC.X = MAIN_BACK_BUTTON_X;
	BackButtonTHC.Y = MAIN_BACK_BUTTON_Y;
	BackButtonTHC.Border = Yes;

	STtextBoxInit(&THcalibrateStepLabel);
	STbuttonInit(&StepConfirmTHC);
	STbuttonInit(&BackButtonTHC);
}
