/*
 * encoder.h
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
#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

#include "stm32f4xx_hal.h"

typedef struct {
	TIM_TypeDef * TIMx;
	uint16_t Divider;
	uint16_t Period;
	uint16_t Counter;
	uint16_t Deadband;
} Encoder_HandlerTypeDef;

extern Encoder_HandlerTypeDef Encoder;


void Encoder_Init(void);

void EncoderHandler(Encoder_HandlerTypeDef *Encoder);

uint16_t EncoderGetCounter(Encoder_HandlerTypeDef *Encoder);

void EncoderSetCounter(uint16_t Counter, Encoder_HandlerTypeDef *Encoder);

uint16_t EncoderGetPeriod(Encoder_HandlerTypeDef *Encoder);

void EncoderSetPeriod(uint16_t Period, Encoder_HandlerTypeDef *Encoder);

uint16_t EncoderGetDivider(Encoder_HandlerTypeDef *Encoder);

void EncoderSetDivider(uint16_t Divider, Encoder_HandlerTypeDef *Encoder);

#endif /* INC_ENCODER_H_ */
