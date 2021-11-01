/*
 * modelmenu.c
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

#define MODEL_MENU_BUTTON_NUM			6


STScreenStyleTypeDef ModelMenuStyle = { 0 };
STScreenTypeDef ModelMenuST = { 0 };

const char __MDMbuttonLabel[MODEL_MENU_BUTTON_NUM][16] = { 		"MODEL SELECT",
																"CH SETTINGS",
																"END POINT",
																"SUB TRIM",
																"CH MAPPING",
																"CH INVERT"

																};


const uint16_t	__MDMbuttonTarget[MODEL_MENU_BUTTON_NUM] = { 	ModelSelect,
															    ChannelSettings,
																EndPoint,
																ChannelTrim,
																ChannelMapping,
																ChannelReverse,

																};

STbuttonTypeDef __MDMbutton[MODEL_MENU_BUTTON_NUM] = { 0 };

STColorStyleTypeDef ModelMenuButtonColor = { 0 };		// �������� ����� ������
STbuttonTypeDef BackToMainlMenuButton = { 0 };		//
STbuttonTypeDef NextModelMenuButton = { 0 };

//STbuttonTypeDef 		DualRateButton = {0};

void _ModelMenu()
{
	uint16_t i = 0;

	if (STscreenShowNow(&ModelMenuST)) {
		/*
		 * ������� ����� � �������� ����
		 */
		if (STbuttonPressed(&BackToMainlMenuButton)) {
			STappSetScreen(MainScreenScr, &STApp);

		//	STappSetScreen(STappGetPrevScreen(&STApp),&STApp);
			/*
			 *
			 * Добавить сохранение настроек
			 *
			 */

			STsaveSettingsToFlash();
		}
		/*
		 * ������� �� ����� ���������� ��������� �������
		 */

		for(i = 0; i < MODEL_MENU_BUTTON_NUM; i++)
		{
			if (STbuttonPressed(&__MDMbutton[i]))
			{
				STappSetScreen(__MDMbuttonTarget[i], &STApp);
			}
		}
	}

	STscreenShow(&ModelMenuST);

	for(i = 0; i < MODEL_MENU_BUTTON_NUM; i++)
	{
		STbuttonShow(&__MDMbutton[i]);
	}

	STbuttonShow(&BackToMainlMenuButton);
//	STbuttonShow(&NextModelMenuButton);

}
void _ModelMenuInit() {
	/*
	 *
	 */

	uint16_t i = 0;

	ModelMenuStyle.HeaderFont = &verdana_8pt_bold;
	ModelMenuStyle.BackgroundColor = MAIN_MENU_BACK_COLOR;
	ModelMenuStyle.BorderColor = MAIN_MENU_BORDER_COLOR;
	ModelMenuStyle.BorderThickness = MAIN_MENU_BORDER_THICKNESS;
	ModelMenuStyle.HeaderColor = MAIN_MENU_HEADER_COLOR;
	ModelMenuStyle.HeaderTextColor = MAIN_MENU_HEADER_TEXT_COLOR;

	ModelMenuST.App = &STApp;
	ModelMenuST.Style = &ModelMenuStyle;
	ModelMenuST.ScreenID = ModelMenu;
	ModelMenuST.FullScreen = No;
	ModelMenuST.Width = MAIN_MENU_WIDTH;
	ModelMenuST.Height = MAIN_MENU_HEIGHT;
	ModelMenuST.Header = Yes;
	ModelMenuST.HeaderText = "MODEL MENU";
	ModelMenuST.Footer = No;

	ModelMenuButtonColor.Font = &verdana_8pt_bold;
	ModelMenuButtonColor.ParentScreen = &ModelMenuST;
	ModelMenuButtonColor.Color = BASE_BUTTON_COLOR;
	ModelMenuButtonColor.TextColor = BASE_BUTTON_COLOR;
	ModelMenuButtonColor.BackgroundColor = BASE_BUTTON_BACK_COLOR;
	ModelMenuButtonColor.BorderThickness = BASE_BUTTON_BORDER_THICKNESS;
	ModelMenuButtonColor.BorderColor = BASE_BUTTON_BORDER_COLOR;
	ModelMenuButtonColor.FocusColor = FOCUS_BUTTON_COLOR;
	ModelMenuButtonColor.FocusBackgroundColor = FOCUS_BUTTON_BACK_COLOR;
	ModelMenuButtonColor.FocusBorderThickness = FOCUS_BUTTON_BORDER_THICKNESS;
	ModelMenuButtonColor.FocusBorderColor = FOCUS_BUTTON_BORDER_COLOR;
	/*
	 *
	 */

	for(i = 0; i < MODEL_MENU_BUTTON_NUM; i++)
	{
		__MDMbutton[i].Text = __MDMbuttonLabel[i];
		__MDMbutton[i].Style = &ModelMenuButtonColor;
		__MDMbutton[i].Width = MAIN_MENU_BUTTON_WIDTH;
		__MDMbutton[i].Height = MAIN_MENU_BUTTON_HEIGHT;
		__MDMbutton[i].X = MAIN_MENU_BUTTON_X;
		__MDMbutton[i].Y = MAIN_MENU_BUTTON_Y + i*MAIN_MENU_BUTTON_STEP;
		__MDMbutton[i].Border = Yes;

		STbuttonInit(&__MDMbutton[i]);
	}


	BackToMainlMenuButton.Text = __buttonLabelBack;
	BackToMainlMenuButton.Style = &ModelMenuButtonColor;
	BackToMainlMenuButton.Width = 60;
	BackToMainlMenuButton.Height = 26;
	BackToMainlMenuButton.X = BACK_BUTTON_X;
	BackToMainlMenuButton.Y = BACK_BUTTON_Y;
	BackToMainlMenuButton.Border = Yes;

//	NextModelMenuButton.Text = "NEXT";
//	NextModelMenuButton.Style = &ModelMenuButtonColor;
//	NextModelMenuButton.Width = 60;
//	NextModelMenuButton.Height = 26;
//	NextModelMenuButton.X = NEXT_BUTTON_X;
//	NextModelMenuButton.Y = NEXT_BUTTON_Y;
//	NextModelMenuButton.Border = Yes;

//	STbuttonInit(&DualRateButton);
	STbuttonInit(&BackToMainlMenuButton);
//	STbuttonInit(&NextModelMenuButton);
}
