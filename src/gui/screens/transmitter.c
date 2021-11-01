/*
 * transmitter.c
 *
 *	Copyright 2019 Denis Davydov
 *
 *	Licensed under the Apache License, Version 2.0 (the "License");
 *	you may not use this file excTXt in compliance with the License.
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
#define CHET_LINE_STTX				18

STScreenStyleTypeDef TransmitterStyle = { 0 };
STScreenTypeDef TransmitterST = { 0 };
STColorStyleTypeDef TransmitterButtonColor = { 0 };		// �������� ����� ������
STColorStyleTypeDef TransmitterInputBoxColor = { 0 };// �������� ����� ����� �����
STColorStyleTypeDef TransmitterText = { 0 };
STbuttonTypeDef BackToModelMenuButtonTX = { 0 };
STinputBoxTypeDef SettingsPPMTransmitterST[MAX_RC_CHANNEL] = { 0 };
STtextBoxTypeDef SettingsNameTransmitter[MAX_RC_CHANNEL] = { 0 };

void _Transmitter()
{
	uint16_t i = 0;

	if (STscreenShowNow(&TransmitterST)) {
		if (STbuttonPressed(&BackToModelMenuButtonTX)) {
			STappSetScreen(ModelMenu, &STApp);
		}

		for(i = 0; i < MAX_RC_CHANNEL; i++)
		{
			if (STinputBoxValueChange(&SettingsPPMTransmitterST[i]))
			{
	//			RCChanelSetHightRate((uint16_t) STinputBoxGetValue(&SettingsPPMTransmitterST[i]), &RCChanel[i]);
			}
		}
	}

	STscreenShow(&TransmitterST);

	if (STscreenOnShow(&TransmitterST))
	{
		for(i = 0; i < MAX_RC_CHANNEL; i++)
		{
	//		STinputBoxSetValue(RCChanelGetHighRate(&RCChanel[i]), &SettingsPPMTransmitterST[i]);
		}
	}


	for(i = 0; i < MAX_RC_CHANNEL; i++)
	{
		STtextBoxShow(&SettingsNameTransmitter[i]);
		STinputBoxShow(&SettingsPPMTransmitterST[i]);
	}

	STbuttonShow(&BackToModelMenuButtonTX);
}
void _TransmitterInit()
{
	uint8_t i = 0;

	TransmitterStyle.HeaderFont = &verdana_8pt_bold;
	TransmitterStyle.BackgroundColor = MAIN_SCREEN_BACK_COLOR;
	TransmitterStyle.HeaderColor = MAIN_HEADER_COLOR;
	TransmitterStyle.HeaderTextColor = MAIN_HEADER_TEXT_COLOR;
	TransmitterStyle.HeaderHeight = MAIN_HEADER_HEIGHT;

	TransmitterST.App = &STApp;
	TransmitterST.Style = &TransmitterStyle;
	TransmitterST.ScreenID = Transmitter;
	TransmitterST.FullScreen = Yes;
	TransmitterST.Width = GUI_MAX_WIDTH_PX;
	TransmitterST.Height = GUI_MAX_HEIGHT_PX;
	TransmitterST.Header = Yes;
	TransmitterST.HeaderText = "TRANSMITTER";
	TransmitterST.Footer = No;

	/*
	 *  ����� � ����� ����������
	 */

	TransmitterButtonColor.Font = &verdana_8pt_bold;
	TransmitterButtonColor.ParentScreen = &TransmitterST;
	TransmitterButtonColor.Color = BASE_BUTTON_COLOR;
	TransmitterButtonColor.TextColor = BASE_BUTTON_COLOR;
	TransmitterButtonColor.BackgroundColor = BASE_BUTTON_BACK_COLOR;
	TransmitterButtonColor.BorderThickness = BASE_BUTTON_BORDER_THICKNESS;
	TransmitterButtonColor.BorderColor = BASE_BUTTON_BORDER_COLOR;
	TransmitterButtonColor.FocusColor = FOCUS_BUTTON_COLOR;
	TransmitterButtonColor.FocusBackgroundColor = FOCUS_BUTTON_BACK_COLOR;
	TransmitterButtonColor.FocusBorderThickness = FOCUS_BUTTON_BORDER_THICKNESS;
	TransmitterButtonColor.FocusBorderColor = FOCUS_BUTTON_BORDER_COLOR;

	TransmitterInputBoxColor.ParentScreen = &TransmitterST;
	TransmitterInputBoxColor.Font = &verdana_8pt_bold;
	TransmitterInputBoxColor.Color = BASE_INPUT_COLOR;
	TransmitterInputBoxColor.TextColor = BASE_INPUT_TEXT_COLOR;
	TransmitterInputBoxColor.BackgroundColor = BASE_INPUT_BACK_COLOR;
	TransmitterInputBoxColor.BorderThickness = BASE_INPUT_BORDER_THICKNESS;
	TransmitterInputBoxColor.BorderColor = BASE_INPUT_BORDER_COLOR;
	TransmitterInputBoxColor.FocusTextColor = FOCUS_INPUT_TEXT_COLOR;
	TransmitterInputBoxColor.FocusColor = FOCUS_INPUT_COLOR;
	TransmitterInputBoxColor.FocusBackgroundColor = FOCUS_INPUT_BACK_COLOR;
	TransmitterInputBoxColor.FocusBorderThickness = FOCUS_INPUT_BORDER_THICKNESS;
	TransmitterInputBoxColor.FocusBorderColor = FOCUS_INPUT_BORDER_COLOR;

	TransmitterText.ParentScreen = &TransmitterST;
	TransmitterText.Font = &verdana_8pt_bold;
	TransmitterText.Color = BASE_TEXT_COLOR;
	TransmitterText.TextColor = BASE_TEXT_COLOR;
	TransmitterText.BackgroundColor = BASE_TEXT_BACK_COLOR;
	TransmitterText.BorderThickness = BASE_TEXT_BORDER_THICKNESS;
	TransmitterText.BorderColor = BASE_TEXT_BORDER_COLOR;



	for(i = 0; i < MAX_RC_CHANNEL; i++)
	{
		SettingsNameTransmitter[i].Text = __CHLabel[i];
		SettingsNameTransmitter[i].Style = &TransmitterText;
		SettingsNameTransmitter[i].Width = 30;
		SettingsNameTransmitter[i].Height = INPUT_BASE_HEIGHT;
		SettingsNameTransmitter[i].X = CHET_NAME_X;
		SettingsNameTransmitter[i].Y = CHET_LINE_Y + CHET_LINE_STTX * i;
		SettingsNameTransmitter[i].Border = No;
		SettingsNameTransmitter[i].TextAlignment = Right;
		SettingsNameTransmitter[i].Dynamic = No;

		SettingsPPMTransmitterST[i].Style = &TransmitterInputBoxColor;
		SettingsPPMTransmitterST[i].Width = INPUT_BASE_WIDTH;
		SettingsPPMTransmitterST[i].Height = INPUT_BASE_HEIGHT;
		SettingsPPMTransmitterST[i].X = CHET_HIGHTRATE_X;
		SettingsPPMTransmitterST[i].Y = CHET_LINE_Y + CHET_LINE_STTX * i;
		SettingsPPMTransmitterST[i].HiLimit = 100;
		SettingsPPMTransmitterST[i].LoLimit = 0;
		SettingsPPMTransmitterST[i].TextAlignment = Centre;
		SettingsPPMTransmitterST[i].Border = Yes;
		SettingsPPMTransmitterST[i].Dynamic = Yes;
		SettingsPPMTransmitterST[i].Units = "%";
		SettingsPPMTransmitterST[i].UnitsShow = Yes;

		STtextBoxInit(&SettingsNameTransmitter[i]);
		STinputBoxInit(&SettingsPPMTransmitterST[i]);
	}


	BackToModelMenuButtonTX.Text = __buttonLabelBack;
	BackToModelMenuButtonTX.Style = &TransmitterButtonColor;
	BackToModelMenuButtonTX.Width = 60;
	BackToModelMenuButtonTX.Height = 26;
	BackToModelMenuButtonTX.X = MAIN_BACK_BUTTON_X;
	BackToModelMenuButtonTX.Y = MAIN_BACK_BUTTON_Y;
	BackToModelMenuButtonTX.Border = Yes;

	STbuttonInit(&BackToModelMenuButtonTX);
}
