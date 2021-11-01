/*
 * discrete.c
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

#include <core/discrete.h>
#include "stconfig.h"

DiscreteCHHandlerTypeDef AuxSW[DISCRETE_CH_NUM];

void DiscreteInputInit (void)
{
	AuxSW[0].InHPort = SWA_H_GPIO_Port;
	AuxSW[0].InLPort = SWA_L_GPIO_Port;
	AuxSW[0].InHPin = SWA_H_Pin;
	AuxSW[0].InLPin = SWA_L_Pin;
	AuxSW[0].ChType = Two;
	AuxSW[0].FilterDuration = DISCRETE_FILTER_DURATION;
	AuxSW[0].Hvalue = DISCRETE_VALUE_HIGHT;
	AuxSW[0].Lvalue = DISCRETE_VALUE_LOW;
	AuxSW[0].Mvalue = DISCRETE_VALUE_MID;

	AuxSW[1].InHPort = SWB_H_GPIO_Port;
	AuxSW[1].InLPort = SWB_L_GPIO_Port;
	AuxSW[1].InHPin = SWB_H_Pin;
	AuxSW[1].InLPin = SWB_L_Pin;
	AuxSW[1].ChType = Two;
	AuxSW[1].FilterDuration = DISCRETE_FILTER_DURATION;
	AuxSW[1].Hvalue = DISCRETE_VALUE_HIGHT;
	AuxSW[1].Lvalue = DISCRETE_VALUE_LOW;
	AuxSW[1].Mvalue = DISCRETE_VALUE_MID;

	AuxSW[2].InHPort = SWC_H_GPIO_Port;
	AuxSW[2].InLPort = SWC_L_GPIO_Port;
	AuxSW[2].InHPin = SWC_H_Pin;
	AuxSW[2].InLPin = SWC_L_Pin;
	AuxSW[2].ChType = Two;
	AuxSW[2].FilterDuration = DISCRETE_FILTER_DURATION;
	AuxSW[2].Hvalue = DISCRETE_VALUE_HIGHT;
	AuxSW[2].Lvalue = DISCRETE_VALUE_LOW;
	AuxSW[2].Mvalue = DISCRETE_VALUE_MID;

	AuxSW[3].InHPort = SWD_H_GPIO_Port;
	AuxSW[3].InLPort = SWD_L_GPIO_Port;
	AuxSW[3].InHPin = SWD_H_Pin;
	AuxSW[3].InLPin = SWD_L_Pin;
	AuxSW[3].ChType = Two;
	AuxSW[3].FilterDuration = DISCRETE_FILTER_DURATION;
	AuxSW[3].Hvalue = DISCRETE_VALUE_HIGHT;
	AuxSW[3].Lvalue = DISCRETE_VALUE_LOW;
	AuxSW[3].Mvalue = DISCRETE_VALUE_MID;

	AuxSW[4].InHPort = SWE_H_GPIO_Port;
	AuxSW[4].InLPort = SWE_L_GPIO_Port;
	AuxSW[4].InHPin = SWE_H_Pin;
	AuxSW[4].InLPin = SWE_L_Pin;
	AuxSW[4].ChType = Two;
	AuxSW[4].FilterDuration = DISCRETE_FILTER_DURATION;
	AuxSW[4].Hvalue = DISCRETE_VALUE_HIGHT;
	AuxSW[4].Lvalue = DISCRETE_VALUE_LOW;
	AuxSW[4].Mvalue = DISCRETE_VALUE_MID;

	AuxSW[5].InHPort = SWF_H_GPIO_Port;
	AuxSW[5].InLPort = SWF_L_GPIO_Port;
	AuxSW[5].InHPin = SWF_H_Pin;
	AuxSW[5].InLPin = SWF_L_Pin;
	AuxSW[5].ChType = Two;
	AuxSW[5].FilterDuration = DISCRETE_FILTER_DURATION;
	AuxSW[5].Hvalue = DISCRETE_VALUE_HIGHT;
	AuxSW[5].Lvalue = DISCRETE_VALUE_LOW;
	AuxSW[5].Mvalue = DISCRETE_VALUE_MID;

}

void DiscreteInputMain (void)
{
	uint16_t i =  0;

	for(i = 0; i < DISCRETE_CH_NUM; i++)
	{
		DiscreteChannelHandler(&AuxSW[i]);
	}
}

void DiscreteChannelHandler(DiscreteCHHandlerTypeDef *Discrete)
{
	/*
	 *
	 * Button push detect
	 *
	 */

	if(HAL_GPIO_ReadPin(Discrete->InHPort, Discrete->InHPin) == GPIO_PIN_RESET)
	{
		if ((HAL_GetTick() - Discrete->prevTimerStateH)	>= Discrete->FilterDuration)
		{
			Discrete->H = GPIO_PIN_SET;
		}
	}
	else
	{
		Discrete->H = GPIO_PIN_RESET;
		Discrete->prevTimerStateH = HAL_GetTick();
	}

	if(HAL_GPIO_ReadPin(Discrete->InLPort, Discrete->InLPin) == GPIO_PIN_RESET)
	{
		if ((HAL_GetTick() - Discrete->prevTimerStateL)	>= Discrete->FilterDuration)
		{
			Discrete->L = GPIO_PIN_SET;
		}
	}
	else
	{
		Discrete->L = GPIO_PIN_RESET;
		Discrete->prevTimerStateL = HAL_GetTick();
	}


	/*
	 *
	 */

	Discrete->State = SW_M;

	if (Discrete->L)
	{
		Discrete->State = SW_L;
	}
	if (Discrete->H)
	{
		Discrete->State = SW_H;
	}

	/*
	 * Analog output
	 */

	switch (Discrete->State)
	{
	case SW_M:
		Discrete->Value = Discrete->Mvalue;
		break;


	case SW_H:
		Discrete->Value = Discrete->Hvalue;
		break;


	case SW_L:
		Discrete->Value = Discrete->Lvalue;
		break;

	default:
	break;

	}
}

uint16_t DiscreteChannelGetState(DiscreteCHHandlerTypeDef *Discrete)
{
	return Discrete->State;
}

uint16_t DiscreteChannelGetValue(DiscreteCHHandlerTypeDef *Discrete)
{
	return Discrete->Value;
}

uint16_t DiscreteChannelGetHvalue(DiscreteCHHandlerTypeDef *Discrete)
{
	return Discrete->Hvalue;
}


uint16_t DiscreteChannelGetMvalue(DiscreteCHHandlerTypeDef *Discrete)
{
	return Discrete->Mvalue;
}


uint16_t DiscreteChannelGetLvalue(DiscreteCHHandlerTypeDef *Discrete)
{
	return Discrete->Lvalue;
}


void DiscreteChannelSetHvalue(uint16_t Value, DiscreteCHHandlerTypeDef *Discrete)
{
	Discrete->Hvalue = Value;
}


void DiscreteChannelSetMvalue(uint16_t Value, DiscreteCHHandlerTypeDef *Discrete)
{
	Discrete->Mvalue = Value;
}


void DiscreteChannelSetLvalue(uint16_t Value, DiscreteCHHandlerTypeDef *Discrete)
{
	Discrete->Lvalue = Value;
}

