/*
 * auxiliary.h
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

#ifndef AUXILIARY_H_
#define AUXILIARY_H_

#include "stm32f4xx_hal.h"
#include "stconfig.h"
#include "gui/guiconfig.h"

/*
 * ����������
 */
typedef struct
{

	uint16_t TotalHour;
	uint16_t TotalMinute;
	uint16_t TotalSecond;
	uint16_t TotalMillisecond;
	uint32_t TotalMsFromStart;
	uint16_t Minute;
	uint16_t Second;
	uint16_t Millisecond;
	uint32_t StartMs;
	uint32_t MsFromStart;
	uint16_t Start;
	uint16_t CurrentLap;
	uint16_t LapList[LAPS_NUM][3];

} RCStopwatchTypeDef;

typedef struct
{
	uint32_t StartMs;
	uint32_t preTimeValue;
	uint32_t Millisecond;
	uint32_t Second;
	uint32_t Minute;
	uint16_t Start;
	uint16_t DiscretBufferItemNumber;
	uint16_t AlarmEnable;
	uint16_t Alarm;
	char 	 Name[MAX_RC_NAME];
	uint32_t MsSetpoint;
	uint16_t MinuteSetpoint;
	uint16_t SecondSetpoint;
} RCTimerTypeDef;

extern RCStopwatchTypeDef MainStopwatch;

extern RCTimerTypeDef RCTimer1;

extern RCTimerTypeDef RCTimer2;


/*
 * Stopwatch
 */

void RCStopwatch(RCStopwatchTypeDef *Stopwatch);

void RCStopwatchInit(RCStopwatchTypeDef *Stopwatch);

void RCStopwatchNewLap(RCStopwatchTypeDef *Stopwatch);

void RCStopwatchReset(RCStopwatchTypeDef *Stopwatch);

uint16_t RCStopwatchGetLapsNumber(RCStopwatchTypeDef *Stopwatch);

uint16_t RCStopwatchGetCurrentLapMs(RCStopwatchTypeDef *Stopwatch);

uint16_t RCStopwatchGetCurrentLapSec(RCStopwatchTypeDef *Stopwatch);

uint16_t RCStopwatchGetCurrentLapMin(RCStopwatchTypeDef *Stopwatch);

uint16_t RCStopwatchGetLapMs(uint16_t Lap, RCStopwatchTypeDef *Stopwatch);

uint16_t RCStopwatchGetLapSec(uint16_t Lap, RCStopwatchTypeDef *Stopwatch);

uint16_t RCStopwatchGetLapMin(uint16_t Lap, RCStopwatchTypeDef *Stopwatch);

void RCStopwatchTotalCalc(uint32_t Ms, RCStopwatchTypeDef *Stopwatch);

uint16_t RCStopwatchGetTotalLapMs(RCStopwatchTypeDef *Stopwatch);

uint16_t RCStopwatchGetTotalLapSec(RCStopwatchTypeDef *Stopwatch);

uint16_t RCStopwatchGetTotalLapMin(RCStopwatchTypeDef *Stopwatch);

uint16_t RCStopwatchGetTotalLapHour(RCStopwatchTypeDef *Stopwatch);


/*
 * Timer
 */

void RCtimerMain();

void RCtimerInitHandler();

void RCStimer(RCTimerTypeDef *Timer);

void RCStimerInit(RCTimerTypeDef *Timer);

void RCStimerReset(RCTimerTypeDef *Timer);

uint16_t RCStimerGetMinute(RCTimerTypeDef *Timer);

uint16_t RCStimerGetSecond(RCTimerTypeDef *Timer);

uint16_t RCStimerGetMs(RCTimerTypeDef *Timer);

uint16_t RCStimerGetBuffItemNumber(RCTimerTypeDef *Timer);

void RCStimerSetBuffItemNumber(uint16_t Value, RCTimerTypeDef *Timer);

char* RCStimerGetName(RCTimerTypeDef *Timer);

void RCStimerSetName(const char *Name,RCTimerTypeDef *Timer);

void RCStimerAlarmEnable(RCTimerTypeDef *Timer);

void RCStimerAlarmDisable(RCTimerTypeDef *Timer);

uint16_t RCStimerGetAlarmState(RCTimerTypeDef *Timer);

void RCStimerSetAlarmState(uint16_t Value, RCTimerTypeDef *Timer);

void RCStimerSetName(const char *Name,RCTimerTypeDef *Timer);

void RCStimerSetMinuteSetpoint(uint16_t Value,RCTimerTypeDef *Timer);

uint16_t RCStimerGetMinuteSetpoint(RCTimerTypeDef *Timer);

void RCStimerSetSecondSetpoint(uint16_t Value,RCTimerTypeDef *Timer);

uint16_t RCStimerGetSecondSetpoint(RCTimerTypeDef *Timer);

uint16_t RCtimerGetAlarm(RCTimerTypeDef *Timer);

#endif /* AUXILIARY_H_ */

