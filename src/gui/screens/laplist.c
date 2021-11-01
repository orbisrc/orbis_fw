/*
 * laplist.c
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
#include "core/auxiliary.h"

#define TOTAL_TIME_X				4
#define TOTAL_TIME_Y				20
#define LAP_LIST_X					1
#define	LAP_LIST_X_STEP				120
#define LAP_LIST_Y					40
#define	LAP_LIST_Y_STEP				20
#define RESET_BUTTON_X				179
#define RESET_BUTTON_Y				293

STScreenStyleTypeDef LapListStyle = { 0 };
STScreenTypeDef LapListST = { 0 };

STColorStyleTypeDef LapListButtonColor = { 0 };
STbuttonTypeDef BackToModelMenuButtonLL = { 0 };
STbuttonTypeDef ResetButtonLL = { 0 };

STColorStyleTypeDef LapListValueColor = { 0 };
STtextBoxTypeDef LapListValue[LAPS_NUM] = { 0 };
STtextBoxTypeDef TotalTimeLL = { 0 };

void _LapList() {
	uint16_t i = 0;
	uint16_t lapNum = 0;
	char valNum[LAPS_NUM][32] = { 0 };
//	char TotalTime[32] = { 0 };

	if (STscreenShowNow(&LapListST)) {

		/*
		 * ������� �� ���������� �����
		 */
		if (STbuttonPressed(&BackToModelMenuButtonLL)) {
			STappSetScreen(MainScreenScr, &STApp);
		}

		/*
		 * ����� �����������
		 */
		if (STbuttonPressed(&ResetButtonLL)) {
			STappSetScreen(AckScreen, &STApp);

		}

		// ��� ������� ������������� �����
		if (STappGetAckState(&STApp)) {
			RCStopwatchReset(&MainStopwatch);
		}

		/*
		 * ������ �����
		 */
//		snprintf(TotalTime, 32, "Total %02dh : %02dm : %02d : %02dm",
//				RCStopwatchGetTotalLapHour(&MainStopwatch),
//				RCStopwatchGetTotalLapMin(&MainStopwatch),
//				RCStopwatchGetTotalLapSec(&MainStopwatch),
//				RCStopwatchGetTotalLapMs(&MainStopwatch));
//
//		STtextBoxSetValue(TotalTime, &TotalTimeLL);

		/*
		 * ���������� ������
		 */
		lapNum = RCStopwatchGetLapsNumber(&MainStopwatch);

		for (i = 0; i < LAPS_NUM; i++) {
			if (i < lapNum) {
				snprintf(valNum[i], 32, "%d. %02dm:%02ds:%02dms", i + 1,
						RCStopwatchGetLapMin(i, &MainStopwatch),
						RCStopwatchGetLapSec(i, &MainStopwatch),
						RCStopwatchGetLapMs(i, &MainStopwatch));
				STtextBoxSetValue(valNum[i], &LapListValue[i]);
			} else {
				snprintf(valNum[i], 32, "%d. -----------", i + 1);
				STtextBoxSetValue(valNum[i], &LapListValue[i]);
			}
		}
	}

	STscreenShow(&LapListST);

	if (STscreenOnShow(&LapListST)) {

	}

	for (i = 0; i < LAPS_NUM; i++) {
		STtextBoxShow(&LapListValue[i]);
	}

	STtextBoxShow(&TotalTimeLL);
	STbuttonShow(&BackToModelMenuButtonLL);
	STbuttonShow(&ResetButtonLL);
}
void _LapListInit() {
	uint16_t i = 0;			// �������
	uint16_t j = 0;			// ������
	uint16_t textNum = 0;
	char valNum[32] = { 0 };

	/*
	 *  ������������� �������� ������
	 */
	LapListStyle.HeaderFont = &verdana_8pt_bold;
	LapListStyle.BackgroundColor = MAIN_SCREEN_BACK_COLOR;
	LapListStyle.HeaderColor = MAIN_HEADER_COLOR;
	LapListStyle.HeaderTextColor = MAIN_HEADER_TEXT_COLOR;
	LapListStyle.HeaderHeight = MAIN_HEADER_HEIGHT;

	LapListST.App = &STApp;
	LapListST.Style = &LapListStyle;
	LapListST.ScreenID = LapList;
	LapListST.FullScreen = Yes;
	LapListST.Width = GUI_MAX_WIDTH_PX;
	LapListST.Height = GUI_MAX_HEIGHT_PX;
	LapListST.Header = Yes;
	LapListST.HeaderText = "LAP LIST";
	LapListST.Footer = No;

	/*
	 *  ����� � ����� ����������
	 */
	LapListButtonColor.Font = &verdana_8pt_bold;
	LapListButtonColor.ParentScreen = &LapListST;
	LapListButtonColor.Color = BASE_BUTTON_COLOR;
	LapListButtonColor.TextColor = BASE_BUTTON_COLOR;
	LapListButtonColor.BackgroundColor = BASE_BUTTON_BACK_COLOR;
	LapListButtonColor.BorderThickness = BASE_BUTTON_BORDER_THICKNESS;
	LapListButtonColor.BorderColor = BASE_BUTTON_BORDER_COLOR;
	LapListButtonColor.FocusColor = FOCUS_BUTTON_COLOR;
	LapListButtonColor.FocusBackgroundColor = FOCUS_BUTTON_BACK_COLOR;
	LapListButtonColor.FocusBorderThickness = FOCUS_BUTTON_BORDER_THICKNESS;
	LapListButtonColor.FocusBorderColor = FOCUS_BUTTON_BORDER_COLOR;

	LapListValueColor.ParentScreen = &LapListST;
	LapListValueColor.Font = &verdana_8pt_bold;
	LapListValueColor.Color = BASE_TEXT_COLOR;
	LapListValueColor.TextColor = BASE_TEXT_COLOR;
	LapListValueColor.BackgroundColor = BASE_TEXT_BACK_COLOR;
	LapListValueColor.BorderThickness = BASE_TEXT_BORDER_THICKNESS;
	LapListValueColor.BorderColor = BASE_TEXT_BORDER_COLOR;

	TotalTimeLL.Style = &LapListValueColor;
	TotalTimeLL.Width = 100;
	TotalTimeLL.Height = 26;
	TotalTimeLL.X = TOTAL_TIME_X;
	TotalTimeLL.Y = TOTAL_TIME_Y;
	TotalTimeLL.Border = No;
	TotalTimeLL.TextAlignment = Left;
	TotalTimeLL.Dynamic = No;

	for (i = 0; i < ROW_NUM; i++) {
		for (j = 0; j < LINE_NUM; j++) {
			snprintf(valNum, 32, "%d. -----------", textNum + 1);

			LapListValue[textNum].Text = valNum;
			LapListValue[textNum].Style = &LapListValueColor;
			LapListValue[textNum].Width = 100;
			LapListValue[textNum].Height = 26;
			LapListValue[textNum].X = LAP_LIST_X + LAP_LIST_X_STEP * i;
			LapListValue[textNum].Y = LAP_LIST_Y + LAP_LIST_Y_STEP * j;
			LapListValue[textNum].Border = No;
			LapListValue[textNum].TextAlignment = Left;
			LapListValue[textNum].Dynamic = No;

			STtextBoxInit(&LapListValue[textNum]);

			textNum++;
		}
	}

	BackToModelMenuButtonLL.Text = __buttonLabelBack;
	BackToModelMenuButtonLL.Style = &LapListButtonColor;
	BackToModelMenuButtonLL.Width = 60;
	BackToModelMenuButtonLL.Height = 26;
	BackToModelMenuButtonLL.X = MAIN_BACK_BUTTON_X;
	BackToModelMenuButtonLL.Y = MAIN_BACK_BUTTON_Y;
	BackToModelMenuButtonLL.Border = Yes;

	ResetButtonLL.Text = "RESET";
	ResetButtonLL.Style = &LapListButtonColor;
	ResetButtonLL.Width = 60;
	ResetButtonLL.Height = 26;
	ResetButtonLL.X = RESET_BUTTON_X;
	ResetButtonLL.Y = RESET_BUTTON_Y;
	ResetButtonLL.Border = Yes;

	STtextBoxInit(&TotalTimeLL);
	STbuttonInit(&BackToModelMenuButtonLL);
	STbuttonInit(&ResetButtonLL);
}
