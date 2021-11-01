/*
 * screensaver.c
 *
*	Copyright 2019 Denis Davydov
 *
 *	Licensed under the Apache License, Version 2.0 (the "License");
 *	you may not use this file except in compliance with the License.
 *	You may obtInfon a copy of the License at
 *
 *		http://www.apache.org/licenses/LICENSE-2.0
 *
 *	Unless required by applicable law or agreed to in writing, software
 *	distributed under the License is distributed on an "AS IS" BASIS,
 *	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *	See the License for the specific language governing permissions and
 *	limitations under the License.
 */

#include "gui/screens.h"

#define SCREEN_SAVER_MAX_X	200

#define SCREEN_SAVER_MAX_Y	200

STScreenStyleTypeDef ScreenSaverStyle = { 0 };
STScreenTypeDef ScreenSaverST = { 0 };


void _ScreenSaver() {

	char Time[32] = {0};

	RTC_TimeTypeDef CurrentTime = { 0 };
	RTC_DateTypeDef CurrentDate = { 0 };

	static uint32_t LastActivityTime = {0};
	static uint16_t LastEncoderCounter = {0};

	static uint8_t preMinutes = {0};
	static uint16_t Y = {0};

	if (STscreenShowNow(&ScreenSaverST)) {

		HAL_RTC_GetTime(&hrtc, &CurrentTime, RTC_FORMAT_BIN);
		HAL_RTC_GetDate (&hrtc, &CurrentDate, RTC_FORMAT_BIN);


		if (preMinutes != CurrentTime.Minutes)
		{
			if (Y > 260)
			{
				Y = 10;
			}
			else
			{
				Y += 20;
			}


			snprintf(Time, 32, "%02d:%02d", CurrentTime.Hours,	CurrentTime.Minutes);

			STFillScreen(SCREEN_SAVER_BACK_COLOR);

			STPutText(Time, &verdana_26pt,80, Y, SCREEN_SAVER_MAIN_COLOR, SCREEN_SAVER_BACK_COLOR);

			preMinutes = CurrentTime.Minutes;
		}



		if((GetBufferActiveItemNumber() != NON_CHANGE_BUFFER_ITEM) || (LastEncoderCounter != EncoderGetCounter(&Encoder)))
		{
			LastActivityTime = HAL_GetTick();

			STappSetScreen(STappGetPrevScreen(&STApp), &STApp);
		}
	}
	else
	{
		if((GetBufferActiveItemNumber() != NON_CHANGE_BUFFER_ITEM) || (LastEncoderCounter != EncoderGetCounter(&Encoder)))
		{
			LastActivityTime = HAL_GetTick();

			LastEncoderCounter = EncoderGetCounter(&Encoder);

		}

		else
		{
			if ((HAL_GetTick() - LastActivityTime) >= CommonSettings.ScreenSaverTime * 60000)
			{
				STappSetScreen(ScreenSaver, &STApp);
			}
		}
	}

	STscreenShow(&ScreenSaverST);

}

void _ScreenSaverInit() {

	ScreenSaverStyle.HeaderFont = &verdana_8pt_bold;
	ScreenSaverStyle.BackgroundColor = SCREEN_SAVER_BACK_COLOR	;
	ScreenSaverStyle.HeaderColor = SCREEN_SAVER_BACK_COLOR	;
	ScreenSaverStyle.HeaderTextColor = SCREEN_SAVER_BACK_COLOR	;
	ScreenSaverStyle.HeaderHeight = SCREEN_SAVER_BACK_COLOR	;

	ScreenSaverST.App = &STApp;
	ScreenSaverST.Style = &ScreenSaverStyle;
	ScreenSaverST.ScreenID = ScreenSaver;
	ScreenSaverST.FullScreen = Yes;
	ScreenSaverST.Width = GUI_MAX_WIDTH_PX;
	ScreenSaverST.Height = GUI_MAX_HEIGHT_PX;
	ScreenSaverST.Header = No;
	ScreenSaverST.HeaderText = "";
	ScreenSaverST.Footer = No;
}
