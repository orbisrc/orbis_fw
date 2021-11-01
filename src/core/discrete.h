/*
 * discrete.h
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

#ifndef INC_STCORE_DISCRETE_H_
#define INC_STCORE_DISCRETE_H_

#include "gpio.h"
#include "stconfig.h"

typedef struct {
	uint32_t 		prevTimerStateH;	/* */
	uint32_t 		prevTimerStateL;	/* */
	uint32_t 		timerStateH;		/* */
	uint32_t 		timerStateL;		/* */
	GPIO_TypeDef 	*InLPort;			/* */
	GPIO_TypeDef 	*InHPort;			/* */
	uint16_t 		InHPin;				/* */
	uint16_t 		InLPin;				/* */
	uint16_t 		H;					/* Hight switch */
	uint16_t 		M;					/* Mid switch */
	uint16_t 		L;					/* Low switch */
	uint16_t 		ChType;				/* 1, 2 input channel */
	uint16_t 		FilterDuration;     /* */
	uint16_t 		State;				/* */
	uint16_t 		Hvalue;				/* */
	uint16_t 		Lvalue;				/* */
	uint16_t 		Mvalue;				/* */
	uint16_t 		Value;				/* */

} DiscreteCHHandlerTypeDef;

typedef enum {
	SW_M, SW_H, SW_L
} SWState;

typedef enum {
	None, One, Two
} SWChannelType;

typedef enum {
	SWA_IN,
	SWB_IN,
	SWC_IN,
	SWD_IN,
	SWE_IN,
	SWF_IN,
	SWG_IN,
	SWH_IN,
} SWChannelNameTypedef;

extern DiscreteCHHandlerTypeDef AuxSW[DISCRETE_CH_NUM];

void DiscreteInputInit (void);

void DiscreteInputMain (void);

void DiscreteChannelHandler(DiscreteCHHandlerTypeDef *Discrete);

uint16_t DiscreteChannelGetState(DiscreteCHHandlerTypeDef *Discrete);

uint16_t DiscreteChannelGetValue(DiscreteCHHandlerTypeDef *Discrete);

uint16_t DiscreteChannelGetHvalue(DiscreteCHHandlerTypeDef *Discrete);

uint16_t DiscreteChannelGetMvalue(DiscreteCHHandlerTypeDef *Discrete);

uint16_t DiscreteChannelGetLvalue(DiscreteCHHandlerTypeDef *Discrete);

void DiscreteChannelSetHvalue(uint16_t Value, DiscreteCHHandlerTypeDef *Discrete);

void DiscreteChannelSetMvalue(uint16_t Value, DiscreteCHHandlerTypeDef *Discrete);

void DiscreteChanneSetLvalue(uint16_t Value, DiscreteCHHandlerTypeDef *Discrete);




#endif /* INC_STCORE_DISCRETE_H_ */
