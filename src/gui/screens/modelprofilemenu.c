/*
 * modelprofilemenu.c
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
#include <gui/fonts/verdana_8pt.h>

STScreenStyleTypeDef ModelProfileStyle = { 0 };
STScreenTypeDef ModelProfileST = { 0 };

STColorStyleTypeDef ModelProfileButtonColor = { 0 };	// �������� ����� ������

STbuttonTypeDef ModelProfilesMP = { 0 };		//
STbuttonTypeDef ModelCopyMP = { 0 };		//
STbuttonTypeDef ModelNameMP = { 0 };		//
STbuttonTypeDef DataResetMP = { 0 };		//

STbuttonTypeDef BackToModelProfileButton = { 0 };		//

void _ModelProfile() {
	if (STscreenShowNow(&ModelProfileST)) {

		static uint16_t name_edit_flag = 0;

		/*
		 * ������� � �������� ����
		 */
		if (STbuttonPressed(&BackToModelProfileButton)) {
			STappSetScreen(ModelMenu, &STApp);
			/*
			 *
			 * Добавить сохранение настроек
			 *
			 */

			STrequestSettingsSave();
		}

		/*
		 * ����� ������ ������ ������� ������
		 */
		if (STbuttonPressed(&ModelProfilesMP)) {
			STappSetScreen(ModelSelect, &STApp);
		}

		/*
		 * ����������� ������� ������ � ������ �������
		 */
		if (STbuttonPressed(&ModelCopyMP)) {

		}

		/*
		 * �������������� �������� �������
		 */
		if (STbuttonPressed(&ModelNameMP)) {
			STappSetScreen(Keyboard, &STApp);
			name_edit_flag = Yes;
		}

		if (name_edit_flag && STappGetTextBuffState(&STApp) == Yes) {
			strlcpy(ModelSettings[STgetCurrentModelID()].Name, STappGetInputTextBuff(&STApp),	MAX_RC_NAME);
			STappClearBuff(&STApp);
			STrequestSettingsSave();
			name_edit_flag = No;
		}

		/*
		 * ����� �������� ������� �� ����������� ��������
		 */

		// ������ ������������� ��� ������� ������ ������
		if (STbuttonPressed(&DataResetMP)) {
			STappSetScreen(AckScreen, &STApp);
//			STmodelProfileInit();

			AllReset();
		}

		// ��� ������� ������������� ����� �������� �� ����������� ��������
		if (STappGetAckState(&STApp)) {
			STmodelProfileInit();
		}

	}

	STscreenShow(&ModelProfileST);

	STbuttonShow(&ModelProfilesMP);
	STbuttonShow(&ModelNameMP);
//	STbuttonShow(&ModelCopyMP);
	STbuttonShow(&DataResetMP);
	STbuttonShow(&BackToModelProfileButton);
}

void _ModelProfileInit() {
	ModelProfileStyle.HeaderFont = &verdana_8pt_bold;
	ModelProfileStyle.BackgroundColor = MAIN_MENU_BACK_COLOR;
	ModelProfileStyle.BorderColor = MAIN_MENU_BORDER_COLOR;
	ModelProfileStyle.BorderThickness = MAIN_MENU_BORDER_THICKNESS;
	ModelProfileStyle.HeaderColor = MAIN_MENU_HEADER_COLOR;
	ModelProfileStyle.HeaderTextColor = MAIN_MENU_HEADER_TEXT_COLOR;

	ModelProfileST.App = &STApp;
	ModelProfileST.Style = &ModelProfileStyle;
	ModelProfileST.ScreenID = ModelProfile;
	ModelProfileST.FullScreen = No;
	ModelProfileST.Width = MAIN_MENU_WIDTH;
	ModelProfileST.Height = MAIN_MENU_HEIGHT;
	ModelProfileST.Header = Yes;
	ModelProfileST.HeaderText = "MODEL PROFILES";
	ModelProfileST.Footer = No;

	ModelProfileButtonColor.Font = &verdana_8pt_bold;
	ModelProfileButtonColor.ParentScreen = &ModelProfileST;
	ModelProfileButtonColor.Color = BASE_BUTTON_COLOR;
	ModelProfileButtonColor.TextColor = BASE_BUTTON_COLOR;
	ModelProfileButtonColor.BackgroundColor = BASE_BUTTON_BACK_COLOR;
	ModelProfileButtonColor.BorderThickness = BASE_BUTTON_BORDER_THICKNESS;
	ModelProfileButtonColor.BorderColor = BASE_BUTTON_BORDER_COLOR;
	ModelProfileButtonColor.FocusColor = FOCUS_BUTTON_COLOR;
	ModelProfileButtonColor.FocusBackgroundColor = FOCUS_BUTTON_BACK_COLOR;
	ModelProfileButtonColor.FocusBorderThickness =
			FOCUS_BUTTON_BORDER_THICKNESS;
	ModelProfileButtonColor.FocusBorderColor = FOCUS_BUTTON_BORDER_COLOR;

	ModelProfilesMP.Text = "MODEL SELECT";
	ModelProfilesMP.Style = &ModelProfileButtonColor;
	ModelProfilesMP.Width = MAIN_MENU_BUTTON_WIDTH;
	ModelProfilesMP.Height = MAIN_MENU_BUTTON_HEIGHT;
	ModelProfilesMP.X = MAIN_MENU_BUTTON_X;
	ModelProfilesMP.Y = MENU_BUTTON_1_Y;
	ModelProfilesMP.Border = Yes;

	ModelNameMP.Text = "MODEL NAME";
	ModelNameMP.Style = &ModelProfileButtonColor;
	ModelNameMP.Width = MAIN_MENU_BUTTON_WIDTH;
	ModelNameMP.Height = MAIN_MENU_BUTTON_HEIGHT;
	ModelNameMP.X = MAIN_MENU_BUTTON_X;
	ModelNameMP.Y = MENU_BUTTON_2_Y;
	ModelNameMP.Border = Yes;

	ModelCopyMP.Text = "MODEL COPY";
	ModelCopyMP.Style = &ModelProfileButtonColor;
	ModelCopyMP.Width = MAIN_MENU_BUTTON_WIDTH;
	ModelCopyMP.Height = MAIN_MENU_BUTTON_HEIGHT;
	ModelCopyMP.X = MAIN_MENU_BUTTON_X;
	ModelCopyMP.Y = MENU_BUTTON_3_Y;
	ModelCopyMP.Border = Yes;

	DataResetMP.Text = "DATA RESET";
	DataResetMP.Style = &ModelProfileButtonColor;
	DataResetMP.Width = MAIN_MENU_BUTTON_WIDTH;
	DataResetMP.Height = MAIN_MENU_BUTTON_HEIGHT;
	DataResetMP.X = MAIN_MENU_BUTTON_X;
	DataResetMP.Y = MENU_BUTTON_4_Y;
	DataResetMP.Border = Yes;

	BackToModelProfileButton.Text = __buttonLabelBack;
	BackToModelProfileButton.Style = &ModelProfileButtonColor;
	BackToModelProfileButton.Width = 60;
	BackToModelProfileButton.Height = 26;
	BackToModelProfileButton.X = BACK_BUTTON_X;
	BackToModelProfileButton.Y = BACK_BUTTON_Y;
	BackToModelProfileButton.Border = Yes;

	STbuttonInit(&ModelProfilesMP);
	STbuttonInit(&ModelNameMP);
//	STbuttonInit(&ModelCopyMP);
	STbuttonInit(&DataResetMP);
	STbuttonInit(&BackToModelProfileButton);
}
