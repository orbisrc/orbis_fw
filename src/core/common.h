/*
 * common.h
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


#ifndef INC_STCORE_COMMON_H_
#define INC_STCORE_COMMON_H_

#include "stconfig.h"

typedef struct
{
	uint32_t LoopFreqTemp;
	double LoopFreq;
	uint32_t LoopFreqCounter;

}LoopFreqHandlerTypedef;

/*
 * Diagnose
 */

// ADC
__IO extern uint32_t ADCloopFreq;
__IO extern uint32_t ADCloopDuration;
extern LoopFreqHandlerTypedef ADCloop;

// RC Channel
__IO extern uint32_t RCchLoopFreq;
__IO extern uint32_t RCchLoopDuration;
extern LoopFreqHandlerTypedef RCchLoop;

// Main
__IO extern uint32_t MainLoopFreq;
__IO extern uint32_t MainLoopDuration;
extern LoopFreqHandlerTypedef MainLoop;

extern char dMessage[DEBUG_MESSAGE_MAX];

enum USBmodeEnum
{
	__VCOM,
	__Joystick,
	__SDcardReader
};


void CommonInit(void);

void CommonRun(void);

void CommonLoopFreqCheck(LoopFreqHandlerTypedef *LoopFreqCheck);

double CommonLoopGetFreq(LoopFreqHandlerTypedef *LoopFreqCheck);

void RCloop(void);

uint8_t common_printf(const char * str);

#endif /* INC_STCORE_COMMON_H_ */
