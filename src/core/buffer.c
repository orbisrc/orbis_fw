/*
 * buffer.c
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

#include "stdlib.h"
#include "core/buffer.h"
#include "core/analog.h"
#include "core/discrete.h"
#include "gui/stdispdriver.h"



__IO uint16_t DiscreteBuffer[DISCRETE_BUFFER_LENGTH] = {0};
__IO uint16_t ChannelBuffer[CHANNEL_BUFFER_LENGTH] = {0};
__IO uint16_t BufferChangedItem = {0};

	const char __CHBufferItemsLabel[CHANNEL_BUFFER_LENGTH][16] = 	{	"A",
																		"E",
																		"T",
																		"R",
																		"PT1",
																		"PT2",
																		"PT3",
																		"PT4",
																		"SWA",
																		"SWB",
																		"SWC",
																		"SWD",
																		"SWE",
																		"SWF",
																		"SWG",
																		"SWH",
																		"SWI",
																		"SWJ",
																		"S1",
																		"S2",
																		"S3",
																		"S4",
																		"S5",
																		"S6",
																		"S7",
																		"S8",
																		"S9",
																		"S10",
																		"S11",
																		"S12",
																		"S13",
																		"S14"};

const char __DiscreteBufferItemsLabel[DISCRETE_BUFFER_LENGTH][16] ={	"SWA_L",
																		"SWA_M",
																		"SWA_H",
																		"SWB_L",
																		"SWB_M",
																		"SWB_H",
																		"SWC_L",
																		"SWC_M",
																		"SWC_H",
																		"SWD_L",
																		"SWD_M",
																		"SWD_H",
																		"SWE_L",
																		"SWE_M",
																		"SWE_H",
																		"SWF_L",
																		"SWF_M",
																		"SWF_H",
																		"LS1",
																		"LS2",
																		"LS3",
																		"LS4",
																		"LS5",
																		"LS6",
																		"LS7",
																		"LS8",
																		"LS9",
																		"LS10",
																		"LS11",
																		"LS12",
																		"LS13",
																		"StUP"

};


void ChannelBufferHandler(void)
{
	static uint16_t ChangeCheckBuffer[CHANNEL_BUFFER_LENGTH] = {0};
	uint16_t i = 0;

	ChannelBuffer[A] = AIgetValue(&AnalogChannel[0]);
	ChannelBuffer[E] = AIgetValue(&AnalogChannel[1]);
	ChannelBuffer[T] = AIgetValue(&AnalogChannel[2]);
	ChannelBuffer[R] = AIgetValue(&AnalogChannel[3]);
	ChannelBuffer[PT1] = AIgetValue(&AnalogChannel[4]);
	ChannelBuffer[PT2] = AIgetValue(&AnalogChannel[5]);
	ChannelBuffer[PT3] = 0;
	ChannelBuffer[PT4] = 0;
	ChannelBuffer[SWA] = DiscreteChannelGetValue(&AuxSW[SWA_IN]);
	ChannelBuffer[SWB] = DiscreteChannelGetValue(&AuxSW[SWB_IN]);
	ChannelBuffer[SWC] = DiscreteChannelGetValue(&AuxSW[SWC_IN]);
	ChannelBuffer[SWD] = DiscreteChannelGetValue(&AuxSW[SWD_IN]);
	ChannelBuffer[SWE] = DiscreteChannelGetValue(&AuxSW[SWE_IN]);
	ChannelBuffer[SWF] = DiscreteChannelGetValue(&AuxSW[SWF_IN]);
	ChannelBuffer[SWG] = 0;
	ChannelBuffer[SWH] = 0;
	ChannelBuffer[SWI] = 0;
	ChannelBuffer[SWJ] = 0;
	ChannelBuffer[S1] = 0;
	ChannelBuffer[S2] = 0;
	ChannelBuffer[S3] = 0;
	ChannelBuffer[S4] = 0;
	ChannelBuffer[S5] = 0;
	ChannelBuffer[S6] = 0;
	ChannelBuffer[S7] = 0;
	ChannelBuffer[S8] = 0;
	ChannelBuffer[S9] = 0;
	ChannelBuffer[S10] = 0;
	ChannelBuffer[S11] = 0;
	ChannelBuffer[S12] = 0;
	ChannelBuffer[S13] = 0;
	ChannelBuffer[S14] = 0;

	/*
	 * Change check
	 */
	for(i = 0; i <= CHANNEL_BUFFER_LENGTH; i++)
	{
		if(abs(ChannelBuffer[i] - ChangeCheckBuffer[i]) <= INACTIVITY_DEADBAND)
		{
			if( i >= CHANNEL_BUFFER_LENGTH - 1)
			{
				BufferChangedItem = NON_CHANGE_BUFFER_ITEM;

				break;
			}
		}
		else
		{
			BufferChangedItem = i;

			break;
		}
	}

	for(i = 0; i <= CHANNEL_BUFFER_LENGTH - 1; i++)
	{
		ChangeCheckBuffer[i] = ChannelBuffer[i];
	}
}

void DiscretBufferHandler(void)
{

	DiscreteBuffer[SWA_L] = DiscreteChannelGetState(&AuxSW[SWA_IN]) == SW_L ? 1 : 0;
	DiscreteBuffer[SWA_M] = DiscreteChannelGetState(&AuxSW[SWA_IN]) == SW_M ? 1 : 0;
	DiscreteBuffer[SWA_H] = DiscreteChannelGetState(&AuxSW[SWA_IN]) == SW_H ? 1 : 0;
	DiscreteBuffer[SWB_L] = DiscreteChannelGetState(&AuxSW[SWB_IN]) == SW_L ? 1 : 0;
	DiscreteBuffer[SWB_M] = DiscreteChannelGetState(&AuxSW[SWB_IN]) == SW_M ? 1 : 0;
	DiscreteBuffer[SWB_H] = DiscreteChannelGetState(&AuxSW[SWB_IN]) == SW_H ? 1 : 0;
	DiscreteBuffer[SWC_L] = DiscreteChannelGetState(&AuxSW[SWC_IN]) == SW_L ? 1 : 0;
	DiscreteBuffer[SWC_M] = DiscreteChannelGetState(&AuxSW[SWC_IN]) == SW_M ? 1 : 0;
	DiscreteBuffer[SWC_H] = DiscreteChannelGetState(&AuxSW[SWC_IN]) == SW_H ? 1 : 0;
	DiscreteBuffer[SWD_L] = DiscreteChannelGetState(&AuxSW[SWC_IN]) == SW_L ? 1 : 0;
	DiscreteBuffer[SWD_M] = DiscreteChannelGetState(&AuxSW[SWD_IN]) == SW_M ? 1 : 0;
	DiscreteBuffer[SWD_H] = DiscreteChannelGetState(&AuxSW[SWD_IN]) == SW_H ? 1 : 0;
	DiscreteBuffer[SWE_L] = 0;
	DiscreteBuffer[SWE_M] = 0;
	DiscreteBuffer[SWE_H] = 0;
	DiscreteBuffer[SWF_L] = 0;
	DiscreteBuffer[SWF_M] = 0;
	DiscreteBuffer[SWF_H] = 0;
	DiscreteBuffer[LS1] = 0;
	DiscreteBuffer[LS2] = 0;
	DiscreteBuffer[LS3] = 0;
	DiscreteBuffer[LS4] = 0;
	DiscreteBuffer[LS5] = 0;
	DiscreteBuffer[LS6] = 0;
	DiscreteBuffer[LS7] = 0;
	DiscreteBuffer[LS8] = 0;
	DiscreteBuffer[LS9] = 0;
	DiscreteBuffer[LS10] = 0;
	DiscreteBuffer[LS11] = 0;
	DiscreteBuffer[LS12] = 0;
	DiscreteBuffer[LS13] = 0;
	DiscreteBuffer[StUP] = 1;



}



/*
 * Return number of changed buffer item.
 * But if items not changed, return 0xFF;
 */

uint16_t GetBufferActiveItemNumber (void)
{
	return BufferChangedItem;
}
