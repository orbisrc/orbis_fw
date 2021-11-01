/*
 * AlarmWarning.c
 *
 *  Created on: 10 авг. 2020 г.
 *      Author: ivnss
 */


#include "core/AlarmWarning.h"
#include "core/analog.h"
#include "core/periphery.h"
#include "core/iosettings.h"
#include "core/buffer.h"
#include "stconfig.h"
#include "stdlib.h"
#include "core/rcchannel.h"
#include "core/auxiliary.h"

#define TRIM_BUFF_LEN				4

AlarmWarningTypeDef		CommonAlarm = {0};

void AlarmWarningInit()
{

}

void AlarmWarningHandler()
{
	/*
	 * Main battery low voltage alarm
	 */

	if(AIgetWarning(&AnalogChannel[BatteryADC]))
	{
		BatteryAlarm(&MainBeeper);
	}


	/*
	 * Inactivity check
	 */
	uint16_t i = 0;
	uint16_t InactivityCheckNum = 0;
	static uint16_t InactivityBufferCheck[CHANNEL_BUFFER_LENGTH] = {0};
	static uint32_t InactivityStartTime = 0;

	for(i = 0; i < CHANNEL_BUFFER_LENGTH; i++)
	{
		if(abs(InactivityBufferCheck[i] - ChannelBuffer[i]) < INACTIVITY_DEADBAND)
		{
			InactivityCheckNum++;
		}
		InactivityBufferCheck[i] = ChannelBuffer[i];
	}

	if (InactivityCheckNum >= CHANNEL_BUFFER_LENGTH)
	{
		if ((HAL_GetTick() - InactivityStartTime) >= CommonSettings.InactivityDuration * 60000)
		{
			InactivityAlarm(&MainBeeper);
		}
	}
	else
	{
		InactivityStartTime = HAL_GetTick();
	}


	/*
	 * Trim center
	 */
	static int16_t TrimBufferCheck[TRIM_BUFF_LEN] = {0};

	if (CommonSettings.TrimBeeperEnable)
	{
		for(i = 0;i < TRIM_BUFF_LEN;i++)
		{
			if((RCChanelGetTrim(&RCChanel[i]) == 0) && (TrimBufferCheck[i] != 0))
			{
				TrimZeroWarning(&MainBeeper);
			}

			TrimBufferCheck[i]  = RCChanelGetTrim(&RCChanel[i]);
		}
	}

	/*
	 * Timer warning
	 */

	if ( RCtimerGetAlarm(&RCTimer1) || RCtimerGetAlarm(&RCTimer2))
	{
		TimerWarning(&MainBeeper);
	}
}


uint16_t STgetBatteryAlarmValue(AlarmWarningTypeDef* Struct)
{
	return Struct->BatteryAlarmValue;
}

void STsetBatteryAlarmValue(uint16_t Value, AlarmWarningTypeDef* Struct)
{
	Struct->BatteryAlarmValue = Value;
}

uint16_t STgetInactivityDuration(AlarmWarningTypeDef* Struct)
{
	return Struct->InactivityDuration;
}

void STsetInactivityDuration(uint16_t Value, AlarmWarningTypeDef* Struct)
{
	Struct->InactivityDuration = Value;
}
