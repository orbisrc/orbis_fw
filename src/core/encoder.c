/*
 * encoder.c
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

#include "core/encoder.h"
#include "tim.h"
#include "gui/guiconfig.h"

Encoder_HandlerTypeDef Encoder;

void Encoder_Init(void)
{
	Encoder.Divider = ENCODER_PULSES_PER_STEP;
	Encoder.Period = 20;
	Encoder.TIMx = TIM4;
	Encoder.Deadband = 0;

	EncoderSetPeriod(20, &Encoder);

	HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_ALL);
}


uint16_t EncoderGetCounter(Encoder_HandlerTypeDef *Encoder)
{
	return (uint16_t)Encoder->TIMx->CNT/(Encoder->Divider);
}

void EncoderSetCounter(uint16_t Counter, Encoder_HandlerTypeDef *Encoder)
{
	Encoder->TIMx->CNT = Counter * Encoder->Divider + Encoder->Deadband;
}

uint16_t EncoderGetPeriod(Encoder_HandlerTypeDef *Encoder)
{
	return Encoder->Period;
}

void EncoderSetPeriod(uint16_t Period, Encoder_HandlerTypeDef *Encoder)
{

	if (Encoder->Period != Period)
	{
		Encoder->Period = Period;
		Encoder->TIMx->CNT = Encoder->Divider;
	}

	Encoder->TIMx->ARR = (uint32_t)(Encoder->Period * Encoder->Divider);
}

uint16_t EncoderGetDivider(Encoder_HandlerTypeDef *Encoder)
{
	return Encoder->Divider;
}

void EncoderSetDivider(uint16_t Divider, Encoder_HandlerTypeDef *Encoder)
{
	Encoder->Divider = Divider;
}
