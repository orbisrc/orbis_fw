/*
 * mainscreen.c
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
#include "periphery/rtc.h"
#include "core/auxiliary.h"
#include <core/analog.h>
#include <core/iosettings.h>
#include <core/rcchannel.h>
#include <gui/fonts/verdana_12pt.h>
#include <gui/fonts/verdana_18pt.h>
#include <gui/fonts/verdana_22pt.h>
#include <gui/fonts/verdana_26pt.h>
#include <gui/fonts/verdana_8pt.h>
#include "core/encoder.h"
#include "core/periphery.h"
#include "w25qxx.h"
/*
 * ������������ ��������� �� ������
 */
#define CH_LINE_HEIGHT						18
#define CH_LINE_WIDTH						160
#define TRIM_LINE_WIDTH						124
#define CH1_LINE_Y							85
#define CH2_LINE_Y							110
#define CH3_LINE_Y							135
#define CH4_LINE_Y							160
#define CH_VAL_X							195
#define CH_BAR_X							33
#define CH_NAME_X							1
#define CH_TRIM_NAME_X						0
#define CH_TRIM_BAR_X						62
#define CH_TRIM_VALUE_X						200
#define LAP_NUM_Y							40
#define LAP_NUM_X							150
#define CLOCK_Y								1
#define CLOCK_X								1

/*
 * Buttons
 */
#define MENU_BUTTON_Y						293
#define MENU_BUTTON_X						0

#define	MODEL_BUTTON_X						0
#define	MODEL_BUTTON_Y						293
#define	SYSTEM_BUTTON_X						60
#define	SYSTEM_BUTTON_Y						293
#define	TIMERS_BUTTON_X						120
#define	TIMERS_BUTTON_Y						293
#define	SV_BUTTON_X							180
#define	SV_BUTTON_Y							293

/*
 * Timers
 */
#define MAIN_SCREEN_TIMER_RESET_BUTTON_X	190
#define MAIN_SCREEN_TIMER_X					45
#define MAIN_SCREEN_TIMER_Y					55
#define MAIN_SCREEN_TIMER_SETPOINT_Y		82
#define MAIN_SCREEN_TIMER_STEP				60

/*
 * Battery
 */
#define BATTERY_WIDTH						30
#define BATTERY_HEIGHT						18
#define BATTERY_X							160
#define BATTERY_Y							1
#define BATTERY_VOLTAGE_X					200

/*
 * RSSI
 */
#define RSSI_WIDTH							30
#define RSSI_HEIGHT							18
#define RSSI_X								80
#define RSSI_Y								1

/*
 * Trimmer
 */

#ifdef __ORBIS_AIR__

#define MAIN_SCREEN_TRIM_LINE_WIDTH			100
#define MAIN_SCREEN_TRIM_LINE_HEIGHT		14
#define ELE_TRIM_BAR_X						220
#define AIL_TRIM_BAR_X						130
#define RUD_TRIM_BAR_X						10
#define THR_TRIM_BAR_X						4
#define ELE_TRIM_BAR_Y						40
#define AIL_TRIM_BAR_Y						180
#define RUD_TRIM_BAR_Y						180
#define THR_TRIM_BAR_Y						40

#endif

#ifdef __ORBIS_SURFACE__

#define MAIN_SCREEN_TRIM_LINE_WIDTH			174
#define MAIN_SCREEN_TRIM_LINE_HEIGHT		20
#define ST_TRIM_BAR_X						34
#define TH_TRIM_BAR_X						34
#define ST_TRIM_BAR_Y						180
#define TH_TRIM_BAR_Y						210

#endif


#ifdef __ORBIS_AIR__
#define TRIM_NUMBER							4
#endif

#ifdef __ORBIS_SURFACE__
#define TRIM_NUMBER							2
#endif



/*
 *
 */
STScreenStyleTypeDef MainScreenStyle = { 0 };
STScreenTypeDef MainScreenST = { 0 };

STColorStyleTypeDef MainScreenClockStyle = { 0 };
STColorStyleTypeDef MainScreenText = { 0 };
STColorStyleTypeDef ChanelBarStyle = { 0 };
STColorStyleTypeDef ModelNameStyle = { 0 };
STColorStyleTypeDef MainScreenTimerStyle = { 0 };
STColorStyleTypeDef MainScreenLapListStyle = { 0 };
STColorStyleTypeDef Button = { 0 };
STColorStyleTypeDef TrimValueText = { 0 };

STtextBoxTypeDef ModelName = { 0 };
STtextBoxTypeDef MainScreenClock = { 0 };

STbuttonTypeDef MainMenuButtonMS = { 0 };
STbuttonTypeDef SystemMenuButtonMS = { 0 };
STbuttonTypeDef ModelMenuButtonMS = { 0 };
STbuttonTypeDef TimersButtonMS = { 0 };
STbuttonTypeDef ServoViewButtonMS = { 0 };

STprogressBarTypeDef MainScreenBattery = { 0 };
STtextBoxTypeDef BatteryVoltage = { 0 };
STtextBoxTypeDef RSSItextBox = { 0 };
STtextBoxTypeDef MainScreenTimer1 = { 0 };
STtextBoxTypeDef MainScreenTimer2 = { 0 };
STtextBoxTypeDef MainScreenTimer1setpoint = { 0 };
STtextBoxTypeDef MainScreenTimer2setpoint = { 0 };
STtextBoxTypeDef MainScreenTimer1name = { 0 };
STtextBoxTypeDef MainScreenTimer2name = { 0 };

#ifdef __ORBIS_AIR__
STtextBoxTypeDef ELEtrimValue = { 0 };
STtextBoxTypeDef AILtrimValue = { 0 };
STtextBoxTypeDef RUDtrimValue = { 0 };
STtextBoxTypeDef THRtrimValue = { 0 };

STtrimBarTypeDef ELEtrimBar = { 0 };
STtrimBarTypeDef AILtrimBar = { 0 };
STtrimBarTypeDef RUDtrimBar = { 0 };
STtrimBarTypeDef THRtrimBar = { 0 };
#endif

#ifdef __ORBIS_SURFACE__
STtextBoxTypeDef STtrimValue = { 0 };
STtextBoxTypeDef THtrimValue = { 0 };
STtextBoxTypeDef STtrimName = { 0 };
STtextBoxTypeDef THtrimName = { 0 };
STtrimBarTypeDef STtrimBar = { 0 };
STtrimBarTypeDef THtrimBar = { 0 };

#endif

STbuttonTypeDef Timer1ResetButton = { 0 };
STbuttonTypeDef Timer2ResetButton = { 0 };

void _MainScreen()
{
	uint16_t i = 0;


	STtrimBarTypeDef *TrimBarPtr[TRIM_NUMBER] = {0};
	STtextBoxTypeDef *TrimValuePtr[TRIM_NUMBER] = {0};

#ifdef __ORBIS_AIR__
	TrimBarPtr[0] = &AILtrimBar;
	TrimBarPtr[1] = &ELEtrimBar;
	TrimBarPtr[2] = &THRtrimBar;
	TrimBarPtr[3] = &RUDtrimBar;

	TrimValuePtr[0] = &AILtrimValue;
	TrimValuePtr[1] = &THRtrimValue;
	TrimValuePtr[2] = &ELEtrimValue;
	TrimValuePtr[3] = &RUDtrimValue;

#endif

#ifdef __ORBIS_SURFACE__
	TrimBarPtr[0] = &STtrimBar;
	TrimBarPtr[1] = &THtrimBar;

	TrimValuePtr[0] = &STtrimValue;
	TrimValuePtr[1] = &THtrimValue;
#endif


	char BateryVoltageText[TEXT_BUFF_SIZE] = { 0 };
	char TrimValue[TRIM_NUMBER][16] = { 0 };
	char Time1[16] = { 0 };

	char Timer1[TEXT_BUFF_SIZE] = { 0 };
	char Timer2[TEXT_BUFF_SIZE] = { 0 };

	char Timer1setpoint[TEXT_BUFF_SIZE] = { 0 };
	char Timer2setpoint[TEXT_BUFF_SIZE] = { 0 };

	RTC_TimeTypeDef CurrentTime = { 0 };
	RTC_DateTypeDef CurrentDate = { 0 };

	if (STscreenShowNow(&MainScreenST)) {

		/*
		 *Time
		 */
		HAL_RTC_GetTime(&hrtc, &CurrentTime, RTC_FORMAT_BIN);
		HAL_RTC_GetDate (&hrtc, &CurrentDate, RTC_FORMAT_BIN);
		snprintf(Time1, 32, "%02d:%02d", CurrentTime.Hours,	CurrentTime.Minutes);
		STtextBoxSetValue(Time1, &MainScreenClock);

		/*
		 * Timers
		 */
		snprintf(Timer1, TEXT_BUFF_SIZE, "%02d:%02d", RCStimerGetMinute(&RCTimer1), RCStimerGetSecond(&RCTimer1));
		STtextBoxSetValue(Timer1, &MainScreenTimer1);

		snprintf(Timer2, TEXT_BUFF_SIZE, "%02d:%02d", RCStimerGetMinute(&RCTimer2), RCStimerGetSecond(&RCTimer2));
		STtextBoxSetValue(Timer2, &MainScreenTimer2);

		STtextBoxSetValue(RCStimerGetName(&RCTimer1), &MainScreenTimer1name);
		STtextBoxSetValue(RCStimerGetName(&RCTimer2), &MainScreenTimer2name);

		snprintf(Timer1setpoint, TEXT_BUFF_SIZE, "%02dm:%02ds", RCStimerGetMinuteSetpoint(&RCTimer1), RCStimerGetSecondSetpoint(&RCTimer1));
		STtextBoxSetValue(Timer1setpoint, &MainScreenTimer1setpoint);

		snprintf(Timer2setpoint, TEXT_BUFF_SIZE, "%02dm:%02ds", RCStimerGetMinuteSetpoint(&RCTimer2), RCStimerGetSecondSetpoint(&RCTimer2));
		STtextBoxSetValue(Timer2setpoint, &MainScreenTimer2setpoint);


		if (STbuttonPressed(&Timer1ResetButton)) {
			RCStimerReset(&RCTimer1);
		}

		if (STbuttonPressed(&Timer2ResetButton)) {
			RCStimerReset(&RCTimer2);
		}


		/*
		 * Model name
		 */
		STtextBoxSetValue(ModelSettings[STgetCurrentModelID()].Name, &ModelName);

		/*
		 * Battery voltage
		 */
		snprintf(BateryVoltageText, TEXT_BUFF_SIZE, "%.1fV", (float)AIgetValue(&AnalogChannel[BatteryADC])/100);
		STtextBoxSetValue(BateryVoltageText, &BatteryVoltage);

		STprogressBarSetValue(AIgetValue(&AnalogChannel[BatteryADC]), &MainScreenBattery);


		/* Пипка батарейки. */
		STRectangle(BATTERY_X + BATTERY_WIDTH,
					BATTERY_Y + 4, 4, 10, 0,
					BASE_BAR_COLOR,
					BASE_BAR_COLOR);


		/*
		 * Buttons
		 */
		if (STbuttonPressed(&MainMenuButtonMS)) {
			STappSetScreen(MainMenuScr, &STApp);
		}

		if (STbuttonPressed(&ModelMenuButtonMS)) {
			STappSetScreen(ModelMenu, &STApp);
		}

		if (STbuttonPressed(&SystemMenuButtonMS)) {
			STappSetScreen(SystemMenu, &STApp);
		}
		if (STbuttonPressed(&TimersButtonMS)) {
			STappSetScreen(TimersSettings, &STApp);
		}

		if (STbuttonPressed(&ServoViewButtonMS)) {
			STappSetScreen(ServoView, &STApp);
		}




		for(i = 0; i < TRIM_NUMBER; i++)
		{
			if (STtrimBarValueChange(TrimBarPtr[i]))
			{
				RCChanelSetTrim(STtrimBarGetValue(TrimBarPtr[i]), &RCChanel[i]);

			}

			snprintf(TrimValue[i], 16, "%d", RCChanelGetTrim(&RCChanel[i]));
			STtextBoxSetValue(TrimValue[i], TrimValuePtr[i]);
		}

	}

	STscreenShow(&MainScreenST);

	if (STscreenOnShow(&MainScreenST))
	{
		for(i = 0; i < TRIM_NUMBER; i++)
		{
			STtrimBarSetValue(RCChanelGetTrim(&RCChanel[i]), TrimBarPtr[i]);
		}
	}

	STtextBoxShow(&MainScreenClock);
	STtextBoxShow(&BatteryVoltage);
	STtextBoxShow(&RSSItextBox);
	STtextBoxShow(&MainScreenTimer1);
	STtextBoxShow(&MainScreenTimer2);

	if(RCStimerGetAlarmState(&RCTimer1))
	{
		STtextBoxShow(&MainScreenTimer1setpoint);
	}

	if(RCStimerGetAlarmState(&RCTimer2))
	{
		STtextBoxShow(&MainScreenTimer2setpoint);
	}

	STtextBoxShow(&MainScreenTimer1name);
	STtextBoxShow(&MainScreenTimer2name);
	STprogressBarShow(&MainScreenBattery);
	STtextBoxShow(&ModelName);

#ifdef __ORBIS_AIR__
	STtextBoxShow(&AILtrimValue);
	STtextBoxShow(&RUDtrimValue);
	STtextBoxShow(&ELEtrimValue);
	STtextBoxShow(&THRtrimValue);

	STtrimBarShow(&RUDtrimBar);
	STtrimBarShow(&AILtrimBar);
	STtrimBarShow(&ELEtrimBar);
	STtrimBarShow(&THRtrimBar);
#endif

#ifdef __ORBIS_SURFACE__
	STtextBoxShow(&STtrimValue);
	STtextBoxShow(&THtrimValue);

	STtextBoxShow(&STtrimName);
	STtextBoxShow(&THtrimName);

	STtrimBarShow(&STtrimBar);
	STtrimBarShow(&THtrimBar);
#endif

	STbuttonShow(&Timer1ResetButton);
	STbuttonShow(&Timer2ResetButton);
//	STbuttonShow(&MainMenuButtonMS);
	STbuttonShow(&ModelMenuButtonMS);
	STbuttonShow(&SystemMenuButtonMS);
	STbuttonShow(&TimersButtonMS);
	STbuttonShow(&ServoViewButtonMS);

}

void _MainScreenInit() {

	MainScreenStyle.BackgroundColor = MAIN_SCREEN_BACK_COLOR;

	MainScreenST.App = &STApp;
	MainScreenST.Style = &MainScreenStyle;
	MainScreenST.ScreenID = MainScreenScr;
	MainScreenST.FullScreen = Yes;
	MainScreenST.Width = GUI_MAX_WIDTH_PX;
	MainScreenST.Height = GUI_MAX_HEIGHT_PX;
	MainScreenST.Header = No;
	MainScreenST.Footer = No;

	MainScreenText.Font = &verdana_8pt_bold;
	MainScreenText.ParentScreen = &MainScreenST;
	MainScreenText.Color = BASE_TEXT_COLOR;
	MainScreenText.TextColor = BASE_TEXT_COLOR;
	MainScreenText.BackgroundColor = BASE_TEXT_BACK_COLOR;
	MainScreenText.BorderThickness = BASE_TEXT_BORDER_THICKNESS;
	MainScreenText.BorderColor = BASE_TEXT_BORDER_COLOR;

	TrimValueText.Font = &verdana_12pt_bold;
	TrimValueText.ParentScreen = &MainScreenST;
	TrimValueText.Color = BASE_TEXT_COLOR;
	TrimValueText.TextColor = BASE_TEXT_COLOR;
	TrimValueText.BackgroundColor = BASE_TEXT_BACK_COLOR;
	TrimValueText.BorderThickness = BASE_TEXT_BORDER_THICKNESS;
	TrimValueText.BorderColor = BASE_TEXT_BORDER_COLOR;

	MainScreenClockStyle.Font = &verdana_12pt_bold;
	MainScreenClockStyle.ParentScreen = &MainScreenST;
	MainScreenClockStyle.Color = BASE_TEXT_COLOR;
	MainScreenClockStyle.TextColor = BASE_TEXT_COLOR;
	MainScreenClockStyle.BackgroundColor = BASE_TEXT_BACK_COLOR;
	MainScreenClockStyle.BorderThickness = BASE_TEXT_BORDER_THICKNESS;
	MainScreenClockStyle.BorderColor = BASE_TEXT_BORDER_COLOR;

	MainScreenTimerStyle.Font = &verdana_26pt;
	MainScreenTimerStyle.ParentScreen = &MainScreenST;
	MainScreenTimerStyle.Color = BASE_TEXT_COLOR;
	MainScreenTimerStyle.TextColor = BASE_TEXT_COLOR;
	MainScreenTimerStyle.BackgroundColor = BASE_TEXT_BACK_COLOR;
	MainScreenTimerStyle.BorderThickness = BASE_TEXT_BORDER_THICKNESS;
	MainScreenTimerStyle.BorderColor = BASE_TEXT_BORDER_COLOR;

	ModelNameStyle.Font = &verdana_12pt_bold;
	ModelNameStyle.ParentScreen = &MainScreenST;
	ModelNameStyle.Color = MODELNAME_COLOR;
	ModelNameStyle.TextColor = MODELNAME_COLOR;
	ModelNameStyle.BackgroundColor = BASE_TEXT_BACK_COLOR;
	ModelNameStyle.BorderThickness = BASE_TEXT_BORDER_THICKNESS;
	ModelNameStyle.BorderColor = BASE_TEXT_BORDER_COLOR;

	ChanelBarStyle.ParentScreen = &MainScreenST;
	ChanelBarStyle.Color = BASE_BAR_COLOR;
	ChanelBarStyle.Color2 = BASE_BAR_CENTER_COLOR;
	ChanelBarStyle.TextColor = BASE_TEXT_COLOR;
	ChanelBarStyle.BackgroundColor = BASE_BAR_BACK_COLOR;
	ChanelBarStyle.BorderThickness = BASE_BAR_BORDER_THICKNESS;
	ChanelBarStyle.BorderColor = BASE_BAR_BORDER_COLOR;
	ChanelBarStyle.FocusColor2 = FOCUS_BAR_CENTER_COLOR;
	ChanelBarStyle.FocusColor = FOCUS_BAR_COLOR;
	ChanelBarStyle.FocusBackgroundColor = FOCUS_BAR_BACK_COLOR;
	ChanelBarStyle.FocusBorderThickness = FOCUS_BAR_BORDER_THICKNESS;
	ChanelBarStyle.FocusBorderColor = FOCUS_BAR_BORDER_COLOR;

	Button.Font = &verdana_8pt_bold;
	Button.ParentScreen = &MainScreenST;
	Button.Color = BASE_BUTTON_COLOR;
	Button.TextColor = BASE_BUTTON_COLOR;
	Button.BackgroundColor = BASE_BUTTON_BACK_COLOR;
	Button.BorderThickness = BASE_BUTTON_BORDER_THICKNESS;
	Button.BorderColor = BASE_BUTTON_BORDER_COLOR;
	Button.FocusColor = FOCUS_BUTTON_COLOR;
	Button.FocusBackgroundColor = FOCUS_BUTTON_BACK_COLOR;
	Button.FocusBorderThickness = FOCUS_BUTTON_BORDER_THICKNESS;
	Button.FocusBorderColor = FOCUS_BUTTON_BORDER_COLOR;

	/*
	 *  ������������� �������� ����������� ��������� ������
	 */

	MainScreenClock.Style = &MainScreenClockStyle;
	MainScreenClock.Width = 60;
	MainScreenClock.Height = 18;
	MainScreenClock.X = CLOCK_X;
	MainScreenClock.Y = CLOCK_Y;
	MainScreenClock.Border = No;
	MainScreenClock.TextAlignment = Left;

	ModelName.Style = &ModelNameStyle;
	ModelName.Width = 200;
	ModelName.Height = 18;
	ModelName.X = MODEL_NAME_X;
	ModelName.Y = MODEL_NAME_Y;
	ModelName.Border = No;
	ModelName.TextAlignment = Left;
	ModelName.Dynamic = No;

	MainScreenBattery.Style = &ChanelBarStyle;
	MainScreenBattery.Width = BATTERY_WIDTH;
	MainScreenBattery.Height = BATTERY_HEIGHT;
	MainScreenBattery.X = BATTERY_X;
	MainScreenBattery.Y = BATTERY_Y;
	MainScreenBattery.HiLimit = BATTERY_VOLTAGE_BAR_MAX;
	MainScreenBattery.LoLimit = BATTERY_VOLTAGE_BAR_MIN;

	BatteryVoltage.Text = "0.0V";
	BatteryVoltage.Style = &MainScreenText;
	BatteryVoltage.Width = 30;
	BatteryVoltage.Height = 18;
	BatteryVoltage.X = BATTERY_VOLTAGE_X;
	BatteryVoltage.Y = BATTERY_Y;
	BatteryVoltage.Border = No;
	BatteryVoltage.TextAlignment = Left;
	BatteryVoltage.Dynamic = No;

	RSSItextBox.Text = "RSSI:---";
	RSSItextBox.Style = &MainScreenText;
	RSSItextBox.Width = RSSI_WIDTH;
	RSSItextBox.Height = RSSI_HEIGHT	;
	RSSItextBox.X = RSSI_X;
	RSSItextBox.Y = RSSI_Y;
	RSSItextBox.Border = No;
	RSSItextBox.TextAlignment = Left;
	RSSItextBox.Dynamic = No;

	MainScreenTimer1setpoint.Text = "00:00";
	MainScreenTimer1setpoint.Style = &MainScreenText;
	MainScreenTimer1setpoint.Width = 160;
	MainScreenTimer1setpoint.Height = 22;
	MainScreenTimer1setpoint.X = MAIN_SCREEN_TIMER_X;
	MainScreenTimer1setpoint.Y = MAIN_SCREEN_TIMER_SETPOINT_Y;
	MainScreenTimer1setpoint.Border = No;
	MainScreenTimer1setpoint.TextAlignment = Left;

	MainScreenTimer2setpoint.Text = "00:00";
	MainScreenTimer2setpoint.Style = &MainScreenText;
	MainScreenTimer2setpoint.Width = 160;
	MainScreenTimer2setpoint.Height = 22;
	MainScreenTimer2setpoint.X = MAIN_SCREEN_TIMER_X;
	MainScreenTimer2setpoint.Y = MAIN_SCREEN_TIMER_SETPOINT_Y +  MAIN_SCREEN_TIMER_STEP ;
	MainScreenTimer2setpoint.Border = No;
	MainScreenTimer2setpoint.TextAlignment = Left;

	MainScreenTimer1.Text = "00:00";
	MainScreenTimer1.Style = &MainScreenTimerStyle;
	MainScreenTimer1.Width = 160;
	MainScreenTimer1.Height = 38;
	MainScreenTimer1.X = MAIN_SCREEN_TIMER_X;
	MainScreenTimer1.Y = MAIN_SCREEN_TIMER_Y;
	MainScreenTimer1.Border = No;
	MainScreenTimer1.TextAlignment = Left;

	MainScreenTimer2.Text = "00:00";
	MainScreenTimer2.Style = &MainScreenTimerStyle;
	MainScreenTimer2.Width = 160;
	MainScreenTimer2.Height = 38;
	MainScreenTimer2.X = MAIN_SCREEN_TIMER_X;
	MainScreenTimer2.Y = MAIN_SCREEN_TIMER_Y +  MAIN_SCREEN_TIMER_STEP ;
	MainScreenTimer2.Border = No;
	MainScreenTimer2.TextAlignment = Left;

	MainScreenTimer1name.Text = "Timer1";
	MainScreenTimer1name.Style = &MainScreenText;
	MainScreenTimer1name.Width = 160;
	MainScreenTimer1name.Height = 18;
	MainScreenTimer1name.X = MAIN_SCREEN_TIMER_X;
	MainScreenTimer1name.Y = MAIN_SCREEN_TIMER_Y - 15;
	MainScreenTimer1name.Border = No;
	MainScreenTimer1name.TextAlignment = Left;

	MainScreenTimer2name.Text = "Timer2";
	MainScreenTimer2name.Style = &MainScreenText;
	MainScreenTimer2name.Width = 160;
	MainScreenTimer2name.Height = 18;
	MainScreenTimer2name.X = MAIN_SCREEN_TIMER_X;
	MainScreenTimer2name.Y = MAIN_SCREEN_TIMER_Y +  MAIN_SCREEN_TIMER_STEP - 15;
	MainScreenTimer2name.Border = No;
	MainScreenTimer2name.TextAlignment = Left;

	/*
	 * Trim bars and value
	 */


#ifdef __ORBIS_AIR__
	ELEtrimBar.Style = &ChanelBarStyle;
	ELEtrimBar.Width = MAIN_SCREEN_TRIM_LINE_HEIGHT;
	ELEtrimBar.Height = MAIN_SCREEN_TRIM_LINE_WIDTH;
	ELEtrimBar.X = ELE_TRIM_BAR_X;
	ELEtrimBar.Y = ELE_TRIM_BAR_Y;
	ELEtrimBar.HiLimit = TRIM_CH_MAX;
	ELEtrimBar.LoLimit = TRIM_CH_MIN;
	ELEtrimBar.Dynamic = Yes;
	ELEtrimBar.Orientation = Vertical;

	AILtrimBar.Style = &ChanelBarStyle;
	AILtrimBar.Width = MAIN_SCREEN_TRIM_LINE_WIDTH;
	AILtrimBar.Height = MAIN_SCREEN_TRIM_LINE_HEIGHT;
	AILtrimBar.X = AIL_TRIM_BAR_X;
	AILtrimBar.Y = AIL_TRIM_BAR_Y;
	AILtrimBar.HiLimit = TRIM_CH_MAX;
	AILtrimBar.LoLimit = TRIM_CH_MIN;
	AILtrimBar.Dynamic = Yes;

	RUDtrimBar.Style = &ChanelBarStyle;
	RUDtrimBar.Width = MAIN_SCREEN_TRIM_LINE_WIDTH;
	RUDtrimBar.Height = MAIN_SCREEN_TRIM_LINE_HEIGHT;
	RUDtrimBar.X = RUD_TRIM_BAR_X;
	RUDtrimBar.Y = RUD_TRIM_BAR_Y;
	RUDtrimBar.HiLimit = TRIM_CH_MAX;
	RUDtrimBar.LoLimit = TRIM_CH_MIN;
	RUDtrimBar.Dynamic = Yes;

	THRtrimBar.Style = &ChanelBarStyle;
	THRtrimBar.Width = MAIN_SCREEN_TRIM_LINE_HEIGHT;
	THRtrimBar.Height = MAIN_SCREEN_TRIM_LINE_WIDTH;
	THRtrimBar.X = THR_TRIM_BAR_X;
	THRtrimBar.Y = THR_TRIM_BAR_Y;
	THRtrimBar.HiLimit = TRIM_CH_MAX;
	THRtrimBar.LoLimit = TRIM_CH_MIN;
	THRtrimBar.Dynamic = Yes;
	THRtrimBar.Orientation = Vertical;

	AILtrimValue.Text = "0";
	AILtrimValue.Style = &TrimValueText;
	AILtrimValue.Width = MAIN_SCREEN_TRIM_LINE_WIDTH;
	AILtrimValue.Height = 18;
	AILtrimValue.X = AIL_TRIM_BAR_X;
	AILtrimValue.Y = AIL_TRIM_BAR_Y + 20;
	AILtrimValue.Border = No;
	AILtrimValue.TextAlignment = Centre;
	AILtrimValue.Dynamic = No;

	RUDtrimValue.Text = "0";
	RUDtrimValue.Style = &TrimValueText;
	RUDtrimValue.Width = MAIN_SCREEN_TRIM_LINE_WIDTH;
	RUDtrimValue.Height = 18;
	RUDtrimValue.X = RUD_TRIM_BAR_X;
	RUDtrimValue.Y = RUD_TRIM_BAR_Y + 20;
	RUDtrimValue.Border = No;
	RUDtrimValue.TextAlignment = Centre;
	RUDtrimValue.Dynamic = No;

	ELEtrimValue.Text = "0";
	ELEtrimValue.Style = &TrimValueText;
	ELEtrimValue.Width = 24;
	ELEtrimValue.Height = 18;
	ELEtrimValue.X = 0;
	ELEtrimValue.Y = ELE_TRIM_BAR_Y + MAIN_SCREEN_TRIM_LINE_WIDTH + 4;
	ELEtrimValue.Border = No;
	ELEtrimValue.TextAlignment = Centre;
	ELEtrimValue.Dynamic = No;

	THRtrimValue.Text = "0";
	THRtrimValue.Style = &TrimValueText;
	THRtrimValue.Width = 24;
	THRtrimValue.Height = 18;
	THRtrimValue.X = 216;
	THRtrimValue.Y = THR_TRIM_BAR_Y + MAIN_SCREEN_TRIM_LINE_WIDTH + 4;
	THRtrimValue.Border = No;
	THRtrimValue.TextAlignment = Centre;
	THRtrimValue.Dynamic = No;
#endif

#ifdef __ORBIS_SURFACE__
	STtrimBar.Style = &ChanelBarStyle;
	STtrimBar.Width = MAIN_SCREEN_TRIM_LINE_WIDTH;
	STtrimBar.Height = MAIN_SCREEN_TRIM_LINE_HEIGHT;
	STtrimBar.X = ST_TRIM_BAR_X;
	STtrimBar.Y = ST_TRIM_BAR_Y;
	STtrimBar.HiLimit = TRIM_CH_MAX;
	STtrimBar.LoLimit = TRIM_CH_MIN;
	STtrimBar.Dynamic = Yes;

	THtrimBar.Style = &ChanelBarStyle;
	THtrimBar.Width = MAIN_SCREEN_TRIM_LINE_WIDTH;
	THtrimBar.Height = MAIN_SCREEN_TRIM_LINE_HEIGHT;
	THtrimBar.X = TH_TRIM_BAR_X;
	THtrimBar.Y = TH_TRIM_BAR_Y;
	THtrimBar.HiLimit = TRIM_CH_MAX;
	THtrimBar.LoLimit = TRIM_CH_MIN;
	THtrimBar.Dynamic = Yes;

	STtrimValue.Text = "0";
	STtrimValue.Style = &TrimValueText;
	STtrimValue.Width = 24;
	STtrimValue.Height = MAIN_SCREEN_TRIM_LINE_HEIGHT;
	STtrimValue.X = ST_TRIM_BAR_X + MAIN_SCREEN_TRIM_LINE_WIDTH + 4;
	STtrimValue.Y = ST_TRIM_BAR_Y;
	STtrimValue.Border = No;
	STtrimValue.TextAlignment = Centre;
	STtrimValue.Dynamic = No;

	THtrimValue.Text = "0";
	THtrimValue.Style = &TrimValueText;
	THtrimValue.Width = 24;
	THtrimValue.Height = MAIN_SCREEN_TRIM_LINE_HEIGHT;
	THtrimValue.X = ST_TRIM_BAR_X + MAIN_SCREEN_TRIM_LINE_WIDTH + 4;
	THtrimValue.Y = TH_TRIM_BAR_Y;
	THtrimValue.Border = No;
	THtrimValue.TextAlignment = Centre;
	THtrimValue.Dynamic = No;

	STtrimName.Text = "ST";
	STtrimName.Style = &TrimValueText;
	STtrimName.Width = 24;
	STtrimName.Height = MAIN_SCREEN_TRIM_LINE_HEIGHT;
	STtrimName.X = ST_TRIM_BAR_X - 28;
	STtrimName.Y = ST_TRIM_BAR_Y;
	STtrimName.Border = No;
	STtrimName.TextAlignment = Centre;
	STtrimName.Dynamic = No;

	THtrimName.Text = "TH";
	THtrimName.Style = &TrimValueText;
	THtrimName.Width = 24;
	THtrimName.Height = MAIN_SCREEN_TRIM_LINE_HEIGHT;
	THtrimName.X = ST_TRIM_BAR_X - 28;
	THtrimName.Y = TH_TRIM_BAR_Y;
	THtrimName.Border = No;
	THtrimName.TextAlignment = Centre;
	THtrimName.Dynamic = No;
#endif



	/*
	 * Buttons
	 */

	Timer1ResetButton.Text = "R";
	Timer1ResetButton.Style = &Button;
	Timer1ResetButton.Width = 20;
	Timer1ResetButton.Height = 18;
	Timer1ResetButton.X = MAIN_SCREEN_TIMER_RESET_BUTTON_X;
	Timer1ResetButton.Y = MAIN_SCREEN_TIMER_Y;
	Timer1ResetButton.Border = Yes;

	Timer2ResetButton.Text = "R";
	Timer2ResetButton.Style = &Button;
	Timer2ResetButton.Width = 20;
	Timer2ResetButton.Height = 18;
	Timer2ResetButton.X = MAIN_SCREEN_TIMER_RESET_BUTTON_X;
	Timer2ResetButton.Y = MAIN_SCREEN_TIMER_Y + MAIN_SCREEN_TIMER_STEP;
	Timer2ResetButton.Border = Yes;

	MainMenuButtonMS.Text = "Menu";
	MainMenuButtonMS.Style = &Button;
	MainMenuButtonMS.Width = 60;
	MainMenuButtonMS.Height = 26;
	MainMenuButtonMS.X = MENU_BUTTON_X;
	MainMenuButtonMS.Y = MENU_BUTTON_Y;
	MainMenuButtonMS.Border = Yes;

	ModelMenuButtonMS.Text = "Model";
	ModelMenuButtonMS.Style = &Button;
	ModelMenuButtonMS.Width = 60;
	ModelMenuButtonMS.Height = 26;
	ModelMenuButtonMS.X = MODEL_BUTTON_X;
	ModelMenuButtonMS.Y = MODEL_BUTTON_Y;
	ModelMenuButtonMS.Border = Yes;

	SystemMenuButtonMS.Text = "System";
	SystemMenuButtonMS.Style = &Button;
	SystemMenuButtonMS.Width = 60;
	SystemMenuButtonMS.Height = 26;
	SystemMenuButtonMS.X = SYSTEM_BUTTON_X;
	SystemMenuButtonMS.Y = SYSTEM_BUTTON_Y;
	SystemMenuButtonMS.Border = Yes;

	TimersButtonMS.Text = "Timers";
	TimersButtonMS.Style = &Button;
	TimersButtonMS.Width = 60;
	TimersButtonMS.Height = 26;
	TimersButtonMS.X = TIMERS_BUTTON_X;
	TimersButtonMS.Y = TIMERS_BUTTON_Y;
	TimersButtonMS.Border = Yes;

	ServoViewButtonMS.Text = "Monitor";
	ServoViewButtonMS.Style = &Button;
	ServoViewButtonMS.Width = 60;
	ServoViewButtonMS.Height = 26;
	ServoViewButtonMS.X = SV_BUTTON_X;
	ServoViewButtonMS.Y = SV_BUTTON_Y;
	ServoViewButtonMS.Border = Yes;


	STprogressBarInit(&MainScreenBattery);
	STtextBoxInit(&MainScreenClock);
	STtextBoxInit(&ModelName);
	STtextBoxInit(&MainScreenTimer1);
	STtextBoxInit(&MainScreenTimer2);
	STtextBoxInit(&MainScreenTimer1setpoint);
	STtextBoxInit(&MainScreenTimer2setpoint);
	STtextBoxInit(&MainScreenTimer1name);
	STtextBoxInit(&MainScreenTimer2name);
	STtextBoxInit(&BatteryVoltage);
	STtextBoxInit(&RSSItextBox);

	STbuttonInit(&Timer1ResetButton);
	STbuttonInit(&Timer2ResetButton);

#ifdef __ORBIS_AIR__
	STtextBoxInit(&AILtrimValue);
	STtextBoxInit(&RUDtrimValue);
	STtextBoxInit(&ELEtrimValue);
	STtextBoxInit(&THRtrimValue);

	STtrimBarInit(&THRtrimBar);
	STtrimBarInit(&RUDtrimBar);
	STtrimBarInit(&AILtrimBar);
	STtrimBarInit(&ELEtrimBar);
#endif

#ifdef __ORBIS_SURFACE__
	STtextBoxInit(&STtrimValue);
	STtextBoxInit(&THtrimValue);
	STtextBoxInit(&STtrimName);
	STtextBoxInit(&THtrimName);
	STtrimBarInit(&STtrimBar);
	STtrimBarInit(&THtrimBar);
#endif


	STbuttonInit(&ModelMenuButtonMS);
	STbuttonInit(&ModelMenuButtonMS);
	STbuttonInit(&SystemMenuButtonMS);
	STbuttonInit(&TimersButtonMS);
	STbuttonInit(&ServoViewButtonMS);
}
