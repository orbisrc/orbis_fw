/*
 * filtersettings.c
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

#define CH_FS_NAME_X				4
#define CH_FS_CHECKBOX_X			40
#define CH_FS_X						95
#define CH_DZ_X						160

#define INPUT_FSETS_INPUT_WIDTH		60

#define ST_FS_LINE_Y				40
#define TH_FS_LINE_Y				70
#define CH1_FS_LINE_Y				100
#define CH2_FS_LINE_Y				130
#define CH3_FS_LINE_Y				160
#define CH4_FS_LINE_Y				190
#define CH5_FS_LINE_Y				220
#define CH6_FS_LINE_Y				250

STScreenStyleTypeDef FSStyle = { 0 };
STScreenTypeDef FilterSettingsST = { 0 };

STColorStyleTypeDef FSButtonColor = { 0 };			// �������� ����� ������
STbuttonTypeDef BackToModelMenuButtonFS = { 0 };

STColorStyleTypeDef CHfilterSetCheckBoxColor = { 0 };
STcheckBoxTypeDef STfilterSetCheckBox = { 0 };
STcheckBoxTypeDef THfilterSetCheckBox = { 0 };
STcheckBoxTypeDef CH1filterSetCheckBox = { 0 };
STcheckBoxTypeDef CH2filterSetCheckBox = { 0 };
STcheckBoxTypeDef CH3filterSetCheckBox = { 0 };
STcheckBoxTypeDef CH4filterSetCheckBox = { 0 };
STcheckBoxTypeDef CH5filterSetCheckBox = { 0 };
STcheckBoxTypeDef CH6filterSetCheckBox = { 0 };

STColorStyleTypeDef FilterSetText = { 0 };
STtextBoxTypeDef STNameFilterSet = { 0 };
STtextBoxTypeDef THNameFilterSet = { 0 };
STtextBoxTypeDef CH1NameFilterSet = { 0 };
STtextBoxTypeDef CH2NameFilterSet = { 0 };
STtextBoxTypeDef CH3NameFilterSet = { 0 };
STtextBoxTypeDef CH4NameFilterSet = { 0 };
STtextBoxTypeDef CH5NameFilterSet = { 0 };
STtextBoxTypeDef CH6NameFilterSet = { 0 };

STColorStyleTypeDef FilterKInputBoxColor = { 0 };
STinputBoxTypeDef STFilterK = { 0 };
STinputBoxTypeDef THFilterK = { 0 };
STinputBoxTypeDef CH1FilterK = { 0 };
STinputBoxTypeDef CH2FilterK = { 0 };
STinputBoxTypeDef CH3FilterK = { 0 };
STinputBoxTypeDef CH4FilterK = { 0 };
STinputBoxTypeDef CH5FilterK = { 0 };
STinputBoxTypeDef CH6FilterK = { 0 };

STinputBoxTypeDef STDeadZone = { 0 };
STinputBoxTypeDef THDeadZone = { 0 };
STinputBoxTypeDef CH1DeadZone = { 0 };
STinputBoxTypeDef CH2DeadZone = { 0 };
STinputBoxTypeDef CH3DeadZone = { 0 };
STinputBoxTypeDef CH4DeadZone = { 0 };
STinputBoxTypeDef CH5DeadZone = { 0 };
STinputBoxTypeDef CH6DeadZone = { 0 };

void _FilterSettings() {
//	if (STscreenShowNow(&FilterSettingsST)) {
//		if (STbuttonPressed(&BackToModelMenuButtonFS)) {
//			STappSetScreen(SystemMenu, &STApp);
//			STsaveSettings(&ModelSettings[0]);
//			STcopySettings(&ModelSettings[ModelSettings[0].ID],
//					&ModelSettings[0]);
//		}
//
//		/*
//		 * ���������/���������� ����������
//		 */
//		if (STcheckBoxValueChange(&STfilterSetCheckBox)) {
//			RCChanelFilterSetState(STcheckBoxGetState(&STfilterSetCheckBox),
//					&RCChanel_0);
//		}
//
//		if (STcheckBoxValueChange(&THfilterSetCheckBox)) {
//			RCChanelFilterSetState(STcheckBoxGetState(&THfilterSetCheckBox),
//					&RCChanel_1);
//		}
//
//		if (STcheckBoxValueChange(&CH1filterSetCheckBox)) {
//			RCChanelFilterSetState(STcheckBoxGetState(&CH1filterSetCheckBox),
//					&RCChanel_2);
//		}
//
//		if (STcheckBoxValueChange(&CH2filterSetCheckBox)) {
//			RCChanelFilterSetState(STcheckBoxGetState(&CH2filterSetCheckBox),
//					&RCChanel_3);
//		}
//
//		if (STcheckBoxValueChange(&CH3filterSetCheckBox)) {
//			RCChanelFilterSetState(STcheckBoxGetState(&CH3filterSetCheckBox),
//					&RCChanel_4);
//		}
//
//		if (STcheckBoxValueChange(&CH4filterSetCheckBox)) {
//			RCChanelFilterSetState(STcheckBoxGetState(&CH4filterSetCheckBox),
//					&RCChanel_5);
//		}
//
//		if (STcheckBoxValueChange(&CH5filterSetCheckBox)) {
//			RCChanelFilterSetState(STcheckBoxGetState(&CH5filterSetCheckBox),
//					&RCChanel_6);
//		}
//
//		if (STcheckBoxValueChange(&CH6filterSetCheckBox)) {
//			RCChanelFilterSetState(STcheckBoxGetState(&CH6filterSetCheckBox),
//					&RCChanel_7);
//		}
//
//		/*
//		 * ��������� ���� ����������
//		 */
//		if (STinputBoxValueChange(&STFilterK)) {
//			RCChanelFilterSetN((uint16_t) STinputBoxGetValue(&STFilterK),
//					&RCChanel_0);
//		}
//
//		if (STinputBoxValueChange(&THFilterK)) {
//			RCChanelFilterSetN((uint16_t) STinputBoxGetValue(&THFilterK),
//					&RCChanel_1);
//		}
//
//		if (STinputBoxValueChange(&CH1FilterK)) {
//			RCChanelFilterSetN((uint16_t) STinputBoxGetValue(&CH1FilterK),
//					&RCChanel_2);
//		}
//
//		if (STinputBoxValueChange(&CH2FilterK)) {
//			RCChanelFilterSetN((uint16_t) STinputBoxGetValue(&CH2FilterK),
//					&RCChanel_3);
//		}
//
//		if (STinputBoxValueChange(&CH3FilterK)) {
//			RCChanelFilterSetN((uint16_t) STinputBoxGetValue(&CH3FilterK),
//					&RCChanel_4);
//		}
//
//		if (STinputBoxValueChange(&CH4FilterK)) {
//			RCChanelFilterSetN((uint16_t) STinputBoxGetValue(&CH4FilterK),
//					&RCChanel_5);
//		}
//
//		if (STinputBoxValueChange(&CH5FilterK)) {
//			RCChanelFilterSetN((uint16_t) STinputBoxGetValue(&CH5FilterK),
//					&RCChanel_6);
//		}
//
//		if (STinputBoxValueChange(&CH6FilterK)) {
//			RCChanelFilterSetN((uint16_t) STinputBoxGetValue(&CH6FilterK),
//					&RCChanel_7);
//		}
//
//		/*
//		 * ��������� ������� ����
//		 */
//		if (STinputBoxValueChange(&STDeadZone)) {
//			RCChanelSetDeadZone((uint16_t) STinputBoxGetValue(&STDeadZone),
//					&RCChanel_0);
//		}
//
//		if (STinputBoxValueChange(&THDeadZone)) {
//			RCChanelSetDeadZone((uint16_t) STinputBoxGetValue(&THDeadZone),
//					&RCChanel_1);
//		}
//
//		if (STinputBoxValueChange(&CH1DeadZone)) {
//			RCChanelSetDeadZone((uint16_t) STinputBoxGetValue(&CH1DeadZone),
//					&RCChanel_2);
//		}
//
//		if (STinputBoxValueChange(&CH2DeadZone)) {
//			RCChanelSetDeadZone((uint16_t) STinputBoxGetValue(&CH2DeadZone),
//					&RCChanel_3);
//		}
//
//		if (STinputBoxValueChange(&CH3DeadZone)) {
//			RCChanelSetDeadZone((uint16_t) STinputBoxGetValue(&CH3DeadZone),
//					&RCChanel_4);
//		}
//
//		if (STinputBoxValueChange(&CH4DeadZone)) {
//			RCChanelSetDeadZone((uint16_t) STinputBoxGetValue(&CH4DeadZone),
//					&RCChanel_5);
//		}
//
//		if (STinputBoxValueChange(&CH5DeadZone)) {
//			RCChanelSetDeadZone((uint16_t) STinputBoxGetValue(&CH5DeadZone),
//					&RCChanel_6);
//		}
//
//		if (STinputBoxValueChange(&CH6DeadZone)) {
//			RCChanelSetDeadZone((uint16_t) STinputBoxGetValue(&CH6DeadZone),
//					&RCChanel_7);
//		}
//
//	}

	STscreenShow(&FilterSettingsST);

//	if (STscreenOnShow(&FilterSettingsST)) {
//		STcheckBoxSetValue(RCChanelFilterGetState(&RCChanel_0),
//				&STfilterSetCheckBox);
//		STcheckBoxSetValue(RCChanelFilterGetState(&RCChanel_1),
//				&THfilterSetCheckBox);
//		STcheckBoxSetValue(RCChanelFilterGetState(&RCChanel_2),
//				&CH1filterSetCheckBox);
//		STcheckBoxSetValue(RCChanelFilterGetState(&RCChanel_3),
//				&CH2filterSetCheckBox);
//		STcheckBoxSetValue(RCChanelFilterGetState(&RCChanel_4),
//				&CH3filterSetCheckBox);
//		STcheckBoxSetValue(RCChanelFilterGetState(&RCChanel_5),
//				&CH4filterSetCheckBox);
//		STcheckBoxSetValue(RCChanelFilterGetState(&RCChanel_6),
//				&CH5filterSetCheckBox);
//		STcheckBoxSetValue(RCChanelFilterGetState(&RCChanel_7),
//				&CH6filterSetCheckBox);
//
//		STinputBoxSetValue(RCChanelFilterGetN(&RCChanel_0), &STFilterK);
//		STinputBoxSetValue(RCChanelFilterGetN(&RCChanel_1), &THFilterK);
//		STinputBoxSetValue(RCChanelFilterGetN(&RCChanel_2), &CH1FilterK);
//		STinputBoxSetValue(RCChanelFilterGetN(&RCChanel_3), &CH2FilterK);
//		STinputBoxSetValue(RCChanelFilterGetN(&RCChanel_4), &CH3FilterK);
//		STinputBoxSetValue(RCChanelFilterGetN(&RCChanel_5), &CH4FilterK);
//		STinputBoxSetValue(RCChanelFilterGetN(&RCChanel_6), &CH5FilterK);
//		STinputBoxSetValue(RCChanelFilterGetN(&RCChanel_7), &CH6FilterK);
//
//		STinputBoxSetValue(RCChanelGetDeadZone(&RCChanel_0), &STDeadZone);
//		STinputBoxSetValue(RCChanelGetDeadZone(&RCChanel_1), &THDeadZone);
//		STinputBoxSetValue(RCChanelGetDeadZone(&RCChanel_2), &CH1DeadZone);
//		STinputBoxSetValue(RCChanelGetDeadZone(&RCChanel_3), &CH2DeadZone);
//		STinputBoxSetValue(RCChanelGetDeadZone(&RCChanel_4), &CH3DeadZone);
//		STinputBoxSetValue(RCChanelGetDeadZone(&RCChanel_5), &CH4DeadZone);
//		STinputBoxSetValue(RCChanelGetDeadZone(&RCChanel_6), &CH5DeadZone);
//		STinputBoxSetValue(RCChanelGetDeadZone(&RCChanel_7), &CH6DeadZone);
//	}

	STtextBoxShow(&STNameFilterSet);
	STtextBoxShow(&THNameFilterSet);
	STtextBoxShow(&CH1NameFilterSet);
	STtextBoxShow(&CH2NameFilterSet);
	STtextBoxShow(&CH3NameFilterSet);
	STtextBoxShow(&CH4NameFilterSet);
	STtextBoxShow(&CH5NameFilterSet);
	STtextBoxShow(&CH6NameFilterSet);

	/*
	 * ���� ����������
	 */
	STinputBoxShow(&STFilterK);
	STinputBoxShow(&THFilterK);
	STinputBoxShow(&CH1FilterK);
	STinputBoxShow(&CH2FilterK);
	STinputBoxShow(&CH3FilterK);
	STinputBoxShow(&CH4FilterK);
	STinputBoxShow(&CH5FilterK);
	STinputBoxShow(&CH6FilterK);

	/*
	 * ������� ����
	 */
	STinputBoxShow(&STDeadZone);
	STinputBoxShow(&THDeadZone);
	STinputBoxShow(&CH1DeadZone);
	STinputBoxShow(&CH2DeadZone);
	STinputBoxShow(&CH3DeadZone);
	STinputBoxShow(&CH4DeadZone);
	STinputBoxShow(&CH5DeadZone);
	STinputBoxShow(&CH6DeadZone);

	/*
	 * ���/���� �����������
	 */
	STcheckBoxShow(&STfilterSetCheckBox);
	STcheckBoxShow(&THfilterSetCheckBox);
	STcheckBoxShow(&CH1filterSetCheckBox);
	STcheckBoxShow(&CH2filterSetCheckBox);
	STcheckBoxShow(&CH3filterSetCheckBox);
	STcheckBoxShow(&CH4filterSetCheckBox);
	STcheckBoxShow(&CH5filterSetCheckBox);
	STcheckBoxShow(&CH6filterSetCheckBox);

	STbuttonShow(&BackToModelMenuButtonFS);
}
void _FilterSettingsInit() {
	/*
	 *  ������������� �������� ������
	 */
	FSStyle.HeaderFont = &verdana_8pt_bold;
	FSStyle.BackgroundColor = MAIN_SCREEN_BACK_COLOR;
	FSStyle.HeaderColor = MAIN_HEADER_COLOR;
	FSStyle.HeaderTextColor = MAIN_HEADER_TEXT_COLOR;
	FSStyle.HeaderHeight = MAIN_HEADER_HEIGHT;

	FilterSettingsST.App = &STApp;
	FilterSettingsST.Style = &FSStyle;
	FilterSettingsST.ScreenID = FilterSettings;
	FilterSettingsST.FullScreen = Yes;
	FilterSettingsST.Width = GUI_MAX_WIDTH_PX;
	FilterSettingsST.Height = GUI_MAX_HEIGHT_PX;
	FilterSettingsST.Header = Yes;
	FilterSettingsST.HeaderText = "FILTER SETTINGS";
	FilterSettingsST.Footer = No;

	/*
	 *  ����� � ����� ����������
	 */
	FSButtonColor.Font = &verdana_8pt_bold;
	FSButtonColor.ParentScreen = &FilterSettingsST;
	FSButtonColor.Color = BASE_BUTTON_COLOR;
	FSButtonColor.TextColor = BASE_BUTTON_COLOR;
	FSButtonColor.BackgroundColor = BASE_BUTTON_BACK_COLOR;
	FSButtonColor.BorderThickness = BASE_BUTTON_BORDER_THICKNESS;
	FSButtonColor.BorderColor = BASE_BUTTON_BORDER_COLOR;
	FSButtonColor.FocusColor = FOCUS_BUTTON_COLOR;
	FSButtonColor.FocusBackgroundColor = FOCUS_BUTTON_BACK_COLOR;
	FSButtonColor.FocusBorderThickness = FOCUS_BUTTON_BORDER_THICKNESS;
	FSButtonColor.FocusBorderColor = FOCUS_BUTTON_BORDER_COLOR;

	FilterSetText.ParentScreen = &FilterSettingsST;
	FilterSetText.Font = &verdana_8pt_bold;
	FilterSetText.Color = BASE_TEXT_COLOR;
	FilterSetText.TextColor = BASE_TEXT_COLOR;
	FilterSetText.BackgroundColor = BASE_TEXT_BACK_COLOR;
	FilterSetText.BorderThickness = BASE_TEXT_BORDER_THICKNESS;
	FilterSetText.BorderColor = BASE_TEXT_BORDER_COLOR;

	FilterKInputBoxColor.ParentScreen = &FilterSettingsST;
	FilterKInputBoxColor.Font = &verdana_8pt_bold;
	FilterKInputBoxColor.Color = BASE_INPUT_COLOR;
	FilterKInputBoxColor.TextColor = BASE_INPUT_TEXT_COLOR;
	FilterKInputBoxColor.BackgroundColor = BASE_INPUT_BACK_COLOR;
	FilterKInputBoxColor.BorderThickness = BASE_INPUT_BORDER_THICKNESS;
	FilterKInputBoxColor.BorderColor = BASE_INPUT_BORDER_COLOR;
	FilterKInputBoxColor.FocusTextColor = FOCUS_INPUT_TEXT_COLOR;
	FilterKInputBoxColor.FocusColor = FOCUS_INPUT_COLOR;
	FilterKInputBoxColor.FocusBackgroundColor = FOCUS_INPUT_BACK_COLOR;
	FilterKInputBoxColor.FocusBorderThickness = FOCUS_INPUT_BORDER_THICKNESS;
	FilterKInputBoxColor.FocusBorderColor = FOCUS_INPUT_BORDER_COLOR;

	CHfilterSetCheckBoxColor.ParentScreen = &FilterSettingsST;
	CHfilterSetCheckBoxColor.Font = &verdana_8pt_bold;
	CHfilterSetCheckBoxColor.Color = BASE_CHECKBOX_OFF_COLOR;
	CHfilterSetCheckBoxColor.Color2 = BASE_CHECKBOX_ON_COLOR;
	CHfilterSetCheckBoxColor.TextColor = BASE_CHECKBOX_TEXT_COLOR;
	CHfilterSetCheckBoxColor.BorderThickness = BASE_CHECKBOX_BORDER_THICKNESS;
	CHfilterSetCheckBoxColor.BorderColor = BASE_CHECKBOX_BORDER_COLOR;
	CHfilterSetCheckBoxColor.FocusBorderThickness =
			FOCUS_CHECKBOX_BORDER_THICKNESS;
	CHfilterSetCheckBoxColor.FocusBorderColor = FOCUS_CHECKBOX_BORDER_COLOR;

	STNameFilterSet.Text = __CH1label;
	STNameFilterSet.Style = &FilterSetText;
	STNameFilterSet.Width = 30;
	STNameFilterSet.Height = BASE_CHECKBOX_HEIGHT;
	STNameFilterSet.X = CH_FS_NAME_X;
	STNameFilterSet.Y = ST_FS_LINE_Y;
	STNameFilterSet.Border = No;
	STNameFilterSet.TextAlignment = Right;
	STNameFilterSet.Dynamic = No;

	THNameFilterSet.Text = __CH2label;
	THNameFilterSet.Style = &FilterSetText;
	THNameFilterSet.Width = 30;
	THNameFilterSet.Height = BASE_CHECKBOX_HEIGHT;
	THNameFilterSet.X = CH_FS_NAME_X;
	THNameFilterSet.Y = TH_FS_LINE_Y;
	THNameFilterSet.Border = No;
	THNameFilterSet.TextAlignment = Right;
	THNameFilterSet.Dynamic = No;

	CH1NameFilterSet.Text = __CH3label;
	CH1NameFilterSet.Style = &FilterSetText;
	CH1NameFilterSet.Width = 30;
	CH1NameFilterSet.Height = BASE_CHECKBOX_HEIGHT;
	CH1NameFilterSet.X = CH_FS_NAME_X;
	CH1NameFilterSet.Y = CH1_FS_LINE_Y;
	CH1NameFilterSet.Border = No;
	CH1NameFilterSet.TextAlignment = Right;
	CH1NameFilterSet.Dynamic = No;

	CH2NameFilterSet.Text = __CH4label;
	CH2NameFilterSet.Style = &FilterSetText;
	CH2NameFilterSet.Width = 30;
	CH2NameFilterSet.Height = BASE_CHECKBOX_HEIGHT;
	CH2NameFilterSet.X = CH_FS_NAME_X;
	CH2NameFilterSet.Y = CH2_FS_LINE_Y;
	CH2NameFilterSet.Border = No;
	CH2NameFilterSet.TextAlignment = Right;
	CH2NameFilterSet.Dynamic = No;

	CH3NameFilterSet.Text = __CH5label;
	CH3NameFilterSet.Style = &FilterSetText;
	CH3NameFilterSet.Width = 30;
	CH3NameFilterSet.Height = BASE_CHECKBOX_HEIGHT;
	CH3NameFilterSet.X = CH_FS_NAME_X;
	CH3NameFilterSet.Y = CH3_FS_LINE_Y;
	CH3NameFilterSet.Border = No;
	CH3NameFilterSet.TextAlignment = Right;
	CH3NameFilterSet.Dynamic = No;

	CH4NameFilterSet.Text = __CH6label;
	CH4NameFilterSet.Style = &FilterSetText;
	CH4NameFilterSet.Width = 30;
	CH4NameFilterSet.Height = BASE_CHECKBOX_HEIGHT;
	CH4NameFilterSet.X = CH_FS_NAME_X;
	CH4NameFilterSet.Y = CH4_FS_LINE_Y;
	CH4NameFilterSet.Border = No;
	CH4NameFilterSet.TextAlignment = Right;
	CH4NameFilterSet.Dynamic = No;

	CH5NameFilterSet.Text = __CH7label;
	CH5NameFilterSet.Style = &FilterSetText;
	CH5NameFilterSet.Width = 30;
	CH5NameFilterSet.Height = BASE_CHECKBOX_HEIGHT;
	CH5NameFilterSet.X = CH_FS_NAME_X;
	CH5NameFilterSet.Y = CH5_FS_LINE_Y;
	CH5NameFilterSet.Border = No;
	CH5NameFilterSet.TextAlignment = Right;
	CH5NameFilterSet.Dynamic = No;

	CH6NameFilterSet.Text = __CH8label;
	CH6NameFilterSet.Style = &FilterSetText;
	CH6NameFilterSet.Width = 30;
	CH6NameFilterSet.Height = BASE_CHECKBOX_HEIGHT;
	CH6NameFilterSet.X = CH_FS_NAME_X;
	CH6NameFilterSet.Y = CH6_FS_LINE_Y;
	CH6NameFilterSet.Border = No;
	CH6NameFilterSet.TextAlignment = Right;
	CH6NameFilterSet.Dynamic = No;

	STfilterSetCheckBox.OnText = __buttonLabelCheckBoxOn;
	STfilterSetCheckBox.OffText = __buttonLabelCheckBoxOff;
	STfilterSetCheckBox.Style = &CHfilterSetCheckBoxColor;
	STfilterSetCheckBox.Width = BASE_CHECKBOX_WIDTH;
	STfilterSetCheckBox.Height = BASE_CHECKBOX_HEIGHT;
	STfilterSetCheckBox.X = CH_FS_CHECKBOX_X;
	STfilterSetCheckBox.Y = ST_FS_LINE_Y;
	STfilterSetCheckBox.Border = Yes;
	STfilterSetCheckBox.Dynamic = Yes;

	THfilterSetCheckBox.OnText = __buttonLabelCheckBoxOn;
	THfilterSetCheckBox.OffText = __buttonLabelCheckBoxOff;
	THfilterSetCheckBox.Style = &CHfilterSetCheckBoxColor;
	THfilterSetCheckBox.Width = BASE_CHECKBOX_WIDTH;
	THfilterSetCheckBox.Height = BASE_CHECKBOX_HEIGHT;
	THfilterSetCheckBox.X = CH_FS_CHECKBOX_X;
	THfilterSetCheckBox.Y = TH_FS_LINE_Y;
	THfilterSetCheckBox.Border = Yes;
	THfilterSetCheckBox.Dynamic = Yes;

	CH1filterSetCheckBox.OnText = __buttonLabelCheckBoxOn;
	CH1filterSetCheckBox.OffText = __buttonLabelCheckBoxOff;
	CH1filterSetCheckBox.Style = &CHfilterSetCheckBoxColor;
	CH1filterSetCheckBox.Width = BASE_CHECKBOX_WIDTH;
	CH1filterSetCheckBox.Height = BASE_CHECKBOX_HEIGHT;
	CH1filterSetCheckBox.X = CH_FS_CHECKBOX_X;
	CH1filterSetCheckBox.Y = CH1_FS_LINE_Y;
	CH1filterSetCheckBox.Border = Yes;
	CH1filterSetCheckBox.Dynamic = Yes;

	CH2filterSetCheckBox.OnText = __buttonLabelCheckBoxOn;
	CH2filterSetCheckBox.OffText = __buttonLabelCheckBoxOff;
	CH2filterSetCheckBox.Style = &CHfilterSetCheckBoxColor;
	CH2filterSetCheckBox.Width = BASE_CHECKBOX_WIDTH;
	CH2filterSetCheckBox.Height = BASE_CHECKBOX_HEIGHT;
	CH2filterSetCheckBox.X = CH_FS_CHECKBOX_X;
	CH2filterSetCheckBox.Y = CH2_FS_LINE_Y;
	CH2filterSetCheckBox.Border = Yes;
	CH2filterSetCheckBox.Dynamic = Yes;

	CH3filterSetCheckBox.OnText = __buttonLabelCheckBoxOn;
	CH3filterSetCheckBox.OffText = __buttonLabelCheckBoxOff;
	CH3filterSetCheckBox.Style = &CHfilterSetCheckBoxColor;
	CH3filterSetCheckBox.Width = BASE_CHECKBOX_WIDTH;
	CH3filterSetCheckBox.Height = BASE_CHECKBOX_HEIGHT;
	CH3filterSetCheckBox.X = CH_FS_CHECKBOX_X;
	CH3filterSetCheckBox.Y = CH3_FS_LINE_Y;
	CH3filterSetCheckBox.Border = Yes;
	CH3filterSetCheckBox.Dynamic = Yes;

	CH4filterSetCheckBox.OnText = __buttonLabelCheckBoxOn;
	CH4filterSetCheckBox.OffText = __buttonLabelCheckBoxOff;
	CH4filterSetCheckBox.Style = &CHfilterSetCheckBoxColor;
	CH4filterSetCheckBox.Width = BASE_CHECKBOX_WIDTH;
	CH4filterSetCheckBox.Height = BASE_CHECKBOX_HEIGHT;
	CH4filterSetCheckBox.X = CH_FS_CHECKBOX_X;
	CH4filterSetCheckBox.Y = CH4_FS_LINE_Y;
	CH4filterSetCheckBox.Border = Yes;
	CH4filterSetCheckBox.Dynamic = Yes;

	CH5filterSetCheckBox.OnText = __buttonLabelCheckBoxOn;
	CH5filterSetCheckBox.OffText = __buttonLabelCheckBoxOff;
	CH5filterSetCheckBox.Style = &CHfilterSetCheckBoxColor;
	CH5filterSetCheckBox.Width = BASE_CHECKBOX_WIDTH;
	CH5filterSetCheckBox.Height = BASE_CHECKBOX_HEIGHT;
	CH5filterSetCheckBox.X = CH_FS_CHECKBOX_X;
	CH5filterSetCheckBox.Y = CH5_FS_LINE_Y;
	CH5filterSetCheckBox.Border = Yes;
	CH5filterSetCheckBox.Dynamic = Yes;

	CH6filterSetCheckBox.OnText = __buttonLabelCheckBoxOn;
	CH6filterSetCheckBox.OffText = __buttonLabelCheckBoxOff;
	CH6filterSetCheckBox.Style = &CHfilterSetCheckBoxColor;
	CH6filterSetCheckBox.Width = BASE_CHECKBOX_WIDTH;
	CH6filterSetCheckBox.Height = BASE_CHECKBOX_HEIGHT;
	CH6filterSetCheckBox.X = CH_FS_CHECKBOX_X;
	CH6filterSetCheckBox.Y = CH6_FS_LINE_Y;
	CH6filterSetCheckBox.Border = Yes;
	CH6filterSetCheckBox.Dynamic = Yes;

	STFilterK.Style = &FilterKInputBoxColor;
	STFilterK.Width = INPUT_FSETS_INPUT_WIDTH;
	STFilterK.Height = INPUT_BASE_HEIGHT;
	STFilterK.X = CH_FS_X;
	STFilterK.Y = ST_FS_LINE_Y;
	STFilterK.HiLimit = FILTER_BUFFER_SIZE;
	STFilterK.LoLimit = 2;
	STFilterK.TextAlignment = Centre;
	STFilterK.Border = Yes;
	STFilterK.Dynamic = Yes;

	THFilterK.Style = &FilterKInputBoxColor;
	THFilterK.Width = INPUT_FSETS_INPUT_WIDTH;
	THFilterK.Height = INPUT_BASE_HEIGHT;
	THFilterK.X = CH_FS_X;
	THFilterK.Y = TH_FS_LINE_Y;
	THFilterK.HiLimit = FILTER_BUFFER_SIZE;
	THFilterK.LoLimit = 2;
	THFilterK.TextAlignment = Centre;
	THFilterK.Border = Yes;
	THFilterK.Dynamic = Yes;

	CH1FilterK.Style = &FilterKInputBoxColor;
	CH1FilterK.Width = INPUT_FSETS_INPUT_WIDTH;
	CH1FilterK.Height = INPUT_BASE_HEIGHT;
	CH1FilterK.X = CH_FS_X;
	CH1FilterK.Y = CH1_FS_LINE_Y;
	CH1FilterK.HiLimit = FILTER_BUFFER_SIZE;
	CH1FilterK.LoLimit = 2;
	CH1FilterK.TextAlignment = Centre;
	CH1FilterK.Border = Yes;
	CH1FilterK.Dynamic = Yes;

	CH2FilterK.Style = &FilterKInputBoxColor;
	CH2FilterK.Width = INPUT_FSETS_INPUT_WIDTH;
	CH2FilterK.Height = INPUT_BASE_HEIGHT;
	CH2FilterK.X = CH_FS_X;
	CH2FilterK.Y = CH2_FS_LINE_Y;
	CH2FilterK.HiLimit = FILTER_BUFFER_SIZE;
	CH2FilterK.LoLimit = 2;
	CH2FilterK.TextAlignment = Centre;
	CH2FilterK.Border = Yes;
	CH2FilterK.Dynamic = Yes;

	CH3FilterK.Style = &FilterKInputBoxColor;
	CH3FilterK.Width = INPUT_FSETS_INPUT_WIDTH;
	CH3FilterK.Height = INPUT_BASE_HEIGHT;
	CH3FilterK.X = CH_FS_X;
	CH3FilterK.Y = CH3_FS_LINE_Y;
	CH3FilterK.HiLimit = FILTER_BUFFER_SIZE;
	CH3FilterK.LoLimit = 2;
	CH3FilterK.TextAlignment = Centre;
	CH3FilterK.Border = Yes;
	CH3FilterK.Dynamic = Yes;

	CH4FilterK.Style = &FilterKInputBoxColor;
	CH4FilterK.Width = INPUT_FSETS_INPUT_WIDTH;
	CH4FilterK.Height = INPUT_BASE_HEIGHT;
	CH4FilterK.X = CH_FS_X;
	CH4FilterK.Y = CH4_FS_LINE_Y;
	CH4FilterK.HiLimit = FILTER_BUFFER_SIZE;
	CH4FilterK.LoLimit = 2;
	CH4FilterK.TextAlignment = Centre;
	CH4FilterK.Border = Yes;
	CH4FilterK.Dynamic = Yes;

	CH5FilterK.Style = &FilterKInputBoxColor;
	CH5FilterK.Width = INPUT_FSETS_INPUT_WIDTH;
	CH5FilterK.Height = INPUT_BASE_HEIGHT;
	CH5FilterK.X = CH_FS_X;
	CH5FilterK.Y = CH5_FS_LINE_Y;
	CH5FilterK.HiLimit = FILTER_BUFFER_SIZE;
	CH5FilterK.LoLimit = 2;
	CH5FilterK.TextAlignment = Centre;
	CH5FilterK.Border = Yes;
	CH5FilterK.Dynamic = Yes;

	CH6FilterK.Style = &FilterKInputBoxColor;
	CH6FilterK.Width = INPUT_FSETS_INPUT_WIDTH;
	CH6FilterK.Height = INPUT_BASE_HEIGHT;
	CH6FilterK.X = CH_FS_X;
	CH6FilterK.Y = CH6_FS_LINE_Y;
	CH6FilterK.HiLimit = FILTER_BUFFER_SIZE;
	CH6FilterK.LoLimit = 2;
	CH6FilterK.TextAlignment = Centre;
	CH6FilterK.Border = Yes;
	CH6FilterK.Dynamic = Yes;

	STDeadZone.Style = &FilterKInputBoxColor;
	STDeadZone.Width = INPUT_FSETS_INPUT_WIDTH;
	STDeadZone.Height = INPUT_BASE_HEIGHT;
	STDeadZone.X = CH_DZ_X;
	STDeadZone.Y = ST_FS_LINE_Y;
	STDeadZone.HiLimit = 100;
	STDeadZone.LoLimit = 0;
	STDeadZone.TextAlignment = Centre;
	STDeadZone.Border = Yes;
	STDeadZone.Dynamic = Yes;

	THDeadZone.Style = &FilterKInputBoxColor;
	THDeadZone.Width = INPUT_FSETS_INPUT_WIDTH;
	THDeadZone.Height = INPUT_BASE_HEIGHT;
	THDeadZone.X = CH_DZ_X;
	THDeadZone.Y = TH_FS_LINE_Y;
	THDeadZone.HiLimit = 100;
	THDeadZone.LoLimit = 0;
	THDeadZone.TextAlignment = Centre;
	THDeadZone.Border = Yes;
	THDeadZone.Dynamic = Yes;

	CH1DeadZone.Style = &FilterKInputBoxColor;
	CH1DeadZone.Width = INPUT_FSETS_INPUT_WIDTH;
	CH1DeadZone.Height = INPUT_BASE_HEIGHT;
	CH1DeadZone.X = CH_DZ_X;
	CH1DeadZone.Y = CH1_FS_LINE_Y;
	CH1DeadZone.HiLimit = 100;
	CH1DeadZone.LoLimit = 0;
	CH1DeadZone.TextAlignment = Centre;
	CH1DeadZone.Border = Yes;
	CH1DeadZone.Dynamic = Yes;

	CH2DeadZone.Style = &FilterKInputBoxColor;
	CH2DeadZone.Width = INPUT_FSETS_INPUT_WIDTH;
	CH2DeadZone.Height = INPUT_BASE_HEIGHT;
	CH2DeadZone.X = CH_DZ_X;
	CH2DeadZone.Y = CH2_FS_LINE_Y;
	CH2DeadZone.HiLimit = 100;
	CH2DeadZone.LoLimit = 0;
	CH2DeadZone.TextAlignment = Centre;
	CH2DeadZone.Border = Yes;
	CH2DeadZone.Dynamic = Yes;

	CH3DeadZone.Style = &FilterKInputBoxColor;
	CH3DeadZone.Width = INPUT_FSETS_INPUT_WIDTH;
	CH3DeadZone.Height = INPUT_BASE_HEIGHT;
	CH3DeadZone.X = CH_DZ_X;
	CH3DeadZone.Y = CH3_FS_LINE_Y;
	CH3DeadZone.HiLimit = 100;
	CH3DeadZone.LoLimit = 0;
	CH3DeadZone.TextAlignment = Centre;
	CH3DeadZone.Border = Yes;
	CH3DeadZone.Dynamic = Yes;

	CH4DeadZone.Style = &FilterKInputBoxColor;
	CH4DeadZone.Width = INPUT_FSETS_INPUT_WIDTH;
	CH4DeadZone.Height = INPUT_BASE_HEIGHT;
	CH4DeadZone.X = CH_DZ_X;
	CH4DeadZone.Y = CH4_FS_LINE_Y;
	CH4DeadZone.HiLimit = 100;
	CH4DeadZone.LoLimit = 0;
	CH4DeadZone.TextAlignment = Centre;
	CH4DeadZone.Border = Yes;
	CH4DeadZone.Dynamic = Yes;

	CH5DeadZone.Style = &FilterKInputBoxColor;
	CH5DeadZone.Width = INPUT_FSETS_INPUT_WIDTH;
	CH5DeadZone.Height = INPUT_BASE_HEIGHT;
	CH5DeadZone.X = CH_DZ_X;
	CH5DeadZone.Y = CH5_FS_LINE_Y;
	CH5DeadZone.HiLimit = 100;
	CH5DeadZone.LoLimit = 0;
	CH5DeadZone.TextAlignment = Centre;
	CH5DeadZone.Border = Yes;
	CH5DeadZone.Dynamic = Yes;

	CH6DeadZone.Style = &FilterKInputBoxColor;
	CH6DeadZone.Width = INPUT_FSETS_INPUT_WIDTH;
	CH6DeadZone.Height = INPUT_BASE_HEIGHT;
	CH6DeadZone.X = CH_DZ_X;
	CH6DeadZone.Y = CH6_FS_LINE_Y;
	CH6DeadZone.HiLimit = 100;
	CH6DeadZone.LoLimit = 0;
	CH6DeadZone.TextAlignment = Centre;
	CH6DeadZone.Border = Yes;
	CH6DeadZone.Dynamic = Yes;

	BackToModelMenuButtonFS.Text = __buttonLabelBack;
	BackToModelMenuButtonFS.Style = &FSButtonColor;
	BackToModelMenuButtonFS.Width = 60;
	BackToModelMenuButtonFS.Height = 26;
	BackToModelMenuButtonFS.X = MAIN_BACK_BUTTON_X;
	BackToModelMenuButtonFS.Y = MAIN_BACK_BUTTON_Y;
	BackToModelMenuButtonFS.Border = Yes;

	STtextBoxInit(&STNameFilterSet);
	STtextBoxInit(&THNameFilterSet);
	STtextBoxInit(&CH1NameFilterSet);
	STtextBoxInit(&CH2NameFilterSet);
	STtextBoxInit(&CH3NameFilterSet);
	STtextBoxInit(&CH4NameFilterSet);
	STtextBoxInit(&CH5NameFilterSet);
	STtextBoxInit(&CH6NameFilterSet);

	STcheckBoxInit(&STfilterSetCheckBox);
	STcheckBoxInit(&THfilterSetCheckBox);
	STcheckBoxInit(&CH1filterSetCheckBox);
	STcheckBoxInit(&CH2filterSetCheckBox);
	STcheckBoxInit(&CH3filterSetCheckBox);
	STcheckBoxInit(&CH4filterSetCheckBox);
	STcheckBoxInit(&CH5filterSetCheckBox);
	STcheckBoxInit(&CH6filterSetCheckBox);

	STinputBoxInit(&STFilterK);
	STinputBoxInit(&THFilterK);
	STinputBoxInit(&CH1FilterK);
	STinputBoxInit(&CH2FilterK);
	STinputBoxInit(&CH3FilterK);
	STinputBoxInit(&CH4FilterK);
	STinputBoxInit(&CH5FilterK);
	STinputBoxInit(&CH6FilterK);

	STinputBoxInit(&STDeadZone);
	STinputBoxInit(&THDeadZone);
	STinputBoxInit(&CH1DeadZone);
	STinputBoxInit(&CH2DeadZone);
	STinputBoxInit(&CH3DeadZone);
	STinputBoxInit(&CH4DeadZone);
	STinputBoxInit(&CH5DeadZone);
	STinputBoxInit(&CH6DeadZone);

	STbuttonInit(&BackToModelMenuButtonFS);
}
