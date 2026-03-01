/*
 * mainmenu.c
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

#define MAIN_SCREEN_BUTTON_X			34		// ���������� ������
#define MAIN_SCREEN_BUTTON_Y			270		//

#define MAIN_MENU_BUTTON_NUM			4

const char __MMbuttonLabel[MAIN_MENU_BUTTON_NUM][16] = {  "SERVO VIEW",
											 	"MODEL",
												"SYSTEM",
												"OTHER"
											};


const uint16_t	__MMbuttonTarger[MAIN_MENU_BUTTON_NUM] = { 	ServoView,
													ModelMenu,
													SystemMenu,
													MainScreenScr
											};

STbuttonTypeDef __MMbutton[MAIN_MENU_BUTTON_NUM] = { 0 };

STScreenStyleTypeDef MainMenuStyle = { 0 };
STScreenTypeDef MainMenuST = { 0 };

STColorStyleTypeDef MMButton = { 0 };

STbuttonTypeDef MainScreenButton = { 0 };

void _MainMenu() {

	uint16_t i = 0;

	if (STscreenShowNow(&MainMenuST))\
	{
		if (STbuttonPressed(&MainScreenButton))
		{
			STappSetScreen(MainScreenScr, &STApp);
			STrequestSettingsSave();
		}

		for(i = 0; i < MAIN_MENU_BUTTON_NUM; i++)
		{
			if (STbuttonPressed(&__MMbutton[i]))
			{
				STappSetScreen(__MMbuttonTarger[i], &STApp);
			}
		}
	}

	STscreenShow(&MainMenuST);

	for(i = 0; i < MAIN_MENU_BUTTON_NUM; i++)
	{
		STbuttonShow(&__MMbutton[i]);
	}

	STbuttonShow(&MainScreenButton);
}

void _MainMenuInit() {
	/*
	 *  ������������� �������� ������
	 */

	uint16_t i = 0 ;

	MainMenuStyle.HeaderFont = &verdana_8pt_bold;
	MainMenuStyle.BackgroundColor = MAIN_MENU_BACK_COLOR;
	MainMenuStyle.BorderColor = MAIN_MENU_BORDER_COLOR;
	MainMenuStyle.BorderThickness = MAIN_MENU_BORDER_THICKNESS;
	MainMenuStyle.HeaderColor = MAIN_MENU_HEADER_COLOR;
	MainMenuStyle.HeaderTextColor = MAIN_MENU_HEADER_TEXT_COLOR;

	MainMenuST.App = &STApp;
	MainMenuST.Style = &MainMenuStyle;
	MainMenuST.ScreenID = MainMenuScr;
	MainMenuST.FullScreen = No;
	MainMenuST.Width = MAIN_MENU_WIDTH;
	MainMenuST.Height = MAIN_MENU_HEIGHT;
	MainMenuST.Header = Yes;
	MainMenuST.HeaderText = "MAIN MENU";
	MainMenuST.Footer = No;

	/*
	 *  ����� � ����� ����������
	 */

	MMButton.Font = &verdana_8pt_bold;
	MMButton.ParentScreen = &MainMenuST;
	MMButton.Color = BASE_BUTTON_COLOR;
	MMButton.TextColor = BASE_BUTTON_COLOR;
	MMButton.BackgroundColor = BASE_BUTTON_BACK_COLOR;
	MMButton.BorderThickness = BASE_BUTTON_BORDER_THICKNESS;
	MMButton.BorderColor = BASE_BUTTON_BORDER_COLOR;
	MMButton.FocusColor = FOCUS_BUTTON_COLOR;
	MMButton.FocusBackgroundColor = FOCUS_BUTTON_BACK_COLOR;
	MMButton.FocusBorderThickness = FOCUS_BUTTON_BORDER_THICKNESS;
	MMButton.FocusBorderColor = FOCUS_BUTTON_BORDER_COLOR;

	/*
	 *  ������������� �������� ����������� ��������� ������
	 */

	MainScreenButton.Text = __buttonLabelBack;
	MainScreenButton.Style = &MMButton;
	MainScreenButton.Width = 60;
	MainScreenButton.Height = 26;
	MainScreenButton.X = BACK_BUTTON_X;
	MainScreenButton.Y = BACK_BUTTON_Y;
	MainScreenButton.Border = Yes;

	for(i = 0; i < MAIN_MENU_BUTTON_NUM; i++)
	{
		__MMbutton[i].Text = __MMbuttonLabel[i];
		__MMbutton[i].Style = &MMButton;
		__MMbutton[i].Width = MAIN_MENU_BUTTON_WIDTH;
		__MMbutton[i].Height = MAIN_MENU_BUTTON_HEIGHT;
		__MMbutton[i].X = MAIN_MENU_BUTTON_X;
		__MMbutton[i].Y = MAIN_MENU_BUTTON_Y + i*MAIN_MENU_BUTTON_STEP;
		__MMbutton[i].Border = Yes;

		STbuttonInit(&__MMbutton[i]);
	}

	STbuttonInit(&MainScreenButton);
}
