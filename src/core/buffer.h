/*
 * buffer.h
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

#ifndef INC_STCORE_BUFFER_H_
#define INC_STCORE_BUFFER_H_


#include "stconfig.h"
#include "stm32f4xx_hal.h"

/*
 *  A - Aileron
 *  E - Elevator
 *  T - Throttle
 *  R - Rudder
 *  PT1
 *  PT2
 *  SWA
 *  SWB
 *  SWC
 *  SWD
 *  SWE
 *  SWF
 */

#define NON_CHANGE_BUFFER_ITEM			0xBEAF


extern __IO uint16_t DiscreteBuffer[DISCRETE_BUFFER_LENGTH];
extern __IO uint16_t ChannelBuffer[CHANNEL_BUFFER_LENGTH];
extern const char __CHBufferItemsLabel[CHANNEL_BUFFER_LENGTH][16];
extern const char __DiscreteBufferItemsLabel[CHANNEL_BUFFER_LENGTH][16];

typedef enum {
	A,
	E,
	T,
	R,
	PT1,
	PT2,
	PT3,
	PT4,
	SWA,
	SWB,
	SWC,
	SWD,
	SWE,
	SWF,
	SWG,
	SWH,
	SWI,
	SWJ,
	S1,
	S2,
	S3,
	S4,
	S5,
	S6,
	S7,
	S8,
	S9,
	S10,
	S11,
	S12,
	S13,
	S14
} BufferItemNameTypeDef;

typedef enum {
	SWA_L,
	SWA_M,
	SWA_H,
	SWB_L,
	SWB_M,
	SWB_H,
	SWC_L,
	SWC_M,
	SWC_H,
	SWD_L,
	SWD_M,
	SWD_H,
	SWE_L,
	SWE_M,
	SWE_H,
	SWF_L,
	SWF_M,
	SWF_H,
	LS1,
	LS2,
	LS3,
	LS4,
	LS5,
	LS6,
	LS7,
	LS8,
	LS9,
	LS10,
	LS11,
	LS12,
	LS13,
	StUP
} DiscreteBufferItemNameTypeDef;

void ChannelBufferHandler(void);

void DiscretBufferHandler(void);

uint16_t GetBufferActiveItemNumber (void);

#endif /* INC_STCORE_BUFFER_H_ */
