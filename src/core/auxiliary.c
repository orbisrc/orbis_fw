/*
 * auxiliary.c
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

#include "core/auxiliary.h"
#include "stconfig.h"
#include <string.h>
#include "core/buffer.h"

RCStopwatchTypeDef MainStopwatch = { 0 };

RCTimerTypeDef RCTimer1 = { 0 };

RCTimerTypeDef RCTimer2;

/************************************
 * Stopwatch
 ***********************************/

void RCStopwatch(RCStopwatchTypeDef *Stopwatch) {

	if (Stopwatch != 0) {
		Stopwatch->MsFromStart = HAL_GetTick() - Stopwatch->StartMs;

		Stopwatch->Millisecond = Stopwatch->MsFromStart % 1000;
		Stopwatch->Second = ((Stopwatch->MsFromStart - Stopwatch->Millisecond)
				/ 1000) % 60;
		Stopwatch->Minute = (Stopwatch->MsFromStart - Stopwatch->Second * 1000
				- Stopwatch->Millisecond) / 60000;

		Stopwatch->Millisecond = (uint16_t) Stopwatch->Millisecond / 100;
	}

}

void RCStopwatchInit(RCStopwatchTypeDef *Stopwatch) {
	Stopwatch->Millisecond = 0;
	Stopwatch->Second = 0;
	Stopwatch->Minute = 0;

	Stopwatch->StartMs = HAL_GetTick();
}

void RCStopwatchNewLap(RCStopwatchTypeDef *Stopwatch) {

	RCStopwatchTotalCalc(Stopwatch->MsFromStart, Stopwatch);

	Stopwatch->LapList[Stopwatch->CurrentLap][0] = Stopwatch->Millisecond;
	Stopwatch->LapList[Stopwatch->CurrentLap][1] = Stopwatch->Second;
	Stopwatch->LapList[Stopwatch->CurrentLap][2] = Stopwatch->Minute;

	Stopwatch->StartMs = HAL_GetTick();

	if (Stopwatch->CurrentLap >= LAPS_NUM) {
//		Stopwatch->CurrentLap = 0;
	} else {
		Stopwatch->CurrentLap++;
	}

}

void RCStopwatchReset(RCStopwatchTypeDef *Stopwatch) {
	uint16_t i = 0;

	Stopwatch->StartMs = HAL_GetTick();

	for (i = 0; i < LAPS_NUM; i++) {
		Stopwatch->LapList[i][0] = 0;
		Stopwatch->LapList[i][1] = 0;
		Stopwatch->LapList[i][2] = 0;
	}

	Stopwatch->TotalMsFromStart = 0;
	Stopwatch->TotalMillisecond = 0;
	Stopwatch->TotalSecond = 0;
	Stopwatch->TotalMinute = 0;
	Stopwatch->TotalHour = 0;

	Stopwatch->CurrentLap = 0;
}

uint16_t RCStopwatchGetCurrentLapMs(RCStopwatchTypeDef *Stopwatch) {
	return Stopwatch->Millisecond;
}

uint16_t RCStopwatchGetCurrentLapSec(RCStopwatchTypeDef *Stopwatch) {
	return Stopwatch->Second;
}

uint16_t RCStopwatchGetCurrentLapMin(RCStopwatchTypeDef *Stopwatch) {
	return Stopwatch->Minute;
}

uint16_t RCStopwatchGetLapsNumber(RCStopwatchTypeDef *Stopwatch) {
	return Stopwatch->CurrentLap;
}

uint16_t RCStopwatchGetLapMs(uint16_t Lap, RCStopwatchTypeDef *Stopwatch) {
	return Stopwatch->LapList[Lap][0];
}

uint16_t RCStopwatchGetLapSec(uint16_t Lap, RCStopwatchTypeDef *Stopwatch) {
	return Stopwatch->LapList[Lap][1];
}

uint16_t RCStopwatchGetLapMin(uint16_t Lap, RCStopwatchTypeDef *Stopwatch) {
	return Stopwatch->LapList[Lap][2];
}

void RCStopwatchTotalCalc(uint32_t Ms, RCStopwatchTypeDef *Stopwatch) {
	Stopwatch->TotalMsFromStart += Ms;

	Stopwatch->TotalMillisecond = Stopwatch->TotalMsFromStart % 1000;
	Stopwatch->TotalSecond = ((Stopwatch->TotalMsFromStart
			- Stopwatch->TotalMillisecond) / 1000) % 60;
	Stopwatch->TotalMinute = (Stopwatch->TotalMsFromStart
			- Stopwatch->TotalSecond * 1000 - Stopwatch->TotalMillisecond)
			/ 60000;
	Stopwatch->TotalHour = (Stopwatch->TotalMsFromStart
			- Stopwatch->TotalMinute * 60000 - Stopwatch->TotalSecond * 1000
			- Stopwatch->TotalMillisecond) / 3600000;
	Stopwatch->TotalMillisecond = (uint16_t) Stopwatch->TotalMillisecond / 100;
}

uint16_t RCStopwatchGetTotalLapMs(RCStopwatchTypeDef *Stopwatch) {
	return Stopwatch->TotalMillisecond;
}

uint16_t RCStopwatchGetTotalLapSec(RCStopwatchTypeDef *Stopwatch) {
	return Stopwatch->TotalSecond;
}

uint16_t RCStopwatchGetTotalLapMin(RCStopwatchTypeDef *Stopwatch) {
	return Stopwatch->TotalMinute;
}

uint16_t RCStopwatchGetTotalLapHour(RCStopwatchTypeDef *Stopwatch) {
	return Stopwatch->TotalHour;
}

/************************************
 * Timer
 ***********************************/

void RCtimerMain()
{
	RCStimer(&RCTimer1);
	RCStimer(&RCTimer2);
}

void RCtimerInitHandler()
{
	RCStimerInit(&RCTimer1);
	RCStimerInit(&RCTimer2);

	RCStimerSetName("Timer1", &RCTimer1);
	RCStimerSetName("Timer2", &RCTimer2);
}

void RCStimer(RCTimerTypeDef *Timer)
{
	Timer->Start = DiscreteBuffer[RCStimerGetBuffItemNumber(Timer)];

	if (Timer->Start)
	{
		Timer->Millisecond += HAL_GetTick() - Timer->preTimeValue;

		Timer->Second = (uint32_t)(Timer->Millisecond / 1000) % 60;

		Timer->Minute =(uint32_t)Timer->Millisecond / 60000;

		Timer->preTimeValue = HAL_GetTick();
	}
	else
	{
		Timer->preTimeValue = HAL_GetTick();
	}

	if (Timer->AlarmEnable)
	{
		if(Timer->Millisecond >= Timer->MsSetpoint)
		{
			Timer->Alarm = 1;
		}
		else
		{
			Timer->Alarm = 0;
		}
	}

}

void RCStimerInit(RCTimerTypeDef *Timer)
{
	Timer->StartMs = HAL_GetTick();
	Timer->preTimeValue = HAL_GetTick();

	Timer->Millisecond = 0;
	Timer->Second = 0;
	Timer->Minute = 0;

}

void RCStimerReset(RCTimerTypeDef *Timer)
{
	RCStimerInit(Timer);
}

uint16_t RCStimerGetMinute(RCTimerTypeDef *Timer)
{
	return Timer->Minute;
}

uint16_t RCStimerGetSecond(RCTimerTypeDef *Timer)
{
	return Timer->Second;
}

uint16_t RCStimerGetMs(RCTimerTypeDef *Timer)
{
	return Timer->Millisecond;
}

char* RCStimerGetName(RCTimerTypeDef *Timer)
{
	return Timer->Name;
}

void RCStimerSetName(const char *Name, RCTimerTypeDef *Timer)
{
	if(Name != NULL)
	{
		strlcpy(Timer->Name, Name, MAX_RC_NAME);
	}
}

uint16_t RCStimerGetBuffItemNumber(RCTimerTypeDef *Timer)
{
	return Timer->DiscretBufferItemNumber;
}

void RCStimerSetBuffItemNumber(uint16_t Value, RCTimerTypeDef *Timer)
{
	Timer->DiscretBufferItemNumber = Value;
}

void RCStimerAlarmEnable(RCTimerTypeDef *Timer)
{
	Timer->AlarmEnable = 1;
}

void RCStimerAlarmDisable(RCTimerTypeDef *Timer)
{
	Timer->AlarmEnable = 0;
}

uint16_t RCStimerGetAlarmState(RCTimerTypeDef *Timer)
{
	return Timer->AlarmEnable;
}

void RCStimerSetAlarmState(uint16_t Value, RCTimerTypeDef *Timer)
{
	if (Value >= 1)
	{
		Timer->AlarmEnable = 1;
	} else
	{
		Timer->AlarmEnable = 0;
	}
}

void RCStimerSetMinuteSetpoint(uint16_t Value,RCTimerTypeDef *Timer)
{
	Timer->MinuteSetpoint = Value;


	Timer->MsSetpoint = (uint32_t)(Timer->MinuteSetpoint * 60000 + Timer->SecondSetpoint * 1000);
}

uint16_t RCStimerGetMinuteSetpoint(RCTimerTypeDef *Timer)
{
	return Timer->MinuteSetpoint;
}

void RCStimerSetSecondSetpoint(uint16_t Value,RCTimerTypeDef *Timer)
{
	Timer->SecondSetpoint = Value;

	Timer->MsSetpoint = (uint32_t)(Timer->MinuteSetpoint * 60000 + Timer->SecondSetpoint * 1000);
}

uint16_t RCStimerGetSecondSetpoint(RCTimerTypeDef *Timer)
{
	return Timer->SecondSetpoint;
}

uint16_t RCtimerGetAlarm(RCTimerTypeDef *Timer)
{
	return Timer->Alarm;
}
