/**
 *
 *
 * File Name          : Periphery.c
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

#include "stconfig.h"
#include "main.h"
#include "periphery/gpio.h"
#include "core/periphery.h"
#include "gui/guiconfig.h"
#include "tim.h"

/*
 * ��������� ������������ ������
 */
BTN_HandlerTypeDef hbtnUp = {0};
BTN_HandlerTypeDef hbtnDown = {0};
BTN_HandlerTypeDef hbtnLeft = {0};
BTN_HandlerTypeDef hbtnRight = {0};
BTN_HandlerTypeDef hbtnEnter = {0};
BTN_HandlerTypeDef hbtnReset = {0};

Beeper_HandlerTypeDef Beeper = {0};
Beeper_HandlerTypeDef Vibro = {0};
Beeper_HandlerTypeDef MainBeeper = {0};

/*
 * ������������� �������� ������������ ��������� ������
 *
 */
void BTN_Init(void)
{
	/*
	 * Encoder
	 */
#if CONTROL_METHOD == 1

	hbtnEnter.Port = BTN_ENTR_GPIO_Port;
	hbtnEnter.Pin = BTN_ENTR_Pin;
	hbtnEnter.shortPressDuration = SHORT_PRESS_DURATION;
	hbtnEnter.longPressDuration = LONG_PRESS_DURATION;

#elif CONTROL_METHOD == 2

	/*
	 * Button pad
	 */

	hbtnUp.Port = BTN_UP_GPIO_Port;
	hbtnUp.Pin = BTN_UP_Pin;
	hbtnUp.shortPressDuration = SHORT_PRESS_DURATION;
	hbtnUp.longPressDuration = LONG_PRESS_DURATION;

	hbtnDown.Port = BTN_DOWN_GPIO_Port;
	hbtnDown.Pin = BTN_DOWN_Pin;
	hbtnDown.shortPressDuration = SHORT_PRESS_DURATION;
	hbtnDown.longPressDuration = LONG_PRESS_DURATION;

	hbtnLeft.Port = BTN_LEFT_GPIO_Port;
	hbtnLeft.Pin = BTN_LEFT_Pin;
	hbtnLeft.shortPressDuration = SHORT_PRESS_DURATION;
	hbtnLeft.longPressDuration = LONG_PRESS_DURATION;

	hbtnRight.Port = BTN_RIGHT_GPIO_Port;
	hbtnRight.Pin = BTN_RIGHT_Pin;
	hbtnRight.shortPressDuration = SHORT_PRESS_DURATION;
	hbtnRight.longPressDuration = LONG_PRESS_DURATION;

	hbtnEnter.Port = BTN_ENTR_GPIO_Port;
	hbtnEnter.Pin = BTN_ENTR_Pin;
	hbtnEnter.shortPressDuration = SHORT_PRESS_DURATION;
	hbtnEnter.longPressDuration = LONG_PRESS_DURATION;

#endif
}

uint16_t buttonPress(BTN_HandlerTypeDef *BTNx)
{
	uint16_t buttonState = 0;

	if (HAL_GPIO_ReadPin(BTNx->Port, BTNx->Pin) == PIN_PUSH)
	{
		BTNx->prevState = BUTTON_PRESS;
	}
	else
	{
		if (BTNx->prevState == BUTTON_PRESS)
		{
			if ((HAL_GetTick() - (uint32_t)BTNx->prevTimerState) >= (uint32_t)BTNx->shortPressDuration && (HAL_GetTick() - (uint32_t)BTNx->prevTimerState) < (uint32_t)BTNx->longPressDuration)
			{
				buttonState = BUTTON_PRESS;
			}
			else
			{
				buttonState = BUTTON_NOT_PRESS;
			}
		}
		BTNx->prevState = BUTTON_NOT_PRESS;
		BTNx->prevTimerState = HAL_GetTick();
	}
	return buttonState;
}

/*******************************************************************************
 * ������� ������� �� ������
 * ��������� ������ �� ������������, ��� ������.
 *
 */
uint16_t buttonPressLong(BTN_HandlerTypeDef *BTNx)
{
	uint16_t buttonState = 0;

	if (HAL_GPIO_ReadPin(BTNx->Port, BTNx->Pin) == PIN_PUSH)
	{

		if ((HAL_GetTick() - (uint32_t)BTNx->prevTimerState) >= (uint32_t)BTNx->longPressDuration)
		{
			if (BTNx->prevLongPressState != BUTTON_PRESS)
			{
				buttonState = BUTTON_PRESS;
				BTNx->prevLongPressState = BUTTON_PRESS;
			}
			else
			{
				buttonState = BUTTON_NOT_PRESS;
			}
		}
	}
	else
	{
		BTNx->prevLongPressState = BUTTON_NOT_PRESS;
		BTNx->prevTimerState = HAL_GetTick();
	}

	return buttonState;
}

/*******************************************************************************
 * �������
 */

void BPR_Init()
{

	//	Beeper.Pin = BEEPER_Pin;
	//	Beeper.Port = BEEPER_GPIO_Port;
	//	Beeper.shortBeepDuration = SHORT_BEEP_DURATION;
	//	Beeper.longBeepDuration = LONG_BEEP_DURATION;
	//
	//	Vibro.Pin = VIBRO_Pin;
	//	Vibro.Port = VIBRO_GPIO_Port;
	//	Vibro.shortBeepDuration = SHORT_VIBRO_DURATION;
	//	Vibro.longBeepDuration = LONG_VIBRO_DURATION;
}

void BeeperHandler(Beeper_HandlerTypeDef *BPRx)
{

	/*
	 * 	Battery alarm beeper handler
	 *
	 */
	switch (BPRx->BatteryAlarmStep)
	{
	case FirstBeep:

		if (BPRx->BatteryAlarm)
		{
			ShortBeep();

			BPRx->BatteryAlarmStartTick = HAL_GetTick();

			BPRx->BatteryAlarmStep++;
		}

		//			BPRx->BatteryAlarmStartTick = HAL_GetTick();

		break;

	case FirstPause:

		if ((HAL_GetTick() - BPRx->BatteryAlarmStartTick) > BATTERY_ALARM_PAUSE_DURATION)
		{
			BPRx->BatteryAlarmStep++;
		}
		else
		{
		}

		break;

	case SecondBeep:

		ShortBeep();

		BPRx->BatteryAlarmStartTick = HAL_GetTick();

		BPRx->BatteryAlarmStep++;
		break;

	case SecondPause:

		if ((HAL_GetTick() - BPRx->BatteryAlarmStartTick) > BATTERY_ALARM_FULL_DURATION)
		{
			BPRx->BatteryAlarmStep++;
		}
		else
		{
		}

		break;

	default:

		BPRx->BatteryAlarmStep = 0;
		BPRx->BatteryAlarm = 0;
		break;
	}

	/*
	 * 	Inactivity warning beeper handler
	 *
	 */

	switch (BPRx->StbyWarningStep)
	{
	case FirstBeep:

		if (BPRx->StbyWarning)
		{
			LongBeep();

			BPRx->StbyWarningStartTick = HAL_GetTick();

			BPRx->StbyWarningStep++;
		}

		//				BPRx->StbyWarningStartTick = HAL_GetTick();

		break;

	case FirstPause:

		if ((HAL_GetTick() - BPRx->StbyWarningStartTick) > BATTERY_ALARM_PAUSE_DURATION)
		{
			BPRx->StbyWarningStep++;
		}
		else
		{
		}

		break;

	case SecondBeep:

		LongBeep();

		BPRx->StbyWarningStartTick = HAL_GetTick();

		BPRx->StbyWarningStep++;
		break;

	case SecondPause:

		if ((HAL_GetTick() - BPRx->StbyWarningStartTick) > BATTERY_ALARM_FULL_DURATION)
		{
			BPRx->StbyWarningStep++;
		}
		else
		{
		}

		break;

	default:

		BPRx->StbyWarningStep = 0;
		BPRx->StbyWarning = 0;
		break;
	}

	/*
	 * Trim warning
	 */

	if (BPRx->TrimWarning)
	{
		LongBeep();
		BPRx->TrimWarning = 0;
	}

	/*
	 * Timer warning
	 */

	if (BPRx->TimerWarning)
	{
		BPRx->TrimWarning = 0;

		switch (BPRx->TimerWarningStep)
		{
		case FirstBeep:

			if (BPRx->TimerWarning)
			{
				ShortBeep();
				LongVibro();

				BPRx->TimerWarningStartTick = HAL_GetTick();

				BPRx->TimerWarningStep++;
			}

			//			BPRx->BatteryAlarmStartTick = HAL_GetTick();

			break;

		case FirstPause:

			if ((HAL_GetTick() - BPRx->TimerWarningStartTick) > TIMER_ALARM_PAUSE_DURATION)
			{
				BPRx->TimerWarningStep++;
			}
			else
			{
			}

			break;

		case SecondBeep:

			ShortBeep();
			ShortVibro();

			BPRx->TimerWarningStartTick = HAL_GetTick();

			BPRx->TimerWarningStep++;
			break;

		case SecondPause:

			if ((HAL_GetTick() - BPRx->TimerWarningStartTick) > TIMER_ALARM_FULL_DURATION)
			{
				BPRx->TimerWarningStep++;
			}
			else
			{
			}

			break;

		default:

			BPRx->TimerWarningStep = 0;
			BPRx->TimerWarning = 0;
			break;
		}
	}
}

void LongBeep()
{
	TIM13->ARR = (uint32_t)LONG_BEEP_DURATION;

	HAL_TIM_Base_Start(&htim13);

	HAL_TIM_Base_Start_IT(&htim13);

	HAL_GPIO_WritePin(BEEPER_GPIO_Port, BEEPER_Pin, GPIO_PIN_SET);
}

void ShortBeep()
{
	TIM13->ARR = (uint32_t)SHORT_BEEP_DURATION;

	if (HAL_TIM_Base_Start_IT(&htim13) == HAL_OK)
	{
		HAL_GPIO_WritePin(BEEPER_GPIO_Port, BEEPER_Pin, GPIO_PIN_SET);
	}
}

//

void ShortVibro()
{
	TIM14->ARR = (uint32_t)SHORT_VIBRO_DURATION;

	if (HAL_TIM_Base_Start_IT(&htim14) == HAL_OK)
	{
		HAL_GPIO_WritePin(VIBRO_GPIO_Port, VIBRO_Pin, GPIO_PIN_SET);
	}
}

void LongVibro()
{

	TIM14->ARR = (uint32_t)LONG_VIBRO_DURATION;

	if (HAL_TIM_Base_Start_IT(&htim14) == HAL_OK)
	{
		HAL_GPIO_WritePin(VIBRO_GPIO_Port, VIBRO_Pin, GPIO_PIN_SET);
	}
}

void BatteryAlarm(Beeper_HandlerTypeDef *BPRx)
{
	BPRx->BatteryAlarm = 1;
}

void InactivityAlarm(Beeper_HandlerTypeDef *BPRx)
{
	BPRx->StbyWarning = 1;
}

void TrimZeroWarning(Beeper_HandlerTypeDef *BPRx)
{
	BPRx->TrimWarning = 1;
}

void TimerWarning(Beeper_HandlerTypeDef *BPRx)
{
	BPRx->TimerWarning = 1;
}
