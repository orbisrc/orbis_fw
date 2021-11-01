/*
 * info.c
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

#include <stdio.h>
#include <string.h>
#include "gui/screens.h"
#include "gui/stgui.h"
#include "gui/stapp.h"
#include <core/analog.h>
#include <gui/fonts/verdana_12pt.h>
#include <gui/fonts/verdana_18pt.h>
#include <gui/fonts/verdana_22pt.h>
#include <gui/fonts/verdana_26pt.h>
#include <gui/fonts/verdana_8pt.h>
#include "core/auxiliary.h"
#include "core/filter.h"
#include "core/iosettings.h"
#include "core/common.h"
#include "periphery/sdio.h"

#define INFO_STRING_NUM				8

#define INFO_NAME_X					4
#define INFO_VALUE_X				100
#define INFO_LIST_X					1
#define	INFO_LIST_X_STEP			120
#define INFO_LIST_Y					40
#define	INFO_LIST_Y_STEP			20

#define	___RESET_ACK_WAIT			1
#define	___DFU_ACK_WAIT				2

char __InfoStringLabel[INFO_STRING_NUM][TEXT_BUFF_SIZE] = { 	"HW version:",
																"SW version:",
																"CPU ID",
																"Date:",
																"Main freq",
																"ADC freq",
																"RC freq",
																"SD Card SN",
													//			"RC loop",
													//			"ADC loop"

																};


char	__InfoStringValue[INFO_STRING_NUM][TEXT_BUFF_SIZE] = { 	HW_VERSION,
																SW_VERSION,
																"none",
																SW_DATE

																};

STScreenStyleTypeDef InfoStyle = { 0 };
STScreenTypeDef InfoST = { 0 };

STColorStyleTypeDef InfoButtonColor = { 0 };
STbuttonTypeDef BackToModelMenuButtonInfo = { 0 };
STbuttonTypeDef DFUreqMenuButtonInfo = { 0 };
STbuttonTypeDef FullResetMenuButtonInfo = { 0 };
STColorStyleTypeDef InfoTextColor = { 0 };
STtextBoxTypeDef InfoValue[INFO_STRING_NUM] = { 0 };
STtextBoxTypeDef InfoName[INFO_STRING_NUM] = { 0 };

void _Info() {
	uint16_t i = 0;

	static uint16_t AckWait = 0;



	if (STscreenShowNow(&InfoST)) {

		snprintf(__InfoStringValue[2], TEXT_BUFF_SIZE, "%lx-%lx", HAL_GetUIDw0(), HAL_GetUIDw1());
		snprintf(__InfoStringValue[4], TEXT_BUFF_SIZE, "%.2f Hz", CommonLoopGetFreq(&MainLoop));
		snprintf(__InfoStringValue[5], TEXT_BUFF_SIZE, "%.2f Hz", CommonLoopGetFreq(&ADCloop));
		snprintf(__InfoStringValue[6], TEXT_BUFF_SIZE, "%.2f Hz", CommonLoopGetFreq(&RCchLoop));
		snprintf(__InfoStringValue[7], TEXT_BUFF_SIZE, "%s %d %lu", (char *)&SDCardCID.OEM_AppliID, SDCardCID.ProdRev, SDCardCID.ProdSN);

		/*
		 * ������� �� ���������� �����
		 */
		if (STbuttonPressed(&BackToModelMenuButtonInfo)) {
			STappSetScreen(SystemMenu, &STApp);
		}

		if (STbuttonPressed(&DFUreqMenuButtonInfo)) {

			STappSetScreen(AckScreen, &STApp);
			AckWait = ___DFU_ACK_WAIT;

		}

		for (i = 0; i < INFO_STRING_NUM; i++)
		{
			STtextBoxSetValue(__InfoStringValue[i], &InfoValue[i]);
		}

		/*
		 * Reset
		 */

		if (STbuttonPressed(&FullResetMenuButtonInfo)) {
			STappSetScreen(AckScreen, &STApp);

			AckWait = ___RESET_ACK_WAIT;

		}

		if (STappGetAckState(&STApp)) {

			switch(AckWait)
			{
			case ___RESET_ACK_WAIT:

				AckWait = 0;
				AllReset();

				break;

			case ___DFU_ACK_WAIT:

				AckWait = 0;
				rebootToBootloader();
				break;

			default:
				break;
			}

		}

	}

	STscreenShow(&InfoST);

	if (STscreenOnShow(&InfoST)) {

	}

	for (i = 0; i < INFO_STRING_NUM; i++)
	{
		STtextBoxShow(&InfoName[i]);
		STtextBoxShow(&InfoValue[i]);
	}

	STbuttonShow(&FullResetMenuButtonInfo);
	STbuttonShow(&DFUreqMenuButtonInfo);
	STbuttonShow(&BackToModelMenuButtonInfo);
}
void _InfoInit() {

	uint16_t i=0;

	/*
	 *  ������������� �������� ������
	 */
	InfoStyle.HeaderFont = &verdana_8pt_bold;
	InfoStyle.BackgroundColor = MAIN_SCREEN_BACK_COLOR;
	InfoStyle.HeaderColor = MAIN_HEADER_COLOR;
	InfoStyle.HeaderTextColor = MAIN_HEADER_TEXT_COLOR;
	InfoStyle.HeaderHeight = MAIN_HEADER_HEIGHT;

	InfoST.App = &STApp;
	InfoST.Style = &InfoStyle;
	InfoST.ScreenID = SystemInfo;
	InfoST.FullScreen = Yes;
	InfoST.Width = GUI_MAX_WIDTH_PX;
	InfoST.Height = GUI_MAX_HEIGHT_PX;
	InfoST.Header = Yes;
	InfoST.HeaderText = "INFO";
	InfoST.Footer = No;

	/*
	 *  ����� � ����� ����������
	 */
	InfoButtonColor.Font = &verdana_8pt_bold;
	InfoButtonColor.ParentScreen = &InfoST;
	InfoButtonColor.Color = BASE_BUTTON_COLOR;
	InfoButtonColor.TextColor = BASE_BUTTON_COLOR;
	InfoButtonColor.BackgroundColor = BASE_BUTTON_BACK_COLOR;
	InfoButtonColor.BorderThickness = BASE_BUTTON_BORDER_THICKNESS;
	InfoButtonColor.BorderColor = BASE_BUTTON_BORDER_COLOR;
	InfoButtonColor.FocusColor = FOCUS_BUTTON_COLOR;
	InfoButtonColor.FocusBackgroundColor = FOCUS_BUTTON_BACK_COLOR;
	InfoButtonColor.FocusBorderThickness = FOCUS_BUTTON_BORDER_THICKNESS;
	InfoButtonColor.FocusBorderColor = FOCUS_BUTTON_BORDER_COLOR;

	InfoTextColor.ParentScreen = &InfoST;
	InfoTextColor.Font = &verdana_8pt_bold;
	InfoTextColor.Color = BASE_TEXT_COLOR;
	InfoTextColor.TextColor = BASE_TEXT_COLOR;
	InfoTextColor.BackgroundColor = BASE_TEXT_BACK_COLOR;
	InfoTextColor.BorderThickness = BASE_TEXT_BORDER_THICKNESS;
	InfoTextColor.BorderColor = BASE_TEXT_BORDER_COLOR;

	for (i = 0; i < INFO_STRING_NUM; i++) {

		InfoValue[i].Text = __InfoStringLabel[i];
		InfoValue[i].Style = &InfoTextColor;
		InfoValue[i].Width = 60;
		InfoValue[i].Height = 26;
		InfoValue[i].X = INFO_VALUE_X;
		InfoValue[i].Y = INFO_LIST_Y + INFO_LIST_Y_STEP * i;
		InfoValue[i].Border = No;
		InfoValue[i].TextAlignment = Left;
		InfoValue[i].Dynamic = No;

		InfoName[i].Text = __InfoStringLabel[i];
		InfoName[i].Style = &InfoTextColor;
		InfoName[i].Width = 40;
		InfoName[i].Height = 26;
		InfoName[i].X = INFO_NAME_X;
		InfoName[i].Y = INFO_LIST_Y + INFO_LIST_Y_STEP * i;
		InfoName[i].Border = No;
		InfoName[i].TextAlignment = Left;
		InfoName[i].Dynamic = No;

		STtextBoxInit(&InfoValue[i]);
		STtextBoxInit(&InfoName[i]);
	}

	FullResetMenuButtonInfo.Text = "Reset";
	FullResetMenuButtonInfo.Style = &InfoButtonColor;
	FullResetMenuButtonInfo.Width = 80;
	FullResetMenuButtonInfo.Height = 26;
	FullResetMenuButtonInfo.X = 155;
	FullResetMenuButtonInfo.Y = MAIN_BACK_BUTTON_Y - 30;
	FullResetMenuButtonInfo.Border = Yes;

	DFUreqMenuButtonInfo.Text = "DFU Mode";
	DFUreqMenuButtonInfo.Style = &InfoButtonColor;
	DFUreqMenuButtonInfo.Width = 80;
	DFUreqMenuButtonInfo.Height = 26;
	DFUreqMenuButtonInfo.X = 155;
	DFUreqMenuButtonInfo.Y = MAIN_BACK_BUTTON_Y;
	DFUreqMenuButtonInfo.Border = Yes;

	BackToModelMenuButtonInfo.Text = __buttonLabelBack;
	BackToModelMenuButtonInfo.Style = &InfoButtonColor;
	BackToModelMenuButtonInfo.Width = 60;
	BackToModelMenuButtonInfo.Height = 26;
	BackToModelMenuButtonInfo.X = MAIN_BACK_BUTTON_X;
	BackToModelMenuButtonInfo.Y = MAIN_BACK_BUTTON_Y;
	BackToModelMenuButtonInfo.Border = Yes;


	STbuttonInit(&FullResetMenuButtonInfo);
	STbuttonInit(&DFUreqMenuButtonInfo);
	STbuttonInit(&BackToModelMenuButtonInfo);
}
