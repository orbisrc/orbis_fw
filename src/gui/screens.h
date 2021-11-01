/*
 * screens.h
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

#ifndef STGUI_SCREENS_H_
#define STGUI_SCREENS_H_

#include <stdlib.h>
#include "periphery/rtc.h"
#include "periphery/adc.h"
#include "gui/gui.h"
#include <stdio.h>
#include <string.h>
#include "gui/stgui.h"
#include "gui/stapp.h"
#include "core/analog.h"
#include "gui/fonts/verdana_12pt.h"
#include "gui/fonts/verdana_18pt.h"
#include "gui/fonts/verdana_22pt.h"
#include "gui/fonts/verdana_26pt.h"
#include "gui/fonts/verdana_8pt.h"
#include "core/auxiliary.h"
#include "core/filter.h"
#include "core/iosettings.h"
#include "core/common.h"
#include "core/encoder.h"
#include "core/buffer.h"

typedef enum{
	Terminal,
	MainScreenScr,
	MainMenuScr,
	Settings,
	ModelMenu,
	ModelMenu2,
	ModelProfile,
	ModelSelect,
	EndPoint,
	ChannelTrim,
	STcurve,
	THcurve,
	ChannelReverse,
	SystemMenu,
	ServoView,
	DualRate,
	Keyboard,
	AckScreen,
	LapList,
	FilterSettings,
	STcalibrateScreen,
	THcalibrateScreen,
	DateTimeSets,
	AnalogInputScr,
	DiscreteInputScr,
	ChannelMapping,
	ChannelSettings,
	SystemInfo,
	BasicSettings,
	Transmitter,
	TimersSettings,
	ScreenSaver
}STscreensListEnum;

#define MENU_BUTTON_1_Y 			50
#define MENU_BUTTON_2_Y				(MENU_BUTTON_1_Y+MAIN_MENU_BUTTON_HEIGHT+MAIN_MENU_BUTTON_STEP)
#define MENU_BUTTON_3_Y				(MENU_BUTTON_2_Y+MAIN_MENU_BUTTON_HEIGHT+MAIN_MENU_BUTTON_STEP)
#define MENU_BUTTON_4_Y				(MENU_BUTTON_3_Y+MAIN_MENU_BUTTON_HEIGHT+MAIN_MENU_BUTTON_STEP)
#define MENU_BUTTON_5_Y				(MENU_BUTTON_4_Y+MAIN_MENU_BUTTON_HEIGHT+MAIN_MENU_BUTTON_STEP)
#define MENU_BUTTON_6_Y				(MENU_BUTTON_5_Y+MAIN_MENU_BUTTON_HEIGHT+MAIN_MENU_BUTTON_STEP)
#define MENU_BUTTON_7_Y				(MENU_BUTTON_6_Y+MAIN_MENU_BUTTON_HEIGHT+MAIN_MENU_BUTTON_STEP)

#define BACK_BUTTON_X				34			//
#define BACK_BUTTON_Y				270			//
#define NEXT_BUTTON_X				146			//
#define NEXT_BUTTON_Y				270			//

#define EXP_RATE_Y					60
#define EXP_RATE_NAME_X				1
#define EXP_RATE_INPUT				1

void _MainScreenInit();
void _MainScreen();

void _MainMenuInit();
void _MainMenu();

void _ModelMenuInit();
void _ModelMenu();

void _ModelMenu2();
void _ModelMenuInit2();

void _ModelSelectInit();
void _ModelSelect();

void  _ModelProfileInit();
void  _ModelProfile();

void _EndPointInit();
void _EndPoint();

void _TrimInit();
void _Trim();

void _STcurveInit();
void _STcurve();

void _THcurveInit();
void _THcurve();

void _InvertInit();
void _Invert();

void _SystemMenuInit();
void _SystemMenu();

void _ServoViewInit();
void _ServoView();

void _DualRateInit();
void _DualRate();

void _KeyboardInit();
void _Keyboard();

void _AckMenuInit();
void _AckMenu();

void _LapListInit();
void _LapList();

void _FilterSettingsInit();
void _FilterSettings();

void _STcalibrateInit();
void _STcalibrate();

void _THcalibrateInit();
void _THcalibrate();

void _DateTimeSettingsInit();
void _DateTimeSettings();

void _AnalogInputInit();
void _AnalogInput();

void _BasicSettingsInit();
void _BasicSettings();

void _DiscreteInputInit();
void _DiscreteInput();


void _CHmappingInit();
void _CHmapping();


void _CHsettingsInit();
void _CHsettings();
void _CHsettingsLoadToItem(uint16_t SelectedChannel);


void _InfoInit();
void _Info();

void _TimerSetInit();
void _TimerSet();

void _ScreenSaver();
void _ScreenSaverInit();


#endif /* STGUI_SCREENS_H_ */
