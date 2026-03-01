/*
 * systemmenu.c
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

#define SYSTEM_MENU_BUTTON_NUM			5

STScreenStyleTypeDef SystemMenuStyle = { 0 };
STScreenTypeDef SystemMenuST = { 0 };

const char __SMbuttonLabel[SYSTEM_MENU_BUTTON_NUM][16] = {
																"ST CALIBRATE",
																"ANALOG INPUT",
																"DISCRETE INPUT",
																"BASIC SETTINGS",
																"INFO"
																};


const uint16_t	__SMbuttonTarget[SYSTEM_MENU_BUTTON_NUM] = {
																STcalibrateScreen,
																AnalogInputScr,
																DiscreteInputScr,
																BasicSettings,
																SystemInfo
																};

STbuttonTypeDef __SMbutton[SYSTEM_MENU_BUTTON_NUM] = { 0 };

STColorStyleTypeDef SMButton = { 0 };
STbuttonTypeDef SystemMenuButton = { 0 };

void _SystemMenu() {

	uint16_t i = 0;

	if (STscreenShowNow(&SystemMenuST)) {
		if (STbuttonPressed(&SystemMenuButton)) {
			STappSetScreen(MainScreenScr, &STApp);

			STrequestSettingsSave();
		}

		for(i = 0; i < SYSTEM_MENU_BUTTON_NUM; i++)
		{
			if (STbuttonPressed(&__SMbutton[i]))
			{
				STappSetScreen(__SMbuttonTarget[i], &STApp);
			}
		}
	}

	STscreenShow(&SystemMenuST);

	for(i = 0; i < SYSTEM_MENU_BUTTON_NUM; i++)
	{
		STbuttonShow(&__SMbutton[i]);
	}

	STbuttonShow(&SystemMenuButton);
}

void _SystemMenuInit()
{
	uint16_t i = 0;

	SystemMenuStyle.HeaderFont = &verdana_8pt_bold;
	SystemMenuStyle.BackgroundColor = MAIN_MENU_BACK_COLOR;
	SystemMenuStyle.BorderColor = MAIN_MENU_BORDER_COLOR;
	SystemMenuStyle.BorderThickness = MAIN_MENU_BORDER_THICKNESS;
	SystemMenuStyle.HeaderColor = MAIN_MENU_HEADER_COLOR;
	SystemMenuStyle.HeaderTextColor = MAIN_MENU_HEADER_TEXT_COLOR;

	SystemMenuST.App = &STApp;
	SystemMenuST.Style = &SystemMenuStyle;
	SystemMenuST.ScreenID = SystemMenu;
	SystemMenuST.FullScreen = No;
	SystemMenuST.Width = MAIN_MENU_WIDTH;
	SystemMenuST.Height = MAIN_MENU_HEIGHT;
	SystemMenuST.Header = Yes;
	SystemMenuST.HeaderText = "SYSTEM SETTINGS";
	SystemMenuST.Footer = No;

	SMButton.Font = &verdana_8pt_bold;
	SMButton.ParentScreen = &SystemMenuST;
	SMButton.Color = BASE_BUTTON_COLOR;
	SMButton.TextColor = BASE_BUTTON_COLOR;
	SMButton.BackgroundColor = BASE_BUTTON_BACK_COLOR;
	SMButton.BorderThickness = BASE_BUTTON_BORDER_THICKNESS;
	SMButton.BorderColor = BASE_BUTTON_BORDER_COLOR;
	SMButton.FocusColor = FOCUS_BUTTON_COLOR;
	SMButton.FocusBackgroundColor = FOCUS_BUTTON_BACK_COLOR;
	SMButton.FocusBorderThickness = FOCUS_BUTTON_BORDER_THICKNESS;
	SMButton.FocusBorderColor = FOCUS_BUTTON_BORDER_COLOR;

	for(i = 0; i < SYSTEM_MENU_BUTTON_NUM; i++)
	{
		__SMbutton[i].Text = __SMbuttonLabel[i];
		__SMbutton[i].Style = &SMButton;
		__SMbutton[i].Width = MAIN_MENU_BUTTON_WIDTH;
		__SMbutton[i].Height = MAIN_MENU_BUTTON_HEIGHT;
		__SMbutton[i].X = MAIN_MENU_BUTTON_X;
		__SMbutton[i].Y = MAIN_MENU_BUTTON_Y + i*MAIN_MENU_BUTTON_STEP;
		__SMbutton[i].Border = Yes;

		STbuttonInit(&__SMbutton[i]);
	}


	SystemMenuButton.Text = __buttonLabelBack;
	SystemMenuButton.Style = &SMButton;
	SystemMenuButton.Width = 60;
	SystemMenuButton.Height = 26;
	SystemMenuButton.X = BACK_BUTTON_X;
	SystemMenuButton.Y = BACK_BUTTON_Y;
	SystemMenuButton.Border = Yes;

	STbuttonInit(&SystemMenuButton);

}
