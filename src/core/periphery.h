/**
 *
 * File Name          : Periphery.h
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

#ifndef __periphery_H
#define __periphery_H

#define SHORT_PRESS_DURATION 		30
#define LONG_PRESS_DURATION			2000

#define BUTTON_PRESS				1
#define BUTTON_NOT_PRESS			0

#define PIN_PUSH					0				// �������� �� ����� ��� ������� ������
#define PIN_REL						1				// �������� �� ����� ���� ������ �� ������




typedef enum {
	ButtonNotPress, ButtonPress
} BTNState;

typedef enum {
	NotBeep, Beep
} BPRState;

typedef struct {
	GPIO_TypeDef *Port;					//
	uint16_t Pin;					//
	uint16_t shortPressDuration;     //
	uint16_t longPressDuration;		//
	uint32_t timerState;				//
	uint16_t State;					//
	uint16_t prevState;				//
	uint16_t prevLongPressState;		//
	uint32_t prevTimerState;			//
} BTN_HandlerTypeDef;

typedef struct {
	GPIO_TypeDef *Port;					//
	uint16_t Pin;					//
	uint16_t shortBeepDuration;     //
	uint16_t longBeepDuration;		//
	uint16_t currentBeepDuration;		//
	uint32_t timerState;				//
	uint16_t State;					//
	uint16_t prevState;				//
	uint32_t prevTimerState;			//
	uint16_t Beep;				//

	uint16_t AlarmEnable;
	uint16_t WarningEnable;
	uint16_t BatteryAlarmEnable;
	uint16_t StbyWarningEnable;
	uint16_t TrimWarningEnable;
	uint16_t TimerWarningEnable;

	uint16_t BatteryAlarm;
	uint16_t StbyWarning;
	uint16_t Alarm;
	uint16_t Warning;
	uint16_t TimerAlarm;
	uint16_t TrimWarning;
	uint16_t TimerWarning;

	uint16_t BatteryAlarmStep;
	uint16_t StbyWarningStep;
	uint16_t WarningStep;
	uint16_t AlarmStep;
	uint16_t TimerAlarmStep;
	uint16_t TrimWarningStep;
	uint16_t TimerWarningStep;

	uint32_t BatteryAlarmStartTick;
	uint32_t StbyWarningStartTick;
	uint32_t AlarmStartTick;
	uint32_t WarningStartTick;
	uint32_t TimerAlarmStartTick;
	uint32_t TrimWarningStartTick;
	uint32_t TimerWarningStartTick;
} Beeper_HandlerTypeDef;

typedef enum
{
	FirstBeep,
	FirstPause,
	SecondBeep,
	SecondPause
} BeperAlgoStep;

extern BTN_HandlerTypeDef hbtnUp;
extern BTN_HandlerTypeDef hbtnDown;
extern BTN_HandlerTypeDef hbtnLeft;
extern BTN_HandlerTypeDef hbtnRight;
extern BTN_HandlerTypeDef hbtnEnter;
extern BTN_HandlerTypeDef hbtnReset;

extern Beeper_HandlerTypeDef Beeper;
extern Beeper_HandlerTypeDef Vibro;
extern Beeper_HandlerTypeDef MainBeeper;

void BTN_Init(void);
uint16_t buttonPress(BTN_HandlerTypeDef *BTNx);
uint16_t buttonPressLong(BTN_HandlerTypeDef *BTNx);

void BPR_Init(void);
void BeeperHandler(Beeper_HandlerTypeDef *BPRx);

void LongBeep();
void ShortBeep();

void ShortVibro();
void LongVibro();

void BatteryAlarm(Beeper_HandlerTypeDef *BPRx);
void InactivityAlarm(Beeper_HandlerTypeDef *BPRx);
void TrimZeroWarning(Beeper_HandlerTypeDef *BPRx);
void TimerWarning(Beeper_HandlerTypeDef *BPRx);

#endif
