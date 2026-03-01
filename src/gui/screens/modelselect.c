/*
 * modelselect.c
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
#include <gui/fonts/verdana_12pt.h>
#include <gui/fonts/verdana_18pt.h>
#include <gui/fonts/verdana_22pt.h>
#include <gui/fonts/verdana_26pt.h>
#include <gui/fonts/verdana_8pt.h>

#define LIST_COUNTER				MODEL_MEMORY_NUM
#define LIST_Y						38
#define LIST_STEP_Y					20
#define LIST_NUM_X					1
#define LIST_VAL_X					LIST_NUM_WIDTH + 1
#define LIST_BUTTON_X				186
#define	LIST_ITEM_HEIGHT			20
#define LIST_NUM_WIDTH				20
#define LIST_VAL_WIDTH				156
#define LIST_BUTTON_WIDTH			18
#define LIST_BUTTON_HIGHT			18


STScreenStyleTypeDef ModelSelectStyle = { 0 };
STScreenTypeDef ModelSelectST = { 0 };

STColorStyleTypeDef ModelSelectButtonColor = { 0 };		// �������� ����� ������
STColorStyleTypeDef ModelNameStyleMS = { 0 };
STtextBoxTypeDef ModelNameMS = { 0 };
STbuttonTypeDef BackToModelMenuButtonMS = { 0 };

STColorStyleTypeDef ListNumStyle = { 0 };
STtextBoxTypeDef ListNum[LIST_COUNTER] = { 0 };
STtextBoxTypeDef ListVal[LIST_COUNTER] = { 0 };
STbuttonTypeDef ListButton[LIST_COUNTER] = { 0 };
STbuttonTypeDef ListButtonRename[LIST_COUNTER] = { 0 };
STbuttonTypeDef ListButtonCopy[LIST_COUNTER] = { 0 };

typedef enum{
				__Load = 1,
				__Rename,
				__Copy
}ModelSelectScreenListEnum;

void _ModelSelect() {
	uint16_t i = 0;

	static uint16_t wait_flag[LIST_COUNTER] = { 0 };
	static uint16_t rename_wait_flag[LIST_COUNTER] = { 0 };

	if (STscreenShowNow(&ModelSelectST)) {
		STtextBoxSetValue(ModelSettings[STgetCurrentModelID()].Name, &ModelNameMS);

		if (STbuttonPressed(&BackToModelMenuButtonMS)) {
			STappSetScreen(ModelMenu, &STApp);

			STrequestSettingsSave();
		}

		for (i = 0; i < LIST_COUNTER; i++) {
			STtextBoxSetValue(ModelSettings[i + 1].Name, &ListVal[i]);

			if (wait_flag[i] != 0) {
				 {

						if (STappGetAckState(&STApp)){

							CommonSettings.CurrentModelID = i + 1;
							STloadProfile(&ModelSettings[CommonSettings.CurrentModelID]);

						} else {
							wait_flag[i] = No;
						}
				 }
			}

			 if (rename_wait_flag[i] != 0)
			 {

					if (STappGetTextBuffState(&STApp) == Yes) {

						strlcpy(ModelSettings[i + 1].Name, STappGetInputTextBuff(&STApp),	MAX_RC_NAME);

						STappClearBuff(&STApp);

						STrequestSettingsSave();

						rename_wait_flag[i] = 0;
					}
			 }


			if (STbuttonPressed(&ListButton[i])) {
				wait_flag[i] = Yes;
				STappSetScreen(AckScreen, &STApp);
			}

			if (STbuttonPressed(&ListButtonRename[i])) {
				rename_wait_flag[i] = Yes;
//				STappSetScreen(AckScreen, &STApp);

				STappSetScreen(Keyboard, &STApp);
			}

			if (STbuttonPressed(&ListButtonCopy[i])) {
				STappSetScreen(AckScreen, &STApp);
			}
		}
	}

	STscreenShow(&ModelSelectST);

	STbuttonShow(&BackToModelMenuButtonMS);

	for (i = 0; i < LIST_COUNTER; i++) {
		STtextBoxShow(&ListNum[i]);
		STtextBoxShow(&ListVal[i]);

		STbuttonShow(&ListButtonCopy[i]);
		STbuttonShow(&ListButtonRename[i]);
		STbuttonShow(&ListButton[i]);
	}
	STtextBoxShow(&ModelNameMS);
}
void _ModelSelectInit() {
	uint16_t i = 0;
	char valNum[32] = { 0 };

	/*
	 *  ������������� �������� ������
	 */
	ModelSelectStyle.HeaderFont = &verdana_8pt_bold;
	ModelSelectStyle.BackgroundColor = MAIN_SCREEN_BACK_COLOR;
	ModelSelectStyle.HeaderColor = MAIN_HEADER_COLOR;
	ModelSelectStyle.HeaderTextColor = MAIN_HEADER_TEXT_COLOR;
	ModelSelectStyle.HeaderHeight = MAIN_HEADER_HEIGHT;

	ModelSelectST.App = &STApp;
	ModelSelectST.Style = &ModelSelectStyle;
	ModelSelectST.ScreenID = ModelSelect;
	ModelSelectST.FullScreen = Yes;
	ModelSelectST.Width = GUI_MAX_WIDTH_PX;
	ModelSelectST.Height = GUI_MAX_HEIGHT_PX;
	ModelSelectST.Header = Yes;
	ModelSelectST.HeaderText = "MODEL SELECT";
	ModelSelectST.Footer = No;

	/*
	 *  ����� � ����� ����������
	 */
	ModelSelectButtonColor.Font = &verdana_8pt_bold;
	ModelSelectButtonColor.ParentScreen = &ModelSelectST;
	ModelSelectButtonColor.Color = BASE_BUTTON_COLOR;
	ModelSelectButtonColor.TextColor = BASE_BUTTON_COLOR;
	ModelSelectButtonColor.BackgroundColor = BASE_BUTTON_BACK_COLOR;
	ModelSelectButtonColor.BorderThickness = BASE_BUTTON_BORDER_THICKNESS;
	ModelSelectButtonColor.BorderColor = BASE_BUTTON_BORDER_COLOR;
	ModelSelectButtonColor.FocusColor = FOCUS_BUTTON_COLOR;
	ModelSelectButtonColor.FocusBackgroundColor = FOCUS_BUTTON_BACK_COLOR;
	ModelSelectButtonColor.FocusBorderThickness = FOCUS_BUTTON_BORDER_THICKNESS;
	ModelSelectButtonColor.FocusBorderColor = FOCUS_BUTTON_BORDER_COLOR;

	ListNumStyle.ParentScreen = &ModelSelectST;
	ListNumStyle.Font = &verdana_8pt_bold;
	ListNumStyle.Color = BASE_TEXT_COLOR;
	ListNumStyle.TextColor = BASE_TEXT_COLOR;
	ListNumStyle.BackgroundColor = BASE_TEXT_BACK_COLOR;
	ListNumStyle.BorderThickness = BASE_TEXT_BORDER_THICKNESS;
	ListNumStyle.BorderColor = BASE_TEXT_BORDER_COLOR;

	ModelNameStyleMS.ParentScreen = &ModelSelectST;
	ModelNameStyleMS.Font = &verdana_12pt_bold;
	ModelNameStyleMS.Color = MODELNAME_COLOR;
	ModelNameStyleMS.TextColor = MODELNAME_COLOR;
	ModelNameStyleMS.BackgroundColor = BASE_TEXT_BACK_COLOR;
	ModelNameStyleMS.BorderThickness = BASE_TEXT_BORDER_THICKNESS;
	ModelNameStyleMS.BorderColor = BASE_TEXT_BORDER_COLOR;

	ModelNameMS.Text = "-----";
	ModelNameMS.Style = &ModelNameStyleMS;
	ModelNameMS.Width = 200;
	ModelNameMS.Height = 18;
	ModelNameMS.X = MODEL_NAME_X;
	ModelNameMS.Y = MODEL_NAME_Y;
	ModelNameMS.Border = No;
	ModelNameMS.TextAlignment = Left;
	ModelNameMS.Dynamic = No;

	BackToModelMenuButtonMS.Text = __buttonLabelBack;
	BackToModelMenuButtonMS.Style = &ModelSelectButtonColor;
	BackToModelMenuButtonMS.Width = 60;
	BackToModelMenuButtonMS.Height = 26;
	BackToModelMenuButtonMS.X = MAIN_BACK_BUTTON_X;
	BackToModelMenuButtonMS.Y = MAIN_BACK_BUTTON_Y;
	BackToModelMenuButtonMS.Border = Yes;

	for (i = 0; i < LIST_COUNTER; i++) {
		snprintf(valNum, 8, "%d.", i + 1);

		ListNum[i].Text = valNum;
		ListNum[i].Style = &ListNumStyle;
		ListNum[i].Width = LIST_NUM_WIDTH;
		ListNum[i].Height = LIST_ITEM_HEIGHT;
		ListNum[i].X = LIST_NUM_X;
		ListNum[i].Y = LIST_Y + LIST_STEP_Y * i;
		ListNum[i].Border = No;
		ListNum[i].TextAlignment = Left;
		ListNum[i].Dynamic = No;

		ListVal[i].Text = "---";
		ListVal[i].Style = &ListNumStyle;
		ListVal[i].Width = LIST_VAL_WIDTH;
		ListVal[i].Height = LIST_ITEM_HEIGHT;
		ListVal[i].X = LIST_VAL_X;
		ListVal[i].Y = LIST_Y + LIST_STEP_Y * i;
		ListVal[i].Border = No;
		ListVal[i].TextAlignment = Left;
		ListVal[i].Dynamic = No;

		ListButton[i].Text = "L";
		ListButton[i].Style = &ModelSelectButtonColor;
		ListButton[i].Width = LIST_BUTTON_WIDTH;
		ListButton[i].Height = LIST_BUTTON_HIGHT;
		ListButton[i].X = LIST_BUTTON_X;
		ListButton[i].Y = LIST_Y + LIST_STEP_Y * i;
		ListButton[i].Border = Yes;

		ListButtonRename[i].Text = "N";
		ListButtonRename[i].Style = &ModelSelectButtonColor;
		ListButtonRename[i].Width = LIST_BUTTON_WIDTH;
		ListButtonRename[i].Height = LIST_BUTTON_HIGHT;
		ListButtonRename[i].X = LIST_BUTTON_X + LIST_BUTTON_WIDTH;
		ListButtonRename[i].Y = LIST_Y + LIST_STEP_Y * i ;
		ListButtonRename[i].Border = Yes;

		ListButtonCopy[i].Text = "C";
		ListButtonCopy[i].Style = &ModelSelectButtonColor;
		ListButtonCopy[i].Width = LIST_BUTTON_WIDTH;
		ListButtonCopy[i].Height = LIST_BUTTON_HIGHT;
		ListButtonCopy[i].X = LIST_BUTTON_X + LIST_BUTTON_WIDTH * 2;
		ListButtonCopy[i].Y = LIST_Y + LIST_STEP_Y * i ;
		ListButtonCopy[i].Border = Yes;

		STtextBoxInit(&ListNum[i]);
		STtextBoxInit(&ListVal[i]);

		STbuttonInit(&ListButton[i]);
		STbuttonInit(&ListButtonRename[i]);
		STbuttonInit(&ListButtonCopy[i]);

	}

	STtextBoxInit(&ModelNameMS);

	STbuttonInit(&BackToModelMenuButtonMS);
}
