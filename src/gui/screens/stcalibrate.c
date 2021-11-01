/*
 * stcalibrate.c
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
#include "core/analog.h"

STScreenStyleTypeDef STcalibrateStyle = { 0 };
STScreenTypeDef STcalibrateST = { 0 };

STColorStyleTypeDef STcalibrateButtonColor = { 0 };
STbuttonTypeDef BackButtonSTC = { 0 };
STbuttonTypeDef StepConfirmSTC = { 0 };

STColorStyleTypeDef STcalibrateTextColor = { 0 };
STtextBoxTypeDef STcalibrateStepLabel = { 0 };

void _STcalibrate() {

	static uint16_t CalibrateStep = 0;
	char CalibrateStepLabel[32] = {0};

	static uint16_t Amin;
	static uint16_t Amax;
	static uint16_t Acentral;
	static uint16_t Emin;
	static uint16_t Emax;
	static uint16_t Ecentral;
	static uint16_t Tmin;
	static uint16_t Tmax;
	static uint16_t Tcentral;
	static uint16_t Rmin;
	static uint16_t Rmax;
	static uint16_t Rcentral;

	if (STscreenShowNow(&STcalibrateST))
	{
		if (STbuttonPressed(&BackButtonSTC))
		{
			STappSetScreen(SystemMenu, &STApp);
		}

		if (STbuttonPressed(&StepConfirmSTC))
		{
			switch (CalibrateStep)
			{
				case 0:
//					RCChanelSetADCCentr(Acentral,&RCChanel[0]);
//					RCChanelSetADCCentr(Ecentral,&RCChanel[1]);
//					RCChanelSetADCCentr(Tcentral,&RCChanel[2]);
//					RCChanelSetADCCentr(Rcentral,&RCChanel[3]);

					AIsetADCMin(Amin, &AnalogChannel[0]);
					AIsetADCMax(Amax, &AnalogChannel[0]);

					AIsetADCMin(Emin, &AnalogChannel[1]);
					AIsetADCMax(Emax, &AnalogChannel[1]);

					AIsetADCMin(Tmin, &AnalogChannel[2]);
					AIsetADCMax(Tmax, &AnalogChannel[2]);

					AIsetADCMin(Rmin, &AnalogChannel[3]);
					AIsetADCMax(Rmax, &AnalogChannel[3]);

					break;

				case 1:
//					RCChanelSetADCMin(Amin, &RCChanel[0]);
//					RCChanelSetADCMax(Amax, &RCChanel[0]);
//
//					RCChanelSetADCMin(Emin, &RCChanel[1]);
//					RCChanelSetADCMax(Emax, &RCChanel[1]);
//
//					RCChanelSetADCMin(Tmin, &RCChanel[2]);
//					RCChanelSetADCMax(Tmax, &RCChanel[2]);
//
//					RCChanelSetADCMin(Rmin, &RCChanel[3]);
//					RCChanelSetADCMax(Rmax, &RCChanel[3]);


//					RCChanelSetADCCentr(Acentral,&RCChanel[0]);
//					RCChanelSetADCCentr(Ecentral,&RCChanel[1]);
//					RCChanelSetADCCentr(Tcentral,&RCChanel[2]);
//					RCChanelSetADCCentr(Rcentral,&RCChanel[3]);

					AIsetADCCenter(Acentral,&AnalogChannel[0]);
					AIsetADCCenter(Ecentral,&AnalogChannel[1]);
					AIsetADCCenter(Tcentral,&AnalogChannel[2]);
					AIsetADCCenter(Rcentral,&AnalogChannel[3]);


					break;

				default:

					break;
			}
			CalibrateStep++;

		}

		switch (CalibrateStep)
		{
			case 0:
				snprintf(CalibrateStepLabel, 32, "%s", "Set MIN and MAX");

//				Acentral = AIgetValue(&AnalogChannel[0]);
//				Ecentral = AIgetValue(&AnalogChannel[1]);
//				Tcentral = AIgetValue(&AnalogChannel[2]);
//				Rcentral = AIgetValue(&AnalogChannel[3]);


				if (Amin > AIgetADCValue(&AnalogChannel[0])) Amin = AIgetADCValue(&AnalogChannel[0]);
				if (Amax < AIgetADCValue(&AnalogChannel[0])) Amax = AIgetADCValue(&AnalogChannel[0]);

				if (Emin > AIgetADCValue(&AnalogChannel[1])) Emin = AIgetADCValue(&AnalogChannel[1]);
				if (Emax < AIgetADCValue(&AnalogChannel[1])) Emax = AIgetADCValue(&AnalogChannel[1]);

				if (Tmin > AIgetADCValue(&AnalogChannel[2])) Tmin = AIgetADCValue(&AnalogChannel[2]);
				if (Tmax < AIgetADCValue(&AnalogChannel[2])) Tmax = AIgetADCValue(&AnalogChannel[2]);

				if (Rmin > AIgetADCValue(&AnalogChannel[3])) Rmin = AIgetADCValue(&AnalogChannel[3]);
				if (Rmax < AIgetADCValue(&AnalogChannel[3])) Rmax = AIgetADCValue(&AnalogChannel[3]);

				break;

			case 1:
				snprintf(CalibrateStepLabel, 32, "%s", "Central position?");

//					if (Amin > AIgetValue(&AnalogChannel[0])) Amin = AIgetValue(&AnalogChannel[0]);
//					if (Amax < AIgetValue(&AnalogChannel[0])) Amax = AIgetValue(&AnalogChannel[0]);
//
//					if (Emin > AIgetValue(&AnalogChannel[1])) Emin = AIgetValue(&AnalogChannel[1]);
//					if (Emax < AIgetValue(&AnalogChannel[1])) Emax = AIgetValue(&AnalogChannel[1]);
//
//					if (Tmin > AIgetValue(&AnalogChannel[2])) Tmin = AIgetValue(&AnalogChannel[2]);
//					if (Tmax < AIgetValue(&AnalogChannel[2])) Tmax = AIgetValue(&AnalogChannel[2]);
//
//					if (Rmin > AIgetValue(&AnalogChannel[3])) Rmin = AIgetValue(&AnalogChannel[3]);
//					if (Rmax < AIgetValue(&AnalogChannel[3])) Rmax = AIgetValue(&AnalogChannel[3]);


//				Acentral = AIgetValue(&AnalogChannel[0]);
//				Ecentral = AIgetValue(&AnalogChannel[1]);
//				Tcentral = AIgetValue(&AnalogChannel[2]);
//				Rcentral = AIgetValue(&AnalogChannel[3]);


				//

				Acentral = AIgetADCValue(&AnalogChannel[0]);
				Ecentral = AIgetADCValue(&AnalogChannel[1]);
				Tcentral = AIgetADCValue(&AnalogChannel[2]);
				Rcentral = AIgetADCValue(&AnalogChannel[3]);


					break;

			case 2:

				snprintf(CalibrateStepLabel, 32, "%s", "Calibrate complete");

			break;

			default:

					STappSetScreen(SystemMenu, &STApp);

				break;

		}

		STtextBoxSetValue(CalibrateStepLabel, &STcalibrateStepLabel);

	}

	STscreenShow(&STcalibrateST);

	if (STscreenOnShow(&STcalibrateST))
	{
		CalibrateStep = 0;

		Amin = ADC_MAX;
		Amax = ADC_MIN;
		Emin = ADC_MAX;
		Emax = ADC_MIN;
		Tmin = ADC_MAX;
		Tmax = ADC_MIN;
		Rmin = ADC_MAX;
		Rmax = ADC_MIN;
	}


	STtextBoxShow(&STcalibrateStepLabel);
	STbuttonShow(&StepConfirmSTC);
	STbuttonShow(&BackButtonSTC);
}
void _STcalibrateInit() {
	/*
	 *  ������������� �������� ������
	 */
	STcalibrateStyle.HeaderFont = &verdana_8pt_bold;
	STcalibrateStyle.BackgroundColor = MAIN_SCREEN_BACK_COLOR;
	STcalibrateStyle.HeaderColor = MAIN_HEADER_COLOR;
	STcalibrateStyle.HeaderTextColor = MAIN_HEADER_TEXT_COLOR;
	STcalibrateStyle.HeaderHeight = MAIN_HEADER_HEIGHT;

	STcalibrateST.App = &STApp;
	STcalibrateST.Style = &STcalibrateStyle;
	STcalibrateST.ScreenID = STcalibrateScreen;
	STcalibrateST.FullScreen = Yes;
	STcalibrateST.Width = GUI_MAX_WIDTH_PX;
	STcalibrateST.Height = GUI_MAX_HEIGHT_PX;
	STcalibrateST.Header = Yes;
	STcalibrateST.HeaderText = "STICK CALIBRATE";
	STcalibrateST.Footer = No;

	/*
	 *  ����� � ����� ����������
	 */
	STcalibrateButtonColor.Font = &verdana_8pt_bold;
	STcalibrateButtonColor.ParentScreen = &STcalibrateST;
	STcalibrateButtonColor.Color = BASE_BUTTON_COLOR;
	STcalibrateButtonColor.TextColor = BASE_BUTTON_COLOR;
	STcalibrateButtonColor.BackgroundColor = BASE_BUTTON_BACK_COLOR;
	STcalibrateButtonColor.BorderThickness = BASE_BUTTON_BORDER_THICKNESS;
	STcalibrateButtonColor.BorderColor = BASE_BUTTON_BORDER_COLOR;
	STcalibrateButtonColor.FocusColor = FOCUS_BUTTON_COLOR;
	STcalibrateButtonColor.FocusBackgroundColor = FOCUS_BUTTON_BACK_COLOR;
	STcalibrateButtonColor.FocusBorderThickness = FOCUS_BUTTON_BORDER_THICKNESS;
	STcalibrateButtonColor.FocusBorderColor = FOCUS_BUTTON_BORDER_COLOR;

	STcalibrateTextColor.ParentScreen = &STcalibrateST;
	STcalibrateTextColor.Font = &verdana_8pt_bold;
	STcalibrateTextColor.Color = BASE_TEXT_COLOR;
	STcalibrateTextColor.TextColor = BASE_TEXT_COLOR;
	STcalibrateTextColor.BackgroundColor = BASE_TEXT_BACK_COLOR;
	STcalibrateTextColor.BorderThickness = BASE_TEXT_BORDER_THICKNESS;
	STcalibrateTextColor.BorderColor = BASE_TEXT_BORDER_COLOR;

	STcalibrateStepLabel.Text = "Calibrate complete?";
	STcalibrateStepLabel.Style = &STcalibrateTextColor;
	STcalibrateStepLabel.Width = 140;
	STcalibrateStepLabel.Height = 26 ;
	STcalibrateStepLabel.X = 50;
	STcalibrateStepLabel.Y = 116;
	STcalibrateStepLabel.Border = No;
	STcalibrateStepLabel.TextAlignment = Centre;
	STcalibrateStepLabel.Dynamic = No;

	StepConfirmSTC.Text = __buttonLabelYes;
	StepConfirmSTC.Style = &STcalibrateButtonColor;
	StepConfirmSTC.Width = 60;
	StepConfirmSTC.Height = 26;
	StepConfirmSTC.X = 90;
	StepConfirmSTC.Y = 146;
	StepConfirmSTC.Border = Yes;

	BackButtonSTC.Text = __buttonLabelBack;
	BackButtonSTC.Style = &STcalibrateButtonColor;
	BackButtonSTC.Width = 60;
	BackButtonSTC.Height = 26;
	BackButtonSTC.X = MAIN_BACK_BUTTON_X;
	BackButtonSTC.Y = MAIN_BACK_BUTTON_Y;
	BackButtonSTC.Border = Yes;

	STtextBoxInit(&STcalibrateStepLabel);
	STbuttonInit(&StepConfirmSTC);
	STbuttonInit(&BackButtonSTC);
}
