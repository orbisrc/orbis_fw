/*
 * keyboard.c
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
#include "periphery/rtc.h"
#include "core/auxiliary.h"

#define KEYBOARD_SCREEN_HEIGHT			260
#define KEYBOARD_SCREEN_WIDTH			210
#define OUTPUT_FIRLD_Y					35
#define OUTPUT_KEY_Y					65
#define KEY_START_X						20
#define KEY_START_Y						85
#define KEY_HEIGHT						20
#define KEY_WIDTH						20
#define KEY_NUM_X						10
#define KEY_NUM_Y						10
#define KEY_CHAR_OFFSET					32
#define KEY_CHAR_NUM					100
#define BACK_KEY_X						180
#define ENTR_KEY_X						135
#define OUTPUT_FIELD_X					20

STScreenStyleTypeDef KeyboardStyle = { 0 };
STScreenTypeDef KeyboardST = { 0 };
STkeyCharColorStyleTypeDef KeyboardCharStyle = { 0 };
STColorStyleTypeDef OutFieldStyle = { 0 };

STColorStyleTypeDef KeyButtonStyle = { 0 };

STbuttonTypeDef BackKeyboardButton = { 0 };
STbuttonTypeDef EntrKeyboardButton = { 0 };
STbuttonTypeDef BackspaceKeyboardButton = { 0 };

STtextBoxTypeDef TextOutKeyboard = { 0 };

STkeyCharTypeDef KeyboardChar[KEY_CHAR_NUM] = { 0 };

void _Keyboard() {
	uint16_t i;
	static uint16_t string_buff_pointer = 0;
	static char string[TEXT_BUFF_SIZE] = { 0 };

	if (STkeyboardShowNow(&KeyboardST)) {
		for (i = 0; i < KEY_CHAR_NUM; i++) {
			if (STkeyCharPressed(&KeyboardChar[i])) {
				string[string_buff_pointer] = STkeyCharGet(&KeyboardChar[i]);
				string_buff_pointer++;
				STtextBoxSetValue(string, &TextOutKeyboard);
			}
		}

		if (STbuttonPressed(&BackspaceKeyboardButton)) {
			if (string_buff_pointer > 0) {
				string_buff_pointer--;
				string[string_buff_pointer] = 0;
				;
				STtextBoxSetValue(string, &TextOutKeyboard);
			}
		}

		if (STbuttonPressed(&BackKeyboardButton)) {
			STappSetScreen(STappGetPrevScreen(&STApp), &STApp);
		}

		if (STbuttonPressed(&EntrKeyboardButton)) {
			STappFillInputBuff(string, &STApp);

			STappSetScreen(STappGetPrevScreen(&STApp), &STApp);
		}
	}

	if (STkeyboardOnShow(&KeyboardST)) {
		for (i = 0; i < TEXT_BUFF_SIZE; i++) {
			string[i] = 0;
		}
		string_buff_pointer = 0;
		STtextBoxSetValue(string, &TextOutKeyboard);
	}

	STkeyboardShow(&KeyboardST);

	for (i = 0; i < KEY_CHAR_NUM; i++) {
		STkeyCharShow(&KeyboardChar[i]);
	}

	STtextBoxShow(&TextOutKeyboard);

	STbuttonShow(&BackspaceKeyboardButton);
	STbuttonShow(&BackKeyboardButton);
	STbuttonShow(&EntrKeyboardButton);

}

void _KeyboardInit() {
	uint16_t i, j;
	uint16_t keyPointer = 0;
	char ValueChar = 0;

	KeyboardStyle.BackgroundColor = MAIN_MENU_BACK_COLOR;
	KeyboardStyle.BorderColor = MAIN_MENU_BORDER_COLOR;
	KeyboardStyle.BorderThickness = MAIN_MENU_BORDER_THICKNESS;

	KeyboardST.App = &STApp;
	KeyboardST.Style = &KeyboardStyle;
	KeyboardST.ScreenID = Keyboard;
	KeyboardST.Width = KEYBOARD_SCREEN_WIDTH;
	KeyboardST.Height = KEYBOARD_SCREEN_HEIGHT;

	KeyboardCharStyle.ParentScreen = &KeyboardST;
	KeyboardCharStyle.Width = KEY_WIDTH;
	KeyboardCharStyle.Height = KEY_HEIGHT;
	KeyboardCharStyle.Font = &verdana_8pt_bold;
	KeyboardCharStyle.Color = BASE_BUTTON_COLOR;
	KeyboardCharStyle.TextColor = BASE_BUTTON_COLOR;
	KeyboardCharStyle.BackgroundColor = BASE_BUTTON_BACK_COLOR;
	KeyboardCharStyle.BorderThickness = BASE_BUTTON_BORDER_THICKNESS;
	KeyboardCharStyle.BorderColor = BASE_BUTTON_BORDER_COLOR;
	KeyboardCharStyle.FocusColor = FOCUS_BUTTON_COLOR;
	KeyboardCharStyle.FocusBackgroundColor = FOCUS_BUTTON_BACK_COLOR;
	KeyboardCharStyle.FocusBorderThickness = FOCUS_BUTTON_BORDER_THICKNESS;
	KeyboardCharStyle.FocusBorderColor = FOCUS_BUTTON_BORDER_COLOR;

	KeyButtonStyle.Font = &verdana_8pt_bold;
	KeyButtonStyle.ParentScreen = &KeyboardST;
	KeyButtonStyle.Color = BASE_BUTTON_COLOR;
	KeyButtonStyle.TextColor = BASE_BUTTON_COLOR;
	KeyButtonStyle.BackgroundColor = BASE_BUTTON_BACK_COLOR;
	KeyButtonStyle.BorderThickness = BASE_BUTTON_BORDER_THICKNESS;
	KeyButtonStyle.BorderColor = BASE_BUTTON_BORDER_COLOR;
	KeyButtonStyle.FocusColor = FOCUS_BUTTON_COLOR;
	KeyButtonStyle.FocusBackgroundColor = FOCUS_BUTTON_BACK_COLOR;
	KeyButtonStyle.FocusBorderThickness = FOCUS_BUTTON_BORDER_THICKNESS;
	KeyButtonStyle.FocusBorderColor = FOCUS_BUTTON_BORDER_COLOR;

	OutFieldStyle.ParentScreen = &KeyboardST;
	OutFieldStyle.Font = &verdana_8pt_bold;
	OutFieldStyle.Color = BASE_OUTPUT_COLOR;
	OutFieldStyle.TextColor = BASE_OUTPUT_TEXT_COLOR;
	OutFieldStyle.BackgroundColor = BASE_OUTPUT_BACK_COLOR;
	OutFieldStyle.BorderThickness = BASE_OUTPUT_BORDER_THICKNESS;
	OutFieldStyle.BorderColor = BASE_OUTPUT_BORDER_COLOR;
	OutFieldStyle.FocusTextColor = FOCUS_OUTPUT_TEXT_COLOR;
	OutFieldStyle.FocusColor = FOCUS_OUTPUT_COLOR;
	OutFieldStyle.FocusBackgroundColor = FOCUS_OUTPUT_BACK_COLOR;
	OutFieldStyle.FocusBorderThickness = FOCUS_OUTPUT_BORDER_THICKNESS;
	OutFieldStyle.FocusBorderColor = FOCUS_OUTPUT_BORDER_COLOR;

	TextOutKeyboard.Text = "";
	TextOutKeyboard.Style = &OutFieldStyle;
	TextOutKeyboard.Width = 200;
	TextOutKeyboard.Height = 30;
	TextOutKeyboard.X = OUTPUT_FIELD_X;
	TextOutKeyboard.Y = OUTPUT_FIRLD_Y;
	TextOutKeyboard.Border = Yes;
	TextOutKeyboard.TextAlignment = Left;
	TextOutKeyboard.Dynamic = No;

	BackKeyboardButton.Text = "BCK";
	BackKeyboardButton.Style = &KeyButtonStyle;
	BackKeyboardButton.Width = 40;
	BackKeyboardButton.Height = 20;
	BackKeyboardButton.X = BACK_KEY_X;
	BackKeyboardButton.Y = OUTPUT_KEY_Y;
	BackKeyboardButton.Border = Yes;

	EntrKeyboardButton.Text = "=>";
	EntrKeyboardButton.Style = &KeyButtonStyle;
	EntrKeyboardButton.Width = 40;
	EntrKeyboardButton.Height = 20;
	EntrKeyboardButton.X = ENTR_KEY_X;
	EntrKeyboardButton.Y = OUTPUT_KEY_Y;
	EntrKeyboardButton.Border = Yes;

	BackspaceKeyboardButton.Text = "<- BS";
	BackspaceKeyboardButton.Style = &KeyButtonStyle;
	BackspaceKeyboardButton.Width = 40;
	BackspaceKeyboardButton.Height = 20;
	BackspaceKeyboardButton.X = KEY_START_X;
	BackspaceKeyboardButton.Y = OUTPUT_KEY_Y;
	BackspaceKeyboardButton.Border = Yes;

	STbuttonInit(&BackspaceKeyboardButton);
	STbuttonInit(&EntrKeyboardButton);
	STbuttonInit(&BackKeyboardButton);

	for (i = 0; i < KEY_NUM_Y; i++) {
		for (j = 0; j < KEY_NUM_X; j++) {
			KeyboardChar[keyPointer].Style = &KeyboardCharStyle;
			KeyboardChar[keyPointer].X = KEY_START_X + KEY_WIDTH * j;
			KeyboardChar[keyPointer].Y = KEY_START_Y + KEY_HEIGHT * i;
			KeyboardChar[keyPointer].Char = ValueChar + KEY_CHAR_OFFSET;

			STkeyCharInit(&KeyboardChar[keyPointer]);

			keyPointer++;
			ValueChar++;
		}
	}

	STtextBoxInit(&TextOutKeyboard);
}
