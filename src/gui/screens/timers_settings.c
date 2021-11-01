/*
 * timers_settings.c
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
#include <gui/fonts/verdana_12pt.h>
#include <gui/fonts/verdana_8pt.h>
#include <gui/fonts/verdana_8pt.h>
#include "core/auxiliary.h"
#include "core/buffer.h"

#define TIMER_SETTINGS_FIELD_WIDTH					70
#define TIMER_SETTINGS_FIELD_HEIGHT					25
#define TIMER_SETTINGS_FIELD_NAME_X					1
#define TIMER_SETTINGS_FIELD_NAME_Y				    50
#define TIMER_SETTINGS_FIELD_NAME_STEP				25

#define TIMER_SETTINGS_RENAME_X						180
#define TIMER_SETTINGS_RENAME_Y						25

#define TIMER_SETTINGS_SETPOINT_FIELD_WIDTH			40
#define TIMER_SETTINGS_SETPOINT_FIELD_HEIGHT		20
#define TIMER_SETTINGS_SETPOINT_FIELD_X				120
#define TIMER_SETTINGS_SETPOINT_FIELD_X_STEP		TIMER_SETTINGS_SETPOINT_FIELD_WIDTH + 5

#define TIMER1_NAME_X								1
#define TIMER1_NAME_Y								25
#define TIMER_SETS_DISCRETE_LIST_X					80
#define TIMER_SETS_DISCRETE_LIST_Y					50
#define TIMER_ALARM_ENABLE_CHECK_BOX_X				80
#define TIMER_ALARM_ENABLE_CHECK_BOX_Y				75
#define TIMER_SETTINGS_FIELD_NAME_NUM				2
#define TIME_ITEM_STEP								150

#define TIMER_1										1
#define TIMER_2										2


STScreenStyleTypeDef TimerSetStyle = { 0 };

STScreenTypeDef TimerSetST = { 0 };

STColorStyleTypeDef TimerSetButtonColor = { 0 };

STColorStyleTypeDef TimerSetInputBoxColor = { 0 };
STColorStyleTypeDef DiscretMappingListColor = { 0 };
STColorStyleTypeDef TimerSetText = { 0 };
STColorStyleTypeDef TimersSetAlarmCheckBoxColor = { 0 };
STColorStyleTypeDef TimerSettingsText = { 0 };
STColorStyleTypeDef TimerSettingsInputBoxColor  = { 0 };

STbuttonTypeDef BackToModelMenuButtonTimSet = { 0 };
STbuttonTypeDef Timer1RenameButton = { 0 };
STbuttonTypeDef Timer2RenameButton = { 0 };

STColorStyleTypeDef TimerNameStyle = { 0 };

STcheckBoxTypeDef Timer1AlarmEnableCheckBox = { 0 };
STcheckBoxTypeDef Timer2AlarmEnableCheckBox = { 0 };

STtextBoxTypeDef Timer1Name = { 0 };
STtextBoxTypeDef Timer2Name = { 0 };

STlistBoxTypeDef Timer1SourceList = { 0 };
STlistBoxTypeDef Timer2SourceList = { 0 };

STinputBoxTypeDef Timer1SecondSetpoint = { 0 };
STinputBoxTypeDef Timer1MinuteSetpoint = { 0 };
STinputBoxTypeDef Timer2SecondSetpoint = { 0 };
STinputBoxTypeDef Timer2MinuteSetpoint = { 0 };

const char TimerSettingsFieldNameLabel[TIMER_SETTINGS_FIELD_NAME_NUM][16] = {
																				"Channel",
																				"Alarm",
																			//	"Name",
																			//	"Beeper"
																			};

STtextBoxTypeDef Timer1SettingsFieldName[TIMER_SETTINGS_FIELD_NAME_NUM] = { 0 };
STtextBoxTypeDef Timer2SettingsFieldName[TIMER_SETTINGS_FIELD_NAME_NUM] = { 0 };

void _TimerSet()
{
	uint16_t i = 0;

	static uint16_t TimerWaiting = 0;

	if (STscreenShowNow(&TimerSetST)) {
		if (STbuttonPressed(&BackToModelMenuButtonTimSet)) {

			STsaveSettingsToFlash();

			STappSetScreen(MainScreenScr, &STApp);
		}

		/*
		 *  Rename button
		 */

		if (STbuttonPressed(&Timer1RenameButton)) {
			STappSetScreen(Keyboard, &STApp);
			TimerWaiting = TIMER_1;
		}

		if (STbuttonPressed(&Timer2RenameButton)) {
			STappSetScreen(Keyboard, &STApp);
			TimerWaiting = TIMER_2;
		}

		if (STappGetTextBuffState(&STApp) == Yes) {

			switch(TimerWaiting)
			{
			case TIMER_1:

				RCStimerSetName(STappGetInputTextBuff(&STApp), &RCTimer1);
				STappClearBuff(&STApp);
				TimerWaiting = 0;

				break;

			case TIMER_2:

				RCStimerSetName(STappGetInputTextBuff(&STApp), &RCTimer2);
				STappClearBuff(&STApp);
				TimerWaiting = 0;

				break;

			default:
				break;
			}
		}

		/*
		 * Alarm Enable/Disable
		 */

		if (STcheckBoxValueChange(&Timer1AlarmEnableCheckBox))
		{
			RCStimerSetAlarmState(STcheckBoxGetState(&Timer1AlarmEnableCheckBox), &RCTimer1);
		}

		if (STcheckBoxValueChange(&Timer2AlarmEnableCheckBox))
		{
			RCStimerSetAlarmState(STcheckBoxGetState(&Timer2AlarmEnableCheckBox), &RCTimer2);
		}

		/*
		 * Timers start/stop sources
		 */
		if (STlistBoxValueChange(&Timer1SourceList))
		{
			RCStimerSetBuffItemNumber(STlistBoxGetValue(&Timer1SourceList),&RCTimer1);
		}

		if (STlistBoxValueChange(&Timer2SourceList))
		{
			RCStimerSetBuffItemNumber(STlistBoxGetValue(&Timer2SourceList),&RCTimer2);
		}

		/*
		 * Timers setpoint
		 */

		if (STinputBoxValueChange(&Timer1MinuteSetpoint))
		{
			RCStimerSetMinuteSetpoint(STinputBoxGetValue(&Timer1MinuteSetpoint), &RCTimer1);
		}

		if (STinputBoxValueChange(&Timer1SecondSetpoint))
		{
			RCStimerSetSecondSetpoint(STinputBoxGetValue(&Timer1SecondSetpoint), &RCTimer1);
		}

		if (STinputBoxValueChange(&Timer2MinuteSetpoint))
		{
			RCStimerSetMinuteSetpoint(STinputBoxGetValue(&Timer2MinuteSetpoint), &RCTimer2);
		}

		if (STinputBoxValueChange(&Timer2SecondSetpoint))
		{
			RCStimerSetSecondSetpoint(STinputBoxGetValue(&Timer2SecondSetpoint), &RCTimer2);
		}

		/*
		 * Timers name
		 */
		STtextBoxSetValue(RCStimerGetName(&RCTimer1), &Timer1Name);
		STtextBoxSetValue(RCStimerGetName(&RCTimer2), &Timer2Name);
	}

	STscreenShow(&TimerSetST);

	if (STscreenOnShow(&TimerSetST))
	{
		STlistBoxSetValue(RCStimerGetBuffItemNumber(&RCTimer1),	&Timer1SourceList);
		STlistBoxSetValue(RCStimerGetBuffItemNumber(&RCTimer2),	&Timer2SourceList);
		STcheckBoxSetValue(RCStimerGetAlarmState(&RCTimer1),	&Timer1AlarmEnableCheckBox);
		STcheckBoxSetValue(RCStimerGetAlarmState(&RCTimer2),	&Timer2AlarmEnableCheckBox);
		STinputBoxSetValue(RCStimerGetMinuteSetpoint(&RCTimer1), &Timer1MinuteSetpoint);
		STinputBoxSetValue(RCStimerGetSecondSetpoint(&RCTimer1), &Timer1SecondSetpoint);
		STinputBoxSetValue(RCStimerGetMinuteSetpoint(&RCTimer2), &Timer2MinuteSetpoint);
		STinputBoxSetValue(RCStimerGetSecondSetpoint(&RCTimer2), &Timer2SecondSetpoint);
	}

	for(i = 0; i < TIMER_SETTINGS_FIELD_NAME_NUM; i++)
	{
		STtextBoxShow(&Timer1SettingsFieldName[i]);
		STtextBoxShow(&Timer2SettingsFieldName[i]);
	}

	STinputBoxShow(&Timer1SecondSetpoint);
	STinputBoxShow(&Timer1MinuteSetpoint);
	STinputBoxShow(&Timer2SecondSetpoint);
	STinputBoxShow(&Timer2MinuteSetpoint);
	STtextBoxShow(&Timer1Name);
	STtextBoxShow(&Timer2Name);
	STlistBoxShow(&Timer1SourceList);
	STlistBoxShow(&Timer2SourceList);
	STcheckBoxShow(&Timer1AlarmEnableCheckBox);
	STcheckBoxShow(&Timer2AlarmEnableCheckBox);
	STbuttonShow(&Timer1RenameButton);
	STbuttonShow(&Timer2RenameButton);

	STbuttonShow(&BackToModelMenuButtonTimSet);
}
void _TimerSetInit()
{

	uint16_t i = 0;

	TimerSetStyle.HeaderFont = &verdana_8pt_bold;
	TimerSetStyle.BackgroundColor = MAIN_SCREEN_BACK_COLOR;
	TimerSetStyle.HeaderColor = MAIN_HEADER_COLOR;
	TimerSetStyle.HeaderTextColor = MAIN_HEADER_TEXT_COLOR;
	TimerSetStyle.HeaderHeight = MAIN_HEADER_HEIGHT;

	TimerSetST.App = &STApp;
	TimerSetST.Style = &TimerSetStyle;
	TimerSetST.ScreenID = TimersSettings;
	TimerSetST.FullScreen = Yes;
	TimerSetST.Width = GUI_MAX_WIDTH_PX;
	TimerSetST.Height = GUI_MAX_HEIGHT_PX;
	TimerSetST.Header = Yes;
	TimerSetST.HeaderText = "Timers";
	TimerSetST.Footer = No;

	TimerSetText.ParentScreen = &TimerSetST;
	TimerSetText.Font = &verdana_8pt_bold;
	TimerSetText.Color = BASE_TEXT_COLOR;
	TimerSetText.TextColor = BASE_TEXT_COLOR;
	TimerSetText.BackgroundColor = BASE_TEXT_BACK_COLOR;
	TimerSetText.BorderThickness = BASE_TEXT_BORDER_THICKNESS;
	TimerSetText.BorderColor = BASE_TEXT_BORDER_COLOR;

	TimerNameStyle.Font = &verdana_12pt_bold;
	TimerNameStyle.ParentScreen = &TimerSetST;
	TimerNameStyle.Color = MODELNAME_COLOR;
	TimerNameStyle.TextColor = MODELNAME_COLOR;
	TimerNameStyle.BackgroundColor = BASE_TEXT_BACK_COLOR;
	TimerNameStyle.BorderThickness = BASE_TEXT_BORDER_THICKNESS;
	TimerNameStyle.BorderColor = BASE_TEXT_BORDER_COLOR;

	TimerSettingsText.ParentScreen = &TimerSetST;
	TimerSettingsText.Font = &verdana_8pt_bold;
	TimerSettingsText.Color = BASE_TEXT_COLOR;
	TimerSettingsText.TextColor = BASE_TEXT_COLOR;
	TimerSettingsText.BackgroundColor = BASE_TEXT_BACK_COLOR;
	TimerSettingsText.BorderThickness = BASE_TEXT_BORDER_THICKNESS;
	TimerSettingsText.BorderColor = BASE_TEXT_BORDER_COLOR;

	TimersSetAlarmCheckBoxColor.ParentScreen = &TimerSetST;
	TimersSetAlarmCheckBoxColor.Font = &verdana_8pt_bold;
	TimersSetAlarmCheckBoxColor.Color = BASE_CHECKBOX_OFF_COLOR;
	TimersSetAlarmCheckBoxColor.Color2 = BASE_CHECKBOX_ON_COLOR;
	TimersSetAlarmCheckBoxColor.TextColor = BASE_CHECKBOX_TEXT_COLOR;
	TimersSetAlarmCheckBoxColor.BorderThickness = BASE_CHECKBOX_BORDER_THICKNESS;
	TimersSetAlarmCheckBoxColor.BorderColor = BASE_CHECKBOX_BORDER_COLOR;
	TimersSetAlarmCheckBoxColor.FocusBorderThickness =FOCUS_CHECKBOX_BORDER_THICKNESS;
	TimersSetAlarmCheckBoxColor.FocusBorderColor = FOCUS_CHECKBOX_BORDER_COLOR;

	DiscretMappingListColor.ParentScreen = &TimerSetST;
	DiscretMappingListColor.Font = &verdana_8pt_bold;
	DiscretMappingListColor.Color = BASE_INPUT_COLOR;
	DiscretMappingListColor.TextColor = BASE_INPUT_TEXT_COLOR;
	DiscretMappingListColor.BackgroundColor = BASE_INPUT_BACK_COLOR;
	DiscretMappingListColor.BorderThickness = BASE_INPUT_BORDER_THICKNESS;
	DiscretMappingListColor.BorderColor = BASE_INPUT_BORDER_COLOR;
	DiscretMappingListColor.FocusTextColor = FOCUS_INPUT_TEXT_COLOR;
	DiscretMappingListColor.FocusColor = FOCUS_INPUT_COLOR;
	DiscretMappingListColor.FocusBackgroundColor = FOCUS_INPUT_BACK_COLOR;
	DiscretMappingListColor.FocusBorderThickness = FOCUS_INPUT_BORDER_THICKNESS;
	DiscretMappingListColor.FocusBorderColor = FOCUS_INPUT_BORDER_COLOR;

	TimerSetButtonColor.Font = &verdana_8pt_bold;
	TimerSetButtonColor.ParentScreen = &TimerSetST;
	TimerSetButtonColor.Color = BASE_BUTTON_COLOR;
	TimerSetButtonColor.TextColor = BASE_BUTTON_COLOR;
	TimerSetButtonColor.BackgroundColor = BASE_BUTTON_BACK_COLOR;
	TimerSetButtonColor.BorderThickness = BASE_BUTTON_BORDER_THICKNESS;
	TimerSetButtonColor.BorderColor = BASE_BUTTON_BORDER_COLOR;
	TimerSetButtonColor.FocusColor = FOCUS_BUTTON_COLOR;
	TimerSetButtonColor.FocusBackgroundColor = FOCUS_BUTTON_BACK_COLOR;
	TimerSetButtonColor.FocusBorderThickness = FOCUS_BUTTON_BORDER_THICKNESS;
	TimerSetButtonColor.FocusBorderColor = FOCUS_BUTTON_BORDER_COLOR;

	TimerSetInputBoxColor.ParentScreen = &TimerSetST;
	TimerSetInputBoxColor.Font = &verdana_8pt_bold;
	TimerSetInputBoxColor.Color = BASE_INPUT_COLOR;
	TimerSetInputBoxColor.TextColor = BASE_INPUT_TEXT_COLOR;
	TimerSetInputBoxColor.BackgroundColor = BASE_INPUT_BACK_COLOR;
	TimerSetInputBoxColor.BorderThickness = BASE_INPUT_BORDER_THICKNESS;
	TimerSetInputBoxColor.BorderColor = BASE_INPUT_BORDER_COLOR;
	TimerSetInputBoxColor.FocusTextColor = FOCUS_INPUT_TEXT_COLOR;
	TimerSetInputBoxColor.FocusColor = FOCUS_INPUT_COLOR;
	TimerSetInputBoxColor.FocusBackgroundColor = FOCUS_INPUT_BACK_COLOR;
	TimerSetInputBoxColor.FocusBorderThickness = FOCUS_INPUT_BORDER_THICKNESS;
	TimerSetInputBoxColor.FocusBorderColor = FOCUS_INPUT_BORDER_COLOR;

	TimerSettingsInputBoxColor.ParentScreen = &TimerSetST;
	TimerSettingsInputBoxColor.Font = &verdana_8pt_bold;
	TimerSettingsInputBoxColor.Color = BASE_INPUT_COLOR;
	TimerSettingsInputBoxColor.TextColor = BASE_INPUT_TEXT_COLOR;
	TimerSettingsInputBoxColor.BackgroundColor = BASE_INPUT_BACK_COLOR;
	TimerSettingsInputBoxColor.BorderThickness = BASE_INPUT_BORDER_THICKNESS;
	TimerSettingsInputBoxColor.BorderColor = BASE_INPUT_BORDER_COLOR;
	TimerSettingsInputBoxColor.FocusTextColor = FOCUS_INPUT_TEXT_COLOR;
	TimerSettingsInputBoxColor.FocusColor = FOCUS_INPUT_COLOR;
	TimerSettingsInputBoxColor.FocusBackgroundColor = FOCUS_INPUT_BACK_COLOR;
	TimerSettingsInputBoxColor.FocusBorderThickness = FOCUS_INPUT_BORDER_THICKNESS;
	TimerSettingsInputBoxColor.FocusBorderColor = FOCUS_INPUT_BORDER_COLOR;

	Timer1Name.Style = &TimerNameStyle;
	Timer1Name.Width = 200;
	Timer1Name.Height = 18;
	Timer1Name.X = TIMER1_NAME_X;
	Timer1Name.Y = TIMER1_NAME_Y;
	Timer1Name.Border = No;
	Timer1Name.TextAlignment = Left;
	Timer1Name.Dynamic = No;

	Timer2Name.Style = &TimerNameStyle;
	Timer2Name.Width = 200;
	Timer2Name.Height = 18;
	Timer2Name.X = TIMER1_NAME_X;
	Timer2Name.Y = TIMER1_NAME_Y + TIME_ITEM_STEP;
	Timer2Name.Border = No;
	Timer2Name.TextAlignment = Left;
	Timer2Name.Dynamic = No;

	Timer1SourceList.Style = &DiscretMappingListColor;
	Timer1SourceList.Width = 60;
	Timer1SourceList.Height = 18;
	Timer1SourceList.X = TIMER_SETS_DISCRETE_LIST_X;
	Timer1SourceList.Y = TIMER_SETS_DISCRETE_LIST_Y;
	Timer1SourceList.HiLimit =  DISCRETE_BUFFER_LENGTH;
	Timer1SourceList.LoLimit = 0;
	Timer1SourceList.TextAlignment = Centre;
	Timer1SourceList.Border = Yes;
	Timer1SourceList.Dynamic = Yes;

	Timer2SourceList.Style = &DiscretMappingListColor;
	Timer2SourceList.Width = 60;
	Timer2SourceList.Height = 18;
	Timer2SourceList.X = TIMER_SETS_DISCRETE_LIST_X;
	Timer2SourceList.Y = TIMER_SETS_DISCRETE_LIST_Y + TIME_ITEM_STEP;
	Timer2SourceList.HiLimit =  DISCRETE_BUFFER_LENGTH;
	Timer2SourceList.LoLimit = 0;
	Timer2SourceList.TextAlignment = Centre;
	Timer2SourceList.Border = Yes;
	Timer2SourceList.Dynamic = Yes;


	for(i = 0; i < DISCRETE_BUFFER_LENGTH;i++ )
	{
		STlistBoxSetListItem(__DiscreteBufferItemsLabel[i], i, &Timer1SourceList);
		STlistBoxSetListItem(__DiscreteBufferItemsLabel[i], i, &Timer2SourceList);
	}

	Timer1AlarmEnableCheckBox.OnText = __buttonLabelCheckBoxOn;
	Timer1AlarmEnableCheckBox.OffText = __buttonLabelCheckBoxOff;
	Timer1AlarmEnableCheckBox.Style = &TimersSetAlarmCheckBoxColor;
	Timer1AlarmEnableCheckBox.Width = BASE_CHECKBOX_WIDTH;
	Timer1AlarmEnableCheckBox.Height = 20;
	Timer1AlarmEnableCheckBox.X = TIMER_ALARM_ENABLE_CHECK_BOX_X;
	Timer1AlarmEnableCheckBox.Y = TIMER_ALARM_ENABLE_CHECK_BOX_Y;
	Timer1AlarmEnableCheckBox.Border = Yes;
	Timer1AlarmEnableCheckBox.Dynamic = Yes;

	Timer2AlarmEnableCheckBox.OnText = __buttonLabelCheckBoxOn;
	Timer2AlarmEnableCheckBox.OffText = __buttonLabelCheckBoxOff;
	Timer2AlarmEnableCheckBox.Style = &TimersSetAlarmCheckBoxColor;
	Timer2AlarmEnableCheckBox.Width = BASE_CHECKBOX_WIDTH;
	Timer2AlarmEnableCheckBox.Height = 20;
	Timer2AlarmEnableCheckBox.X = TIMER_ALARM_ENABLE_CHECK_BOX_X;
	Timer2AlarmEnableCheckBox.Y = TIMER_ALARM_ENABLE_CHECK_BOX_Y + TIME_ITEM_STEP;;
	Timer2AlarmEnableCheckBox.Border = Yes;
	Timer2AlarmEnableCheckBox.Dynamic = Yes;

	for(i = 0; i < TIMER_SETTINGS_FIELD_NAME_NUM; i++)
	{
		Timer1SettingsFieldName[i].Text = TimerSettingsFieldNameLabel[i];
		Timer1SettingsFieldName[i].Style = &TimerSettingsText;
		Timer1SettingsFieldName[i].Width =  TIMER_SETTINGS_FIELD_WIDTH;
		Timer1SettingsFieldName[i].Height = TIMER_SETTINGS_FIELD_HEIGHT;
		Timer1SettingsFieldName[i].X = TIMER_SETTINGS_FIELD_NAME_X	;
		Timer1SettingsFieldName[i].Y = TIMER_SETTINGS_FIELD_NAME_Y + TIMER_SETTINGS_FIELD_NAME_STEP * i;
		Timer1SettingsFieldName[i].Border = No;
		Timer1SettingsFieldName[i].TextAlignment = Left;
		Timer1SettingsFieldName[i].Dynamic = No;

		Timer2SettingsFieldName[i].Text = TimerSettingsFieldNameLabel[i];
		Timer2SettingsFieldName[i].Style = &TimerSettingsText;
		Timer2SettingsFieldName[i].Width =  TIMER_SETTINGS_FIELD_WIDTH;
		Timer2SettingsFieldName[i].Height = TIMER_SETTINGS_FIELD_HEIGHT;
		Timer2SettingsFieldName[i].X = TIMER_SETTINGS_FIELD_NAME_X	;
		Timer2SettingsFieldName[i].Y = TIMER_SETTINGS_FIELD_NAME_Y + TIMER_SETTINGS_FIELD_NAME_STEP * i + TIME_ITEM_STEP;
		Timer2SettingsFieldName[i].Border = No;
		Timer2SettingsFieldName[i].TextAlignment = Left;
		Timer2SettingsFieldName[i].Dynamic = No;

		STtextBoxInit(&Timer2SettingsFieldName[i]);
		STtextBoxInit(&Timer1SettingsFieldName[i]);
	}

	/*
	 *
	 */

	Timer1SecondSetpoint.Style = &TimerSettingsInputBoxColor;
	Timer1SecondSetpoint.Width = TIMER_SETTINGS_SETPOINT_FIELD_WIDTH;
	Timer1SecondSetpoint.Height = TIMER_SETTINGS_SETPOINT_FIELD_HEIGHT;
	Timer1SecondSetpoint.X = TIMER_SETTINGS_SETPOINT_FIELD_X + TIMER_SETTINGS_SETPOINT_FIELD_X_STEP;
	Timer1SecondSetpoint.Y = TIMER_ALARM_ENABLE_CHECK_BOX_Y;
	Timer1SecondSetpoint.HiLimit = 59;
	Timer1SecondSetpoint.LoLimit = 0;
	Timer1SecondSetpoint.TextAlignment = Centre;
	Timer1SecondSetpoint.Border = Yes;
	Timer1SecondSetpoint.Dynamic = Yes;
	Timer1SecondSetpoint.UnitsShow = Yes;
	Timer1SecondSetpoint.Units = "s";

	Timer1MinuteSetpoint.Style = &TimerSettingsInputBoxColor;
	Timer1MinuteSetpoint.Width = TIMER_SETTINGS_SETPOINT_FIELD_WIDTH;
	Timer1MinuteSetpoint.Height = TIMER_SETTINGS_SETPOINT_FIELD_HEIGHT;
	Timer1MinuteSetpoint.X = TIMER_SETTINGS_SETPOINT_FIELD_X;
	Timer1MinuteSetpoint.Y = TIMER_ALARM_ENABLE_CHECK_BOX_Y;
	Timer1MinuteSetpoint.HiLimit = 1000;
	Timer1MinuteSetpoint.LoLimit = 0;
	Timer1MinuteSetpoint.TextAlignment = Centre;
	Timer1MinuteSetpoint.Border = Yes;
	Timer1MinuteSetpoint.Dynamic = Yes;
	Timer1MinuteSetpoint.UnitsShow = Yes;
	Timer1MinuteSetpoint.Units = "m";

	Timer2SecondSetpoint.Style = &TimerSettingsInputBoxColor;
	Timer2SecondSetpoint.Width = TIMER_SETTINGS_SETPOINT_FIELD_WIDTH;
	Timer2SecondSetpoint.Height = TIMER_SETTINGS_SETPOINT_FIELD_HEIGHT;
	Timer2SecondSetpoint.X = TIMER_SETTINGS_SETPOINT_FIELD_X + TIMER_SETTINGS_SETPOINT_FIELD_X_STEP;
	Timer2SecondSetpoint.Y = TIMER_ALARM_ENABLE_CHECK_BOX_Y + TIME_ITEM_STEP;
	Timer2SecondSetpoint.HiLimit = 59;
	Timer2SecondSetpoint.LoLimit = 0;
	Timer2SecondSetpoint.TextAlignment = Centre;
	Timer2SecondSetpoint.Border = Yes;
	Timer2SecondSetpoint.Dynamic = Yes;
	Timer2SecondSetpoint.UnitsShow = Yes;
	Timer2SecondSetpoint.Units = "s";

	Timer2MinuteSetpoint.Style = &TimerSettingsInputBoxColor;
	Timer2MinuteSetpoint.Width = TIMER_SETTINGS_SETPOINT_FIELD_WIDTH;
	Timer2MinuteSetpoint.Height = TIMER_SETTINGS_SETPOINT_FIELD_HEIGHT;
	Timer2MinuteSetpoint.X = TIMER_SETTINGS_SETPOINT_FIELD_X;
	Timer2MinuteSetpoint.Y = TIMER_ALARM_ENABLE_CHECK_BOX_Y + TIME_ITEM_STEP;
	Timer2MinuteSetpoint.HiLimit = 1000;
	Timer2MinuteSetpoint.LoLimit = 0;
	Timer2MinuteSetpoint.TextAlignment = Centre;
	Timer2MinuteSetpoint.Border = Yes;
	Timer2MinuteSetpoint.Dynamic = Yes;
	Timer2MinuteSetpoint.UnitsShow = Yes;
	Timer2MinuteSetpoint.Units = "m";

	Timer1RenameButton.Text = "Rename";
	Timer1RenameButton.Style = &TimerSetButtonColor;
	Timer1RenameButton.Width = 60;
	Timer1RenameButton.Height = 20;
	Timer1RenameButton.X = TIMER_SETTINGS_RENAME_X;
	Timer1RenameButton.Y = TIMER_SETTINGS_RENAME_Y;
	Timer1RenameButton.Border = Yes;

	Timer2RenameButton.Text = "Rename";
	Timer2RenameButton.Style = &TimerSetButtonColor;
	Timer2RenameButton.Width = 60;
	Timer2RenameButton.Height = 20;
	Timer2RenameButton.X = TIMER_SETTINGS_RENAME_X;
	Timer2RenameButton.Y = TIMER_SETTINGS_RENAME_Y + TIME_ITEM_STEP;
	Timer2RenameButton.Border = Yes;

	BackToModelMenuButtonTimSet.Text = __buttonLabelBack;
	BackToModelMenuButtonTimSet.Style = &TimerSetButtonColor;
	BackToModelMenuButtonTimSet.Width = 60;
	BackToModelMenuButtonTimSet.Height = 26;
	BackToModelMenuButtonTimSet.X = MAIN_BACK_BUTTON_X;
	BackToModelMenuButtonTimSet.Y = MAIN_BACK_BUTTON_Y;
	BackToModelMenuButtonTimSet.Border = Yes;

	STbuttonInit(&Timer1RenameButton);
	STlistBoxInit(&Timer1SourceList);
	STcheckBoxInit(&Timer1AlarmEnableCheckBox);
	STinputBoxInit(&Timer1MinuteSetpoint);
	STinputBoxInit(&Timer1SecondSetpoint);


	STbuttonInit(&Timer2RenameButton);
	STlistBoxInit(&Timer2SourceList);
	STcheckBoxInit(&Timer2AlarmEnableCheckBox);
	STinputBoxInit(&Timer2MinuteSetpoint);
	STinputBoxInit(&Timer2SecondSetpoint);


	STtextBoxInit(&Timer1Name);
	STtextBoxInit(&Timer2Name);

	STbuttonInit(&BackToModelMenuButtonTimSet);
}
