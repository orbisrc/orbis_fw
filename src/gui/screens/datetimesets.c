/*
 * endpoint.c
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
#include "gui/gui.h"
#include "gui/stapp.h"
#include <core/iosettings.h>
#include <core/rcchannel.h>
#include <gui/fonts/verdana_18pt.h>
#include <gui/fonts/verdana_22pt.h>
#include <gui/fonts/verdana_26pt.h>
#include <gui/fonts/verdana_8pt.h>
#include <gui/fonts/verdana_8pt.h>
#include "periphery/rtc.h"

#define DATE_NAME_X					4
#define DATE_SETS_INPUT_X			80

#define STET_LINE_Y					40
#define THET_LINE_Y					70
#define CH1ET_LINE_Y				100
#define CH2ET_LINE_Y				130
#define CH3ET_LINE_Y				160
#define CH4ET_LINE_Y				190
#define CH5ET_LINE_Y				220
#define CH6ET_LINE_Y				250

STScreenStyleTypeDef DateTimeSetsStyle = { 0 };
STScreenTypeDef DateTimeSetsST = { 0 };

STColorStyleTypeDef DateTimeSetsButtonColor = { 0 };		// �������� ����� ������
STColorStyleTypeDef DateTimeSetsInputBoxColor = { 0 };// �������� ����� ����� �����
STColorStyleTypeDef DateTimeSetsText = { 0 };

STbuttonTypeDef BackToModelMenuButtonDTS = { 0 };

STinputBoxTypeDef DTSSecondInput = { 0 };
STinputBoxTypeDef DTSMinuteInput = { 0 };
STinputBoxTypeDef DTSHourInput = { 0 };
STinputBoxTypeDef DTSDayInput = { 0 };
STinputBoxTypeDef DTSMonthInput = { 0 };
STinputBoxTypeDef DTSYearInput = { 0 };

STtextBoxTypeDef DTSSecond = { 0 };
STtextBoxTypeDef DTSMinute = { 0 };
STtextBoxTypeDef DTSHour = { 0 };
STtextBoxTypeDef DTSDay = { 0 };
STtextBoxTypeDef DTSMonth = { 0 };
STtextBoxTypeDef DTSYear = { 0 };

void _DateTimeSettings()
{

	RTC_TimeTypeDef CurrentTime = { 0 };
	RTC_DateTypeDef CurrentDate = { 0 };

	RTC_TimeTypeDef sTime = {0};
	RTC_DateTypeDef sDate = {0};

	if (STscreenShowNow(&DateTimeSetsST)) {
		if (STbuttonPressed(&BackToModelMenuButtonDTS)) {
			STappSetScreen(MainMenuScr, &STApp);
		}

		if (STinputBoxValueChange(&DTSYearInput)) {
			HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
			HAL_RTC_GetDate (&hrtc, &sDate, RTC_FORMAT_BIN);
			sDate.Year = (uint8_t)(STinputBoxGetValue(&DTSYearInput) - 2000);
			HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
		}

		if (STinputBoxValueChange(&DTSMonthInput)) {
			HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
			HAL_RTC_GetDate (&hrtc, &sDate, RTC_FORMAT_BIN);
			sDate.Month = (uint8_t)STinputBoxGetValue(&DTSMonthInput);
			HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
		}

		if (STinputBoxValueChange(&DTSDayInput)) {
			HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
			HAL_RTC_GetDate (&hrtc, &sDate, RTC_FORMAT_BIN);
			sDate.Date = (uint8_t)STinputBoxGetValue(&DTSDayInput);
			HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
		}

		if (STinputBoxValueChange(&DTSHourInput)) {
			HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
			HAL_RTC_GetDate (&hrtc, &sDate, RTC_FORMAT_BIN);
			sTime.Hours = (uint8_t)STinputBoxGetValue(&DTSHourInput);
			HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
		}

		if (STinputBoxValueChange(&DTSMinuteInput)) {
			HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
			HAL_RTC_GetDate (&hrtc, &sDate, RTC_FORMAT_BIN);
			sTime.Minutes = (uint8_t)STinputBoxGetValue(&DTSMinuteInput);
			HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
		}

		if (STinputBoxValueChange(&DTSSecondInput)) {
			HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
			HAL_RTC_GetDate (&hrtc, &sDate, RTC_FORMAT_BIN);
			sTime.Seconds = (uint8_t)STinputBoxGetValue(&DTSSecondInput);
			HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
		}
	}

	STscreenShow(&DateTimeSetsST);

	if (STscreenOnShow(&DateTimeSetsST))
	{
		HAL_RTC_GetTime(&hrtc, &CurrentTime, RTC_FORMAT_BIN);
		HAL_RTC_GetDate (&hrtc, &CurrentDate, RTC_FORMAT_BIN);

		STinputBoxSetValue((uint16_t)CurrentDate.Year+2000, &DTSYearInput);
		STinputBoxSetValue((uint16_t)CurrentDate.Month, &DTSMonthInput);
		STinputBoxSetValue((uint16_t)CurrentDate.Date, &DTSDayInput);
		STinputBoxSetValue((uint16_t)CurrentTime.Hours, &DTSHourInput);
		STinputBoxSetValue((uint16_t)CurrentTime.Minutes, &DTSMinuteInput);
		STinputBoxSetValue((uint16_t)CurrentTime.Seconds, &DTSSecondInput);

	}

	STtextBoxShow(&DTSYear);
	STtextBoxShow(&DTSMonth);
	STtextBoxShow(&DTSDay);
	STtextBoxShow(&DTSHour);
	STtextBoxShow(&DTSMinute);
	STtextBoxShow(&DTSSecond);

	STinputBoxShow(&DTSYearInput);
	STinputBoxShow(&DTSMonthInput);
	STinputBoxShow(&DTSDayInput);
	STinputBoxShow(&DTSHourInput);
	STinputBoxShow(&DTSMinuteInput);
	STinputBoxShow(&DTSSecondInput);

	STbuttonShow(&BackToModelMenuButtonDTS);
}
void _DateTimeSettingsInit() {
	/*
	 *  ������������� �������� ������
	 */
	DateTimeSetsStyle.HeaderFont = &verdana_8pt_bold;
	DateTimeSetsStyle.BackgroundColor = MAIN_SCREEN_BACK_COLOR;
	DateTimeSetsStyle.HeaderColor = MAIN_HEADER_COLOR;
	DateTimeSetsStyle.HeaderTextColor = MAIN_HEADER_TEXT_COLOR;
	DateTimeSetsStyle.HeaderHeight = MAIN_HEADER_HEIGHT;

	DateTimeSetsST.App = &STApp;
	DateTimeSetsST.Style = &DateTimeSetsStyle;
	DateTimeSetsST.ScreenID = DateTimeSets;
	DateTimeSetsST.FullScreen = Yes;
	DateTimeSetsST.Width = GUI_MAX_WIDTH_PX;
	DateTimeSetsST.Height = GUI_MAX_HEIGHT_PX;
	DateTimeSetsST.Header = Yes;
	DateTimeSetsST.HeaderText = "DATE & TIME";
	DateTimeSetsST.Footer = No;

	/*
	 *  ����� � ����� ����������
	 */

	DateTimeSetsButtonColor.Font = &verdana_8pt_bold;
	DateTimeSetsButtonColor.ParentScreen = &DateTimeSetsST;
	DateTimeSetsButtonColor.Color = BASE_BUTTON_COLOR;
	DateTimeSetsButtonColor.TextColor = BASE_BUTTON_COLOR;
	DateTimeSetsButtonColor.BackgroundColor = BASE_BUTTON_BACK_COLOR;
	DateTimeSetsButtonColor.BorderThickness = BASE_BUTTON_BORDER_THICKNESS;
	DateTimeSetsButtonColor.BorderColor = BASE_BUTTON_BORDER_COLOR;
	DateTimeSetsButtonColor.FocusColor = FOCUS_BUTTON_COLOR;
	DateTimeSetsButtonColor.FocusBackgroundColor = FOCUS_BUTTON_BACK_COLOR;
	DateTimeSetsButtonColor.FocusBorderThickness = FOCUS_BUTTON_BORDER_THICKNESS;
	DateTimeSetsButtonColor.FocusBorderColor = FOCUS_BUTTON_BORDER_COLOR;

	DateTimeSetsInputBoxColor.ParentScreen = &DateTimeSetsST;
	DateTimeSetsInputBoxColor.Font = &verdana_8pt_bold;
	DateTimeSetsInputBoxColor.Color = BASE_INPUT_COLOR;
	DateTimeSetsInputBoxColor.TextColor = BASE_INPUT_TEXT_COLOR;
	DateTimeSetsInputBoxColor.BackgroundColor = BASE_INPUT_BACK_COLOR;
	DateTimeSetsInputBoxColor.BorderThickness = BASE_INPUT_BORDER_THICKNESS;
	DateTimeSetsInputBoxColor.BorderColor = BASE_INPUT_BORDER_COLOR;
	DateTimeSetsInputBoxColor.FocusTextColor = FOCUS_INPUT_TEXT_COLOR;
	DateTimeSetsInputBoxColor.FocusColor = FOCUS_INPUT_COLOR;
	DateTimeSetsInputBoxColor.FocusBackgroundColor = FOCUS_INPUT_BACK_COLOR;
	DateTimeSetsInputBoxColor.FocusBorderThickness = FOCUS_INPUT_BORDER_THICKNESS;
	DateTimeSetsInputBoxColor.FocusBorderColor = FOCUS_INPUT_BORDER_COLOR;

	DateTimeSetsText.ParentScreen = &DateTimeSetsST;
	DateTimeSetsText.Font = &verdana_8pt_bold;
	DateTimeSetsText.Color = BASE_TEXT_COLOR;
	DateTimeSetsText.TextColor = BASE_TEXT_COLOR;
	DateTimeSetsText.BackgroundColor = BASE_TEXT_BACK_COLOR;
	DateTimeSetsText.BorderThickness = BASE_TEXT_BORDER_THICKNESS;
	DateTimeSetsText.BorderColor = BASE_TEXT_BORDER_COLOR;

	DTSYear.Text = "Year";
	DTSYear.Style = &DateTimeSetsText;
	DTSYear.Width = 50;
	DTSYear.Height = INPUT_BASE_HEIGHT;
	DTSYear.X = DATE_NAME_X	;
	DTSYear.Y = STET_LINE_Y;
	DTSYear.Border = No;
	DTSYear.TextAlignment = Right;
	DTSYear.Dynamic = No;

	DTSMonth.Text = "Month";
	DTSMonth.Style = &DateTimeSetsText;
	DTSMonth.Width = 50;
	DTSMonth.Height = INPUT_BASE_HEIGHT;
	DTSMonth.X = DATE_NAME_X	;
	DTSMonth.Y = THET_LINE_Y;
	DTSMonth.Border = No;
	DTSMonth.TextAlignment = Right;
	DTSMonth.Dynamic = No;

	DTSDay.Text = "Day";
	DTSDay.Style = &DateTimeSetsText;
	DTSDay.Width = 50;
	DTSDay.Height = INPUT_BASE_HEIGHT;
	DTSDay.X = DATE_NAME_X	;
	DTSDay.Y = CH1ET_LINE_Y;
	DTSDay.Border = No;
	DTSDay.TextAlignment = Right;
	DTSDay.Dynamic = No;

	DTSHour.Text = "Hour";
	DTSHour.Style = &DateTimeSetsText;
	DTSHour.Width = 50;
	DTSHour.Height = INPUT_BASE_HEIGHT;
	DTSHour.X = DATE_NAME_X	;
	DTSHour.Y = CH2ET_LINE_Y;
	DTSHour.Border = No;
	DTSHour.TextAlignment = Right;
	DTSHour.Dynamic = No;

	DTSMinute.Text = "Minute";
	DTSMinute.Style = &DateTimeSetsText;
	DTSMinute.Width = 50;
	DTSMinute.Height = INPUT_BASE_HEIGHT;
	DTSMinute.X = DATE_NAME_X	;
	DTSMinute.Y = CH3ET_LINE_Y;
	DTSMinute.Border = No;
	DTSMinute.TextAlignment = Right;
	DTSMinute.Dynamic = No;

	DTSSecond.Text = "Second";
	DTSSecond.Style = &DateTimeSetsText;
	DTSSecond.Width = 50;
	DTSSecond.Height = INPUT_BASE_HEIGHT;
	DTSSecond.X = DATE_NAME_X	;
	DTSSecond.Y = CH4ET_LINE_Y;
	DTSSecond.Border = No;
	DTSSecond.TextAlignment = Right;
	DTSSecond.Dynamic = No;

	DTSYearInput.Style = &DateTimeSetsInputBoxColor;
	DTSYearInput.Width = INPUT_BASE_WIDTH;
	DTSYearInput.Height = INPUT_BASE_HEIGHT;
	DTSYearInput.X = DATE_SETS_INPUT_X;
	DTSYearInput.Y = STET_LINE_Y;
	DTSYearInput.HiLimit = 2099;
	DTSYearInput.LoLimit = 2000;
	DTSYearInput.TextAlignment = Centre;
	DTSYearInput.Border = Yes;
	DTSYearInput.Dynamic = Yes;

	DTSMonthInput.Style = &DateTimeSetsInputBoxColor;
	DTSMonthInput.Width = INPUT_BASE_WIDTH;
	DTSMonthInput.Height = INPUT_BASE_HEIGHT;
	DTSMonthInput.X = DATE_SETS_INPUT_X;
	DTSMonthInput.Y = THET_LINE_Y;
	DTSMonthInput.HiLimit = 12;
	DTSMonthInput.LoLimit = 1;
	DTSMonthInput.TextAlignment = Centre;
	DTSMonthInput.Border = Yes;
	DTSMonthInput.Dynamic = Yes;

	DTSDayInput.Style = &DateTimeSetsInputBoxColor;
	DTSDayInput.Width = INPUT_BASE_WIDTH;
	DTSDayInput.Height = INPUT_BASE_HEIGHT;
	DTSDayInput.X = DATE_SETS_INPUT_X;
	DTSDayInput.Y = CH1ET_LINE_Y;
	DTSDayInput.HiLimit = 31;
	DTSDayInput.LoLimit = 0;
	DTSDayInput.TextAlignment = Centre;
	DTSDayInput.Border = Yes;
	DTSDayInput.Dynamic = Yes;

	DTSHourInput.Style = &DateTimeSetsInputBoxColor;
	DTSHourInput.Width = INPUT_BASE_WIDTH;
	DTSHourInput.Height = INPUT_BASE_HEIGHT;
	DTSHourInput.X = DATE_SETS_INPUT_X;
	DTSHourInput.Y = CH2ET_LINE_Y;
	DTSHourInput.HiLimit = 23;
	DTSHourInput.LoLimit = 0;
	DTSHourInput.TextAlignment = Centre;
	DTSHourInput.Border = Yes;
	DTSHourInput.Dynamic = Yes;

	DTSMinuteInput.Style = &DateTimeSetsInputBoxColor;
	DTSMinuteInput.Width = INPUT_BASE_WIDTH;
	DTSMinuteInput.Height = INPUT_BASE_HEIGHT;
	DTSMinuteInput.X = DATE_SETS_INPUT_X;
	DTSMinuteInput.Y = CH3ET_LINE_Y;
	DTSMinuteInput.HiLimit = 59;
	DTSMinuteInput.LoLimit = 0;
	DTSMinuteInput.TextAlignment = Centre;
	DTSMinuteInput.Border = Yes;
	DTSMinuteInput.Dynamic = Yes;

	DTSSecondInput.Style = &DateTimeSetsInputBoxColor;
	DTSSecondInput.Width = INPUT_BASE_WIDTH;
	DTSSecondInput.Height = INPUT_BASE_HEIGHT;
	DTSSecondInput.X = DATE_SETS_INPUT_X;
	DTSSecondInput.Y = CH4ET_LINE_Y;
	DTSSecondInput.HiLimit = 59;
	DTSSecondInput.LoLimit = 0;
	DTSSecondInput.TextAlignment = Centre;
	DTSSecondInput.Border = Yes;
	DTSSecondInput.Dynamic = Yes;

	BackToModelMenuButtonDTS.Text = __buttonLabelBack;

	BackToModelMenuButtonDTS.Style = &DateTimeSetsButtonColor;
	BackToModelMenuButtonDTS.Width = 60;
	BackToModelMenuButtonDTS.Height = 26;
	BackToModelMenuButtonDTS.X = MAIN_BACK_BUTTON_X;
	BackToModelMenuButtonDTS.Y = MAIN_BACK_BUTTON_Y;
	BackToModelMenuButtonDTS.Border = Yes;

	STtextBoxInit(&DTSYear);
	STtextBoxInit(&DTSMonth);
	STtextBoxInit(&DTSDay);
	STtextBoxInit(&DTSHour);
	STtextBoxInit(&DTSMinute);
	STtextBoxInit(&DTSSecond);

	STinputBoxInit(&DTSYearInput);
	STinputBoxInit(&DTSMonthInput);
	STinputBoxInit(&DTSDayInput);
	STinputBoxInit(&DTSHourInput);
	STinputBoxInit(&DTSMinuteInput);
	STinputBoxInit(&DTSSecondInput);


	STbuttonInit(&BackToModelMenuButtonDTS);
}
