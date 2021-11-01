/*
 * ppm.c
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


#include <core/ppm.h>
#include "tim.h"
#include "core/rcchannel.h"

PPM_HandlerTypedef	PPMSignal = {0};



void PPMhandler(PPM_HandlerTypedef *PPM)
{
	uint16_t i = 0;
	uint16_t AllDuration = 0;

	if (PPM->ppmStep >= PPM_CH_NUM*2 + 2 - 1)
	{
		PPM->ppmStep = 0;
	}
	else
	{
		PPM->ppmStep++;
	}

	HAL_GPIO_TogglePin(PPM->Port, PPM->Pin);

	if(PPM->ppmStep <= PPM_CH_NUM*2)
	{
		TIM10->ARR = (uint32_t)PPM->ppmCHValue[PPM->ppmStep];
	}
	else
	{
		for (i=0; i < PPM_CH_NUM*2;i++)
		{
			AllDuration += PPM->ppmCHValue[i];
		}

		PPM->Tsync = PPM->T - AllDuration;

		TIM10->ARR = (uint32_t)PPM->Tsync;
	}
}



void PPMhandlerInit(void)
{
	PPMSignal.Port = PPM_GPIO_Port;
	PPMSignal.Pin = PPM_Pin;
	PPMSignal.T = PPM_FULL_DURATION;
	PPMSignal.Tmin = PPM_TMIN;
	PPMSignal.Tmax = PPM_TMAX;
	PPMSignal.Tpause = PPM_PAUSE;
	PPMSignal.Polarity = Direct;
	PPMSignal.ChannelNumber = 8;
//

	PPMinit(&PPMSignal, &htim10);
}

void PPMassignmentValues(void)
{
	uint16_t i = 0;
	uint16_t Value = 0;

	for(i = 0; i < 8;i++)
	{
		Value = RCChanelGetValue(&RCChanel[i]) 	+
				RCChanelGetTrim(&RCChanel[i])	+
				PPM_TMIN + TRIM_CH_MAX - TRIM_CH_MIN;

		PPMsetChannelValue(Value, i, &PPMSignal);
	}

}

void PPMinit(PPM_HandlerTypedef *PPM, TIM_HandleTypeDef *htim)
{
	uint16_t i = 0;

	for (i = 0; i < PPM_CH_NUM*2 + 2 - 1; i++)
	{
		PPM->ppmCHValue[i] = PPM->Tpause;
	}

	PPM->ppmCHValue[PPM_CH_NUM*2 + 2 - 1] = PPM->Tsync;

	HAL_GPIO_WritePin(PPM->Port, PPM->Pin, GPIO_PIN_SET);

	TIM10->ARR = (uint32_t)PPM->Tpause;

	HAL_TIM_Base_Start_IT(htim);

}

uint16_t PPMgetCurrentDuration(PPM_HandlerTypedef *PPM)
{
	return 0;
}

void PPMsetChannelValue(uint16_t Value, uint16_t Channel, PPM_HandlerTypedef *PPM)
{
	if(Value > PPM->Tmax)
	{
		PPM->ppmCHValue[Channel*2 + 1] = PPM->Tmax;
	}
	else if(Value <  PPM->Tmin)
	{
		PPM->ppmCHValue[Channel*2 + 1] =  PPM->Tmin;
	}
	else
	{
		PPM->ppmCHValue[Channel*2 + 1] = Value;
	}
}


uint16_t PPMgetFullT(PPM_HandlerTypedef *PPM)
{
	return PPM->T;
}

uint16_t PPMgetTmin(PPM_HandlerTypedef *PPM)
{
	return PPM->Tmin;
}

uint16_t PPmgetTmax(PPM_HandlerTypedef *PPM)
{
	return PPM->Tmax;
}

uint16_t PPMgetTpause(PPM_HandlerTypedef *PPM)
{
	return PPM->Tpause;
}

uint16_t PPMgetTsync(PPM_HandlerTypedef *PPM)
{
	return PPM->Tsync;
}

void PPMsetFullT(uint16_t Value, PPM_HandlerTypedef *PPM)
{
	PPM->T = Value;
}

void PPMsetTmin(uint16_t Value, PPM_HandlerTypedef *PPM)
{
	PPM->Tmin = Value;
}

void PPmsetTmax(uint16_t Value, PPM_HandlerTypedef *PPM)
{
	PPM->Tmax = Value;
}

void PPMsetTpause(uint16_t Value, PPM_HandlerTypedef *PPM)
{
	PPM->Tpause = Value;
}

void PPMsetTsync(uint16_t Value, PPM_HandlerTypedef *PPM)
{
	PPM->Tsync = Value;
}
