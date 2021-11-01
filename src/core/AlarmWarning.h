/*
 * AlarmWarning.h
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
 *limitations under the License.
 */

#include "stm32f4xx_hal.h"

#ifndef INC_STCORE_ALARMWARNING_H_
#define INC_STCORE_ALARMWARNING_H_

typedef struct {
	uint16_t		BatteryAlarmValue;	/* Battery minimum*/
	uint16_t		InactivityDuration;			/* Inactivity time (minutes)*/
}AlarmWarningTypeDef;


extern AlarmWarningTypeDef		CommonAlarm;

void AlarmWarningInit();

void AlarmWarningHandler();

uint16_t STgetBatteryAlarmValue(AlarmWarningTypeDef* Struct);

void STsetBatteryAlarmValue(uint16_t Value, AlarmWarningTypeDef* Struct);

uint16_t STgetInactivityDuration(AlarmWarningTypeDef* Struct);

void STsetInactivityDuration(uint16_t Value, AlarmWarningTypeDef* Struct);

#endif /* INC_STCORE_ALARMWARNING_H_ */
