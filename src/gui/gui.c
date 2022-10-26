/*
 * gui.c
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

#include <gui/fonts/verdana_26pt.h>
#include "gui/gui.h"
#include "gui/stapp.h"
#include "gui/screens.h"
#include "gui/stgui.h"
#include "gui/logo.h"
#include "gui/dispdriver/S6D0154X.h"
#include "core/iosettings.h"
#include "core/buffer.h"
#include "core/encoder.h"
#include "stconfig.h"



const char * __buttonLabelYes = 								"YES";
const char * __buttonLabelNo = 									"NO";
const char * __buttonLabelBack = 								"BACK";
const char * __buttonLabelNext = 								"NEXT";
const char * __buttonLabelMenu = 								"MENU";
const char * __buttonLabelCheckBoxOn= 							"ON";
const char * __buttonLabelCheckBoxOff = 						"OFF";

const char * __CH1label = 										"ST";
const char * __CH2label = 										"TH";
const char * __CH3label = 										"CH1";
const char * __CH4label = 										"CH2";
const char * __CH5label = 										"CH3";
const char * __CH6label = 										"CH4";
const char * __CH7label = 										"CH5";
const char * __CH8label = 										"CH6";

#ifdef __ORBIS_AIR__
const char * __CHLabel[] = 										{"Aileron",
																"Elevator",
																"Throttle",
																"Rudder",
																"CH5",
																"CH6",
																"CH7",
																"CH8",
																"CH9",
																"CH10",
																"CH11",
																"CH12",
																"CH13",
																"CH14",
																"CH15",
																"CH16"};

const char * __CHLabelShort[] = 								{"Ail",
																"Ele",
																"Thr",
																"Rud",
																"CH5",
																"CH6",
																"CH7",
																"CH8",
																"CH9",
																"CH10",
																"CH11",
																"CH12",
																"CH13",
																"CH14",
																"CH15",
																"CH16"};

#endif	

#ifdef __ORBIS_SURFACE__
const char * __CHLabel[] = 										{"Stearing",
																"Throttle",
																"CH3",
																"CH4",
																"CH5",
																"CH6",
																"CH7",
																"CH8",
																"CH9",
																"CH10",
																"CH11",
																"CH12",
																"CH13",
																"CH14",
																"CH15",
																"CH16"};

const char * __CHLabelShort[] = 								{"ST",
																"TH",
																"CH3",
																"CH4",
																"CH5",
																"CH6",
																"CH7",
																"CH8",
																"CH9",
																"CH10",
																"CH11",
																"CH12",
																"CH13",
																"CH14",
																"CH15",
																"CH16"};
#endif

const  char		*			 __USBmodeLaberl[] =			{ "VirtualCom",
																"Joystick",
																"SDcard"

};

void GUI_DFU(void)
{

}

void GUI_Init(void) {

	STLCDinit();
	STLCDsetBrightness(CommonSettings.DisplayBrightness); //

	ShowLogo();

	STappInit();

	_MainScreenInit();
	_MainMenuInit();
	_ModelMenuInit();
//	_ModelMenuInit2();
	_EndPointInit();
	_TrimInit();
//	_STcurveInit();
//	_THcurveInit();
	_InvertInit();
	_SystemMenuInit();
	_ModelSelectInit();
	_ServoViewInit();
//	_DualRateInit();
	_KeyboardInit();
	_AckMenuInit();
//	_ModelProfileInit();
//	_LapListInit();
//	_FilterSettingsInit();
	_STcalibrateInit();
//	_THcalibrateInit();
//	_DateTimeSettingsInit();
	_AnalogInputInit();
	_DiscreteInputInit();
	_CHmappingInit();
	_CHsettingsInit();
	_InfoInit();
	_BasicSettingsInit();
	_TimerSetInit();
	_ScreenSaverInit();

	STappSetScreen(MainScreenScr, &STApp);
}

void GUI(void) {

	// static uint32_t LastActivityTime = {0};
	// static uint16_t LastEncoderCounter = {0};

	STapp();


#ifdef __ORBIS_AIR__
	
#endif

#ifdef __ORBIS_SURFACE__

#endif

	_MainScreen();
	_MainMenu();
	_ModelMenu();
//	_ModelMenu2();
	_EndPoint();
	_Trim();
//	_STcurve();
//	_THcurve();
	_Invert();
	_SystemMenu();
	_ModelSelect();
	_ServoView();
//	_DualRate();
	_Keyboard();
	_AckMenu();
//	_ModelProfile();
//	_LapList();
//	_FilterSettings();
	_STcalibrate();
//	_THcalibrate();
//	_DateTimeSettings();
	_AnalogInput();
	_DiscreteInput();
	_CHmapping();
	_CHsettings();
	_Info();
	_BasicSettings();
	_TimerSet();
	_ScreenSaver();


	/*
	 * Inactiv screen
	 */
	// if((GetBufferActiveItemNumber() == NON_CHANGE_BUFFER_ITEM) && (LastEncoderCounter == EncoderGetCounter(&Encoder)))
	// {
	// 	if ((HAL_GetTick() - LastActivityTime) >= CommonSettings.ScreenInactivityTime * 60000)
	// 	{
	// 		STLCDsetBrightness(100);
	// 	}
	// }

	// else
	// {
	// 	LastActivityTime = HAL_GetTick();

	// 	LastEncoderCounter = EncoderGetCounter(&Encoder);

	// 	STLCDsetBrightness(CommonSettings.DisplayBrightness);
	// }

	STLCDsetBrightness(CommonSettings.DisplayBrightness);

	/*
	 * ScreenSaver
	 */
}
