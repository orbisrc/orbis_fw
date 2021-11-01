/*
 * basicsettings.c
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
#include "core/buffer.h"
#include <core/rccurve.h>
#include <gui/fonts/verdana_18pt.h>
#include <gui/fonts/verdana_22pt.h>
#include <gui/fonts/verdana_26pt.h>
#include <gui/fonts/verdana_8pt.h>
#include <gui/fonts/verdana_8pt.h>
#include "gui/stdispdriver.h"
#include "core/analog.h"
#include "rtc.h"

#define BASIC_SETTINGS_FIELD_WIDTH					70
#define BASIC_SETTINGS_FIELD_HEIGHT					25
#define BASIC_SETTINGS_FIELD_NAME_X					1
#define BASIC_SETTINGS_FIELD_NAME_Y				    25
#define BASIC_SETTINGS_FIELD_NAME_STEP				25
#define BASIC_SETTINGS_FIELD_NAME_NUM				11


#define BASIC_SETTINGS_FIELD_INPUT_WIDTH			48
#define BASIC_SETTINGS_FIELD_INPUT_HEIGHT			17
#define BASIC_SETTINGS_FIELD_INPUT_X				90


#define BASIC_SETTINGS_DATE_Y						25
#define BASIC_SETTINGS_TIME_Y						50
#define BASIC_SETTINGS_INACTIVITY_Y					75
#define BASIC_SETTINGS_INACTIVITY_DURATION_Y		100
#define BASIC_SETTINGS_SCREEN_SAVER_Y				125
#define BASIC_SETTINGS_BATTERYLOW_Y					150
#define BASIC_SETTINGS_BATTERY_ADJ_Y				175
#define BASIC_SETTINGS_BRIGHTNESS_Y					200
#define BASIC_SETTINGS_BEEPER_Y						225
#define BASIC_SETTINGS_TRIM_BEEPER_Y				250
#define BASIC_SETTINGS_USB_MODE_Y					275

STScreenStyleTypeDef BasicSettingsStyle = { 0 };
STScreenTypeDef BasicSettingsST = { 0 };
STColorStyleTypeDef BasicSettingsText = { 0 };
STColorStyleTypeDef BasicSettingsButtonColor = { 0 };
STColorStyleTypeDef BasicSettingsCheckBoxColor = { 0 };
STColorStyleTypeDef BasicSettingsInputBoxColor = { 0 };

STcheckBoxTypeDef BasicSettingsBeeperEnable = { 0 };
STcheckBoxTypeDef BasicSettingsTrimBeeperEnable = { 0 };

STlistBoxTypeDef  BasicSettingsUSBmode = { 0 };

STinputBoxTypeDef BasicSettingsInactivity = { 0 };
STinputBoxTypeDef BasicSettingsInactivityDuration = { 0 };
STinputBoxTypeDef BasicSettingsScreenSaver = { 0 };
STinputBoxTypeDef BasicSettingsBatteryLow = { 0 };
STinputBoxTypeDef BasicSettingsBatteryAdj = { 0 };
STinputBoxTypeDef BasicSettingsDispplayBrightness = { 0 };

STinputBoxTypeDef BasicSettingsSecondInput = { 0 };
STinputBoxTypeDef BasicSettingsMinuteInput = { 0 };
STinputBoxTypeDef BasicSettingsHourInput = { 0 };
STinputBoxTypeDef BasicSettingsDayInput = { 0 };
STinputBoxTypeDef BasicSettingsMonthInput = { 0 };
STinputBoxTypeDef BasicSettingsYearInput = { 0 };

STbuttonTypeDef BackToModelMenuButtonBSSTT = { 0 };


const char BasicSettingsFieldNameLabel[BASIC_SETTINGS_FIELD_NAME_NUM][16] = {	"Date",
																				"Time",
																				"Inactivity ",
																				"Inactiv scr",
																				"Screen saver",
																				"Battery low",
																				"BattAdj",
																				"Brightness",
																				"Beeper",
																				"Trim zero",
																				"USB mode"
																			};

STtextBoxTypeDef BasicSettingsFieldName[BASIC_SETTINGS_FIELD_NAME_NUM] = { 0 };

void _BasicSettings()
{
	uint16_t i = 0;

	RTC_TimeTypeDef CurrentTime = { 0 };
	RTC_DateTypeDef CurrentDate = { 0 };

	RTC_TimeTypeDef sTime = {0};
	RTC_DateTypeDef sDate = {0};

	if (STscreenShowNow(&BasicSettingsST))
	{

		if (STbuttonPressed(&BackToModelMenuButtonBSSTT))
		{
			STappSetScreen(SystemMenu, &STApp);

			STsaveSettingsToFlash();
		}


		if (STinputBoxValueChange(&BasicSettingsDispplayBrightness)) {
//			STLCDsetBrightness((uint16_t)STinputBoxGetValue(&BasicSettingsDispplayBrightness)*40);

			CommonSettings.DispplayBrightness = (uint16_t)STinputBoxGetValue(&BasicSettingsDispplayBrightness)*40;
		}

		if (STinputBoxValueChange(&BasicSettingsInactivity)) {
			CommonSettings.InactivityDuration = (uint16_t)STinputBoxGetValue(&BasicSettingsInactivity);
		}

		if (STinputBoxValueChange(&BasicSettingsInactivityDuration)) {
			CommonSettings.ScreenInactivityTime = (uint16_t)STinputBoxGetValue(&BasicSettingsInactivityDuration);
		}

		if (STinputBoxValueChange(&BasicSettingsScreenSaver)) {
			CommonSettings.ScreenSaverTime = (uint16_t)STinputBoxGetValue(&BasicSettingsScreenSaver);
		}

		if (STinputBoxValueChange(&BasicSettingsBatteryLow)) {
			AIsetL((uint16_t)STinputBoxGetValue(&BasicSettingsBatteryLow), &AnalogChannel[BatteryADC]);
		}

		if (STinputBoxValueChange(&BasicSettingsBatteryAdj)) {

			CommonSettings.BatteryAdjustment = STinputBoxGetValue(&BasicSettingsBatteryAdj);

			AIsetScaleMax((AIgetScaleMax( &AnalogChannel[BatteryADC]) + CommonSettings.BatteryAdjustment), &AnalogChannel[BatteryADC]);
		}

		if (STcheckBoxValueChange(&BasicSettingsBeeperEnable))
		{
			STappSetBeeperState(STcheckBoxGetState(&BasicSettingsBeeperEnable), &STApp);
		}

		if (STcheckBoxValueChange(&BasicSettingsTrimBeeperEnable))
		{
			CommonSettings.TrimBeeperEnable = STcheckBoxGetState(&BasicSettingsTrimBeeperEnable);
		}

		if (STlistBoxValueChange(&BasicSettingsUSBmode))
		{
			CommonSettings.USBmode = STlistBoxGetValue(&BasicSettingsUSBmode);
		}

		/*
		 * Date&time
		 */

		if (STinputBoxValueChange(&BasicSettingsYearInput)) {
			HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
			HAL_RTC_GetDate (&hrtc, &sDate, RTC_FORMAT_BIN);
			sDate.Year = (uint8_t)(STinputBoxGetValue(&BasicSettingsYearInput) - 2000);
			HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
		}

		if (STinputBoxValueChange(&BasicSettingsMonthInput)) {
			HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
			HAL_RTC_GetDate (&hrtc, &sDate, RTC_FORMAT_BIN);
			sDate.Month = (uint8_t)STinputBoxGetValue(&BasicSettingsMonthInput);
			HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
		}

		if (STinputBoxValueChange(&BasicSettingsDayInput)) {
			HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
			HAL_RTC_GetDate (&hrtc, &sDate, RTC_FORMAT_BIN);
			sDate.Date = (uint8_t)STinputBoxGetValue(&BasicSettingsDayInput);
			HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
		}

		if (STinputBoxValueChange(&BasicSettingsHourInput)) {
			HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
			HAL_RTC_GetDate (&hrtc, &sDate, RTC_FORMAT_BIN);
			sTime.Hours = (uint8_t)STinputBoxGetValue(&BasicSettingsHourInput);
			HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
		}

		if (STinputBoxValueChange(&BasicSettingsMinuteInput)) {
			HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
			HAL_RTC_GetDate (&hrtc, &sDate, RTC_FORMAT_BIN);
			sTime.Minutes = (uint8_t)STinputBoxGetValue(&BasicSettingsMinuteInput);
			HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
		}

		if (STinputBoxValueChange(&BasicSettingsSecondInput)) {
			HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
			HAL_RTC_GetDate (&hrtc, &sDate, RTC_FORMAT_BIN);
			sTime.Seconds = (uint8_t)STinputBoxGetValue(&BasicSettingsSecondInput);
			HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
		}
	}

	STscreenShow(&BasicSettingsST);

	if (STscreenOnShow(&BasicSettingsST))
	{
		STinputBoxSetValue(STLCDgetBrightness()/40, &BasicSettingsDispplayBrightness);
		STinputBoxSetValue(CommonSettings.InactivityDuration, &BasicSettingsInactivity);
		STinputBoxSetValue(CommonSettings.ScreenInactivityTime, &BasicSettingsInactivityDuration);
		STinputBoxSetValue(CommonSettings.ScreenSaverTime, &BasicSettingsScreenSaver);
		STinputBoxSetValue(AIgetL(&AnalogChannel[BatteryADC]), &BasicSettingsBatteryLow);
		STinputBoxSetValue(CommonSettings.BatteryAdjustment , &BasicSettingsBatteryAdj);
		STcheckBoxSetValue(CommonSettings.TrimBeeperEnable, &BasicSettingsTrimBeeperEnable);
		STcheckBoxSetValue(STappGetBeeperState(&STApp),	&BasicSettingsBeeperEnable);
		STcheckBoxSetValue(STappGetBeeperState(&STApp),	&BasicSettingsBeeperEnable);
		STlistBoxSetValue((uint16_t)CommonSettings.USBmode, &BasicSettingsUSBmode);

		/*
		 * Date&Time
		 */

		HAL_RTC_GetTime(&hrtc, &CurrentTime, RTC_FORMAT_BIN);
		HAL_RTC_GetDate (&hrtc, &CurrentDate, RTC_FORMAT_BIN);

		STinputBoxSetValue((uint16_t)CurrentDate.Year+2000, &BasicSettingsYearInput);
		STinputBoxSetValue((uint16_t)CurrentDate.Month, &BasicSettingsMonthInput);
		STinputBoxSetValue((uint16_t)CurrentDate.Date, &BasicSettingsDayInput);
		STinputBoxSetValue((uint16_t)CurrentTime.Hours, &BasicSettingsHourInput);
		STinputBoxSetValue((uint16_t)CurrentTime.Minutes, &BasicSettingsMinuteInput);
		STinputBoxSetValue((uint16_t)CurrentTime.Seconds, &BasicSettingsSecondInput);

	}


	for(i = 0; i < BASIC_SETTINGS_FIELD_NAME_NUM; i++)
	{
		STtextBoxShow(&BasicSettingsFieldName[i]);
	}

	/*
	 * Date&time
	 */
	STinputBoxShow(&BasicSettingsYearInput);
	STinputBoxShow(&BasicSettingsMonthInput);
	STinputBoxShow(&BasicSettingsDayInput);
	STinputBoxShow(&BasicSettingsHourInput);
	STinputBoxShow(&BasicSettingsMinuteInput);
	STinputBoxShow(&BasicSettingsSecondInput);

	STinputBoxShow(&BasicSettingsInactivity);
	STinputBoxShow(&BasicSettingsInactivityDuration);
	STinputBoxShow(&BasicSettingsScreenSaver);
	STinputBoxShow(&BasicSettingsBatteryLow);
	STinputBoxShow(&BasicSettingsBatteryAdj);
	STinputBoxShow(&BasicSettingsDispplayBrightness);
	STcheckBoxShow(&BasicSettingsBeeperEnable);
	STcheckBoxShow(&BasicSettingsTrimBeeperEnable);
	STlistBoxShow(&BasicSettingsUSBmode);
	STbuttonShow(&BackToModelMenuButtonBSSTT);

}



void _BasicSettingsInit()
{
	uint16_t i = 0;

	BasicSettingsStyle.HeaderFont = &verdana_8pt_bold;
	BasicSettingsStyle.BackgroundColor = MAIN_SCREEN_BACK_COLOR;
	BasicSettingsStyle.HeaderColor = MAIN_HEADER_COLOR;
	BasicSettingsStyle.HeaderTextColor = MAIN_HEADER_TEXT_COLOR;
	BasicSettingsStyle.HeaderHeight = MAIN_HEADER_HEIGHT;

	BasicSettingsST.App = &STApp;
	BasicSettingsST.Style = &BasicSettingsStyle;
	BasicSettingsST.ScreenID = BasicSettings;
	BasicSettingsST.FullScreen = Yes;
	BasicSettingsST.Width = GUI_MAX_WIDTH_PX;
	BasicSettingsST.Height = GUI_MAX_HEIGHT_PX;
	BasicSettingsST.Header = Yes;
	BasicSettingsST.HeaderText = "BASIC SETTINGS";
	BasicSettingsST.Footer = No;

	BasicSettingsCheckBoxColor.ParentScreen = &BasicSettingsST;
	BasicSettingsCheckBoxColor.Font = &verdana_8pt_bold;
	BasicSettingsCheckBoxColor.Color = BASE_CHECKBOX_OFF_COLOR;
	BasicSettingsCheckBoxColor.Color2 = BASE_CHECKBOX_ON_COLOR;
	BasicSettingsCheckBoxColor.TextColor = BASE_CHECKBOX_TEXT_COLOR;
	BasicSettingsCheckBoxColor.BorderThickness = BASE_CHECKBOX_BORDER_THICKNESS;
	BasicSettingsCheckBoxColor.BorderColor = BASE_CHECKBOX_BORDER_COLOR;
	BasicSettingsCheckBoxColor.FocusBorderThickness =FOCUS_CHECKBOX_BORDER_THICKNESS;
	BasicSettingsCheckBoxColor.FocusBorderColor = FOCUS_CHECKBOX_BORDER_COLOR;

	BasicSettingsText.ParentScreen = &BasicSettingsST;
	BasicSettingsText.Font = &verdana_8pt_bold;
	BasicSettingsText.Color = BASE_TEXT_COLOR;
	BasicSettingsText.TextColor = BASE_TEXT_COLOR;
	BasicSettingsText.BackgroundColor = BASE_TEXT_BACK_COLOR;
	BasicSettingsText.BorderThickness = BASE_TEXT_BORDER_THICKNESS;
	BasicSettingsText.BorderColor = BASE_TEXT_BORDER_COLOR;

	BasicSettingsButtonColor.Font = &verdana_8pt_bold;
	BasicSettingsButtonColor.ParentScreen = &BasicSettingsST;
	BasicSettingsButtonColor.Color = BASE_BUTTON_COLOR;
	BasicSettingsButtonColor.TextColor = BASE_BUTTON_COLOR;
	BasicSettingsButtonColor.BackgroundColor = BASE_BUTTON_BACK_COLOR;
	BasicSettingsButtonColor.BorderThickness = BASE_BUTTON_BORDER_THICKNESS;
	BasicSettingsButtonColor.BorderColor = BASE_BUTTON_BORDER_COLOR;
	BasicSettingsButtonColor.FocusColor = FOCUS_BUTTON_COLOR;
	BasicSettingsButtonColor.FocusBackgroundColor = FOCUS_BUTTON_BACK_COLOR;
	BasicSettingsButtonColor.FocusBorderThickness = FOCUS_BUTTON_BORDER_THICKNESS;
	BasicSettingsButtonColor.FocusBorderColor = FOCUS_BUTTON_BORDER_COLOR;

	BasicSettingsInputBoxColor.ParentScreen = &BasicSettingsST;
	BasicSettingsInputBoxColor.Font = &verdana_8pt_bold;
	BasicSettingsInputBoxColor.Color = BASE_INPUT_COLOR;
	BasicSettingsInputBoxColor.TextColor = BASE_INPUT_TEXT_COLOR;
	BasicSettingsInputBoxColor.BackgroundColor = BASE_INPUT_BACK_COLOR;
	BasicSettingsInputBoxColor.BorderThickness = BASE_INPUT_BORDER_THICKNESS;
	BasicSettingsInputBoxColor.BorderColor = BASE_INPUT_BORDER_COLOR;
	BasicSettingsInputBoxColor.FocusTextColor = FOCUS_INPUT_TEXT_COLOR;
	BasicSettingsInputBoxColor.FocusColor = FOCUS_INPUT_COLOR;
	BasicSettingsInputBoxColor.FocusBackgroundColor = FOCUS_INPUT_BACK_COLOR;
	BasicSettingsInputBoxColor.FocusBorderThickness = FOCUS_INPUT_BORDER_THICKNESS;
	BasicSettingsInputBoxColor.FocusBorderColor = FOCUS_INPUT_BORDER_COLOR;

	BasicSettingsInactivity.Style = &BasicSettingsInputBoxColor;
	BasicSettingsInactivity.Width = INPUT_BASE_WIDTH;
	BasicSettingsInactivity.Height = INPUT_BASE_HEIGHT;
	BasicSettingsInactivity.X = BASIC_SETTINGS_FIELD_INPUT_X;
	BasicSettingsInactivity.Y = BASIC_SETTINGS_INACTIVITY_Y;
	BasicSettingsInactivity.HiLimit = INACTIVITY_TIME_MAX;
	BasicSettingsInactivity.LoLimit = INACTIVITY_TIME_MIN;
	BasicSettingsInactivity.TextAlignment = Centre;
	BasicSettingsInactivity.Border = Yes;
	BasicSettingsInactivity.Dynamic = Yes;
	BasicSettingsInactivity.Units = "m";
	BasicSettingsInactivity.UnitsShow = Yes;

	BasicSettingsInactivityDuration.Style = &BasicSettingsInputBoxColor;
	BasicSettingsInactivityDuration.Width = INPUT_BASE_WIDTH;
	BasicSettingsInactivityDuration.Height = INPUT_BASE_HEIGHT;
	BasicSettingsInactivityDuration.X = BASIC_SETTINGS_FIELD_INPUT_X;
	BasicSettingsInactivityDuration.Y = BASIC_SETTINGS_INACTIVITY_DURATION_Y;
	BasicSettingsInactivityDuration.HiLimit = INACTIVITY_TIME_MAX;
	BasicSettingsInactivityDuration.LoLimit = INACTIVITY_TIME_MIN;
	BasicSettingsInactivityDuration.TextAlignment = Centre;
	BasicSettingsInactivityDuration.Border = Yes;
	BasicSettingsInactivityDuration.Dynamic = Yes;
	BasicSettingsInactivityDuration.Units = "m";
	BasicSettingsInactivityDuration.UnitsShow = Yes;

	BasicSettingsScreenSaver.Style = &BasicSettingsInputBoxColor;
	BasicSettingsScreenSaver.Width = INPUT_BASE_WIDTH;
	BasicSettingsScreenSaver.Height = INPUT_BASE_HEIGHT;
	BasicSettingsScreenSaver.X = BASIC_SETTINGS_FIELD_INPUT_X;
	BasicSettingsScreenSaver.Y = BASIC_SETTINGS_SCREEN_SAVER_Y;
	BasicSettingsScreenSaver.HiLimit = INACTIVITY_TIME_MAX;
	BasicSettingsScreenSaver.LoLimit = INACTIVITY_TIME_MIN;
	BasicSettingsScreenSaver.TextAlignment = Centre;
	BasicSettingsScreenSaver.Border = Yes;
	BasicSettingsScreenSaver.Dynamic = Yes;
	BasicSettingsScreenSaver.Units = "m";
	BasicSettingsScreenSaver.UnitsShow = Yes;


	BasicSettingsBatteryLow.Style = &BasicSettingsInputBoxColor;
	BasicSettingsBatteryLow.Width = INPUT_BASE_WIDTH;
	BasicSettingsBatteryLow.Height = INPUT_BASE_HEIGHT;
	BasicSettingsBatteryLow.X = BASIC_SETTINGS_FIELD_INPUT_X;
	BasicSettingsBatteryLow.Y = BASIC_SETTINGS_BATTERYLOW_Y;
	BasicSettingsBatteryLow.HiLimit = 400;
	BasicSettingsBatteryLow.LoLimit = 0;
	BasicSettingsBatteryLow.TextAlignment = Centre;
	BasicSettingsBatteryLow.Border = Yes;
	BasicSettingsBatteryLow.Dynamic = Yes;
	BasicSettingsBatteryLow.Units = "V";
	BasicSettingsBatteryLow.UnitsShow = Yes;
	BasicSettingsBatteryLow.AddDot = Yes;
	BasicSettingsBatteryLow.DotPosition = 1;

	BasicSettingsBatteryAdj.Style = &BasicSettingsInputBoxColor;
	BasicSettingsBatteryAdj.Width = INPUT_BASE_WIDTH;
	BasicSettingsBatteryAdj.Height = INPUT_BASE_HEIGHT;
	BasicSettingsBatteryAdj.X = BASIC_SETTINGS_FIELD_INPUT_X;
	BasicSettingsBatteryAdj.Y = BASIC_SETTINGS_BATTERY_ADJ_Y;
	BasicSettingsBatteryAdj.HiLimit = 400;
	BasicSettingsBatteryAdj.LoLimit = 0;
	BasicSettingsBatteryAdj.TextAlignment = Centre;
	BasicSettingsBatteryAdj.Border = Yes;
	BasicSettingsBatteryAdj.Dynamic = Yes;
	BasicSettingsBatteryAdj.Units = "mV";
	BasicSettingsBatteryAdj.UnitsShow = Yes;

	BasicSettingsDispplayBrightness.Style = &BasicSettingsInputBoxColor;
	BasicSettingsDispplayBrightness.Width = INPUT_BASE_WIDTH;
	BasicSettingsDispplayBrightness.Height = INPUT_BASE_HEIGHT;
	BasicSettingsDispplayBrightness.X = BASIC_SETTINGS_FIELD_INPUT_X;
	BasicSettingsDispplayBrightness.Y = BASIC_SETTINGS_BRIGHTNESS_Y;
	BasicSettingsDispplayBrightness.HiLimit = 100;
	BasicSettingsDispplayBrightness.LoLimit = 1;
	BasicSettingsDispplayBrightness.TextAlignment = Centre;
	BasicSettingsDispplayBrightness.Border = Yes;
	BasicSettingsDispplayBrightness.Dynamic = Yes;
	BasicSettingsDispplayBrightness.Units = "%";
	BasicSettingsDispplayBrightness.UnitsShow = Yes;

	BasicSettingsBeeperEnable.OnText = __buttonLabelCheckBoxOn;
	BasicSettingsBeeperEnable.OffText = __buttonLabelCheckBoxOff;
	BasicSettingsBeeperEnable.Style = &BasicSettingsCheckBoxColor;
	BasicSettingsBeeperEnable.Width = BASE_CHECKBOX_WIDTH;
	BasicSettingsBeeperEnable.Height = BASE_CHECKBOX_HEIGHT;
	BasicSettingsBeeperEnable.X = BASIC_SETTINGS_FIELD_INPUT_X;
	BasicSettingsBeeperEnable.Y = BASIC_SETTINGS_BEEPER_Y;
	BasicSettingsBeeperEnable.Border = Yes;
	BasicSettingsBeeperEnable.Dynamic = Yes;

	BasicSettingsTrimBeeperEnable.OnText = __buttonLabelCheckBoxOn;
	BasicSettingsTrimBeeperEnable.OffText = __buttonLabelCheckBoxOff;
	BasicSettingsTrimBeeperEnable.Style = &BasicSettingsCheckBoxColor;
	BasicSettingsTrimBeeperEnable.Width = BASE_CHECKBOX_WIDTH;
	BasicSettingsTrimBeeperEnable.Height = BASE_CHECKBOX_HEIGHT;
	BasicSettingsTrimBeeperEnable.X = BASIC_SETTINGS_FIELD_INPUT_X;
	BasicSettingsTrimBeeperEnable.Y = BASIC_SETTINGS_TRIM_BEEPER_Y;
	BasicSettingsTrimBeeperEnable.Border = Yes;
	BasicSettingsTrimBeeperEnable.Dynamic = Yes;

	BasicSettingsUSBmode.Style =&BasicSettingsInputBoxColor;
	BasicSettingsUSBmode.Width = 80;
	BasicSettingsUSBmode.Height = INPUT_BASE_HEIGHT;
	BasicSettingsUSBmode.X = BASIC_SETTINGS_FIELD_INPUT_X;
	BasicSettingsUSBmode.Y = BASIC_SETTINGS_USB_MODE_Y;
	BasicSettingsUSBmode.HiLimit = 3;
	BasicSettingsUSBmode.LoLimit = 0;
	BasicSettingsUSBmode.TextAlignment = Centre;
	BasicSettingsUSBmode.Border = Yes;
	BasicSettingsUSBmode.Dynamic = Yes;

	for(i = 0; i < USB_MODE_NUMDER; i++)
	{
		STlistBoxSetListItem(__USBmodeLaberl[i], i, &BasicSettingsUSBmode);
	}

	for(i = 0; i < BASIC_SETTINGS_FIELD_NAME_NUM; i++)
	{
		BasicSettingsFieldName[i].Text = BasicSettingsFieldNameLabel[i];
		BasicSettingsFieldName[i].Style = &BasicSettingsText;
		BasicSettingsFieldName[i].Width =  BASIC_SETTINGS_FIELD_WIDTH;
		BasicSettingsFieldName[i].Height = BASIC_SETTINGS_FIELD_HEIGHT;
		BasicSettingsFieldName[i].X = BASIC_SETTINGS_FIELD_NAME_X	;
		BasicSettingsFieldName[i].Y = BASIC_SETTINGS_FIELD_NAME_Y + BASIC_SETTINGS_FIELD_NAME_STEP * i;
		BasicSettingsFieldName[i].Border = No;
		BasicSettingsFieldName[i].TextAlignment = Left;
		BasicSettingsFieldName[i].Dynamic = No;

		STtextBoxInit(&BasicSettingsFieldName[i]);
	}

	BasicSettingsYearInput.Style = &BasicSettingsInputBoxColor;
	BasicSettingsYearInput.Width = BASIC_SETTINGS_FIELD_INPUT_WIDTH;
	BasicSettingsYearInput.Height = BASIC_SETTINGS_FIELD_INPUT_HEIGHT;
	BasicSettingsYearInput.X = BASIC_SETTINGS_FIELD_INPUT_X;
	BasicSettingsYearInput.Y = BASIC_SETTINGS_DATE_Y;
	BasicSettingsYearInput.HiLimit = 2099;
	BasicSettingsYearInput.LoLimit = 2000;
	BasicSettingsYearInput.TextAlignment = Centre;
	BasicSettingsYearInput.Border = Yes;
	BasicSettingsYearInput.Dynamic = Yes;
	BasicSettingsYearInput.Units = "y";
	BasicSettingsYearInput.UnitsShow = Yes;

	BasicSettingsMonthInput.Style = &BasicSettingsInputBoxColor;
	BasicSettingsMonthInput.Width = BASIC_SETTINGS_FIELD_INPUT_WIDTH;
	BasicSettingsMonthInput.Height = BASIC_SETTINGS_FIELD_INPUT_HEIGHT;
	BasicSettingsMonthInput.X = BASIC_SETTINGS_FIELD_INPUT_X + BASIC_SETTINGS_FIELD_INPUT_WIDTH;
	BasicSettingsMonthInput.Y = BASIC_SETTINGS_DATE_Y;
	BasicSettingsMonthInput.HiLimit = 12;
	BasicSettingsMonthInput.LoLimit = 1;
	BasicSettingsMonthInput.TextAlignment = Centre;
	BasicSettingsMonthInput.Border = Yes;
	BasicSettingsMonthInput.Dynamic = Yes;
	BasicSettingsMonthInput.Units = "m";
	BasicSettingsMonthInput.UnitsShow = Yes;

	BasicSettingsDayInput.Style = &BasicSettingsInputBoxColor;
	BasicSettingsDayInput.Width = BASIC_SETTINGS_FIELD_INPUT_WIDTH;
	BasicSettingsDayInput.Height = BASIC_SETTINGS_FIELD_INPUT_HEIGHT;
	BasicSettingsDayInput.X = BASIC_SETTINGS_FIELD_INPUT_X + BASIC_SETTINGS_FIELD_INPUT_WIDTH * 2;
	BasicSettingsDayInput.Y = BASIC_SETTINGS_DATE_Y;
	BasicSettingsDayInput.HiLimit = 31;
	BasicSettingsDayInput.LoLimit = 0;
	BasicSettingsDayInput.TextAlignment = Centre;
	BasicSettingsDayInput.Border = Yes;
	BasicSettingsDayInput.Dynamic = Yes;
	BasicSettingsDayInput.Units = "d";
	BasicSettingsDayInput.UnitsShow = Yes;

	BasicSettingsHourInput.Style = &BasicSettingsInputBoxColor;
	BasicSettingsHourInput.Width = BASIC_SETTINGS_FIELD_INPUT_WIDTH;
	BasicSettingsHourInput.Height = BASIC_SETTINGS_FIELD_INPUT_HEIGHT;
	BasicSettingsHourInput.X = BASIC_SETTINGS_FIELD_INPUT_X;
	BasicSettingsHourInput.Y = BASIC_SETTINGS_TIME_Y;
	BasicSettingsHourInput.HiLimit = 23;
	BasicSettingsHourInput.LoLimit = 0;
	BasicSettingsHourInput.TextAlignment = Centre;
	BasicSettingsHourInput.Border = Yes;
	BasicSettingsHourInput.Dynamic = Yes;
	BasicSettingsHourInput.Units = "h";
	BasicSettingsHourInput.UnitsShow = Yes;

	BasicSettingsMinuteInput.Style = &BasicSettingsInputBoxColor;
	BasicSettingsMinuteInput.Width = BASIC_SETTINGS_FIELD_INPUT_WIDTH;
	BasicSettingsMinuteInput.Height = BASIC_SETTINGS_FIELD_INPUT_HEIGHT;
	BasicSettingsMinuteInput.X = BASIC_SETTINGS_FIELD_INPUT_X + BASIC_SETTINGS_FIELD_INPUT_WIDTH;
	BasicSettingsMinuteInput.Y = BASIC_SETTINGS_TIME_Y;
	BasicSettingsMinuteInput.HiLimit = 59;
	BasicSettingsMinuteInput.LoLimit = 0;
	BasicSettingsMinuteInput.TextAlignment = Centre;
	BasicSettingsMinuteInput.Border = Yes;
	BasicSettingsMinuteInput.Dynamic = Yes;
	BasicSettingsMinuteInput.Units = "m";
	BasicSettingsMinuteInput.UnitsShow = Yes;

	BasicSettingsSecondInput.Style = &BasicSettingsInputBoxColor;
	BasicSettingsSecondInput.Width = BASIC_SETTINGS_FIELD_INPUT_WIDTH;
	BasicSettingsSecondInput.Height = BASIC_SETTINGS_FIELD_INPUT_HEIGHT;
	BasicSettingsSecondInput.X = BASIC_SETTINGS_FIELD_INPUT_X + BASIC_SETTINGS_FIELD_INPUT_WIDTH * 2;
	BasicSettingsSecondInput.Y = BASIC_SETTINGS_TIME_Y;
	BasicSettingsSecondInput.HiLimit = 59;
	BasicSettingsSecondInput.LoLimit = 0;
	BasicSettingsSecondInput.TextAlignment = Centre;
	BasicSettingsSecondInput.Border = Yes;
	BasicSettingsSecondInput.Dynamic = Yes;
	BasicSettingsSecondInput.Units = "s";
	BasicSettingsSecondInput.UnitsShow = Yes;

	BackToModelMenuButtonBSSTT.Text = __buttonLabelBack;
	BackToModelMenuButtonBSSTT.Style = &BasicSettingsButtonColor;
	BackToModelMenuButtonBSSTT.Width = MAIN_BACK_BUTTON_WIDTH;
	BackToModelMenuButtonBSSTT.Height = MAIN_BACK_BUTTON_HEIGHT;
	BackToModelMenuButtonBSSTT.X = MAIN_BACK_BUTTON_X;
	BackToModelMenuButtonBSSTT.Y = MAIN_BACK_BUTTON_Y;
	BackToModelMenuButtonBSSTT.Border = Yes;


	STinputBoxInit(&BasicSettingsYearInput);
	STinputBoxInit(&BasicSettingsMonthInput);
	STinputBoxInit(&BasicSettingsDayInput);
	STinputBoxInit(&BasicSettingsHourInput);
	STinputBoxInit(&BasicSettingsMinuteInput);
	STinputBoxInit(&BasicSettingsSecondInput);

	STinputBoxInit(&BasicSettingsInactivity);
	STinputBoxInit(&BasicSettingsInactivityDuration);
	STinputBoxInit(&BasicSettingsScreenSaver);
	STinputBoxInit(&BasicSettingsBatteryLow);
	STinputBoxInit(&BasicSettingsBatteryAdj);
	STinputBoxInit(&BasicSettingsDispplayBrightness);
	STcheckBoxInit(&BasicSettingsBeeperEnable);
	STcheckBoxInit(&BasicSettingsTrimBeeperEnable);
	STlistBoxInit(&BasicSettingsUSBmode);
	STbuttonInit(&BackToModelMenuButtonBSSTT);

}
