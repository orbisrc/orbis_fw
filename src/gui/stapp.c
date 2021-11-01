/*
 * atapp.c
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

#include "gui/stgui.h"
#include "gui/stapp.h"
#include "core/periphery.h"
#include "core/encoder.h"

STApplicationTypeDef STApp = { 0 };

void STappInit()
{



}

void STapp() {
	/* �������� ���������. ������ �������
	 * ���� �����, �� � ��������.
	 */
	/*
	 * ���� ����� ������� ������� ������ � ���������.
	 * �������� ����� �������� ���-�� ����� �������
	 */
	STApp.ButtonLeft = buttonPress(&hbtnLeft);
	STApp.ButtonRight = buttonPress(&hbtnRight);
	STApp.ButtonUp = buttonPress(&hbtnUp);
	STApp.ButtonDown = buttonPress(&hbtnDown);
	STApp.ButtonEnter = buttonPress(&hbtnEnter);
	STApp.ButtonBack = buttonPress(&hbtnReset);
	STApp.ButtonLeftLong = buttonPressLong(&hbtnLeft);
	STApp.ButtonRightLong = buttonPressLong(&hbtnRight);
	STApp.ButtonUpLong = buttonPressLong(&hbtnUp);
	STApp.ButtonDownLong = buttonPressLong(&hbtnDown);
	STApp.ButtonEnterLong = buttonPressLong(&hbtnEnter);
	STApp.ButtonBackLong = buttonPressLong(&hbtnReset);


	STappShow(&STApp);

	if (STApp.Beep == Yes) {

		if (STApp.BeeperEnable)
		{
			ShortBeep();
		}
	//
	//	ShortVibro();

	}

	if (STApp.LongBeep == Yes) {

		ShortVibro();
//		BatteryAlarm(&MainBeeper);
	}

}
