/*
 * ppm.h
 *
	Copyright 2019 Denis Davydov

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
 */

#ifndef INC_STCORE_PPM_H_
#define INC_STCORE_PPM_H_

#include "main.h"
#include "stconfig.h"

typedef enum{
	Direct,
	Invert
} enumPolarity;

typedef struct
{
	GPIO_TypeDef *Port;					//
	uint16_t 	Pin;
	uint16_t	ppmCHValue[PPM_CH_NUM*2 + 2];			/*	*/
	uint16_t	ppmStep;				/*	������� ������	*/
	uint16_t	CurrentPulseType;		/*	0 - ����������� �������, 1 - �������� ������*/
	uint16_t	CurrentDuration;		/*  ������� ������������ �������	*/
	uint16_t	T;						/*	������ ������������ �������. */
	uint16_t	Tsync;					/*	������������ ������������� */
	uint16_t	Tmin;					/*	����������� ������������ ������� �������� ������*/
	uint16_t	Tmax;					/*	������������ ������������ ������� �������� ������*/
	uint16_t	Tpause;					/*	������������ ����� ����� ��������*/
	uint16_t	Polarity;				/*	���������� �������*/
	uint16_t	ChannelNumber;			/*	����� ������� 1...8*/

}PPM_HandlerTypedef;


extern PPM_HandlerTypedef	PPMSignal;

uint16_t PPMgetCurrentDuration(PPM_HandlerTypedef *PPM);

void PPMinit(PPM_HandlerTypedef *PPM, TIM_HandleTypeDef *htim);

void PPMnextStep(PPM_HandlerTypedef *PPM);

uint16_t PPMgetChannelValue(PPM_HandlerTypedef *PPM);

void PPMsetChannelValue(uint16_t Value, uint16_t Channel, PPM_HandlerTypedef *PPM);

void PPMhandler(PPM_HandlerTypedef *PPM);

void PPMhandlerInit(void);

void PPMassignmentValues(void);

uint16_t PPMgetFullT(PPM_HandlerTypedef *PPM);

uint16_t PPMgetTmin(PPM_HandlerTypedef *PPM);

uint16_t PPmgetTmax(PPM_HandlerTypedef *PPM);

uint16_t PPMgetTpause(PPM_HandlerTypedef *PPM);

uint16_t PPMgetTsync(PPM_HandlerTypedef *PPM);

void PPMsetFullT(uint16_t Value, PPM_HandlerTypedef *PPM);

void PPMsetTmin(uint16_t Value, PPM_HandlerTypedef *PPM);

void PPmsetTmax(uint16_t Value, PPM_HandlerTypedef *PPM);

void PPMsetTpause(uint16_t Value, PPM_HandlerTypedef *PPM);

void PPMsetTsync(uint16_t Value, PPM_HandlerTypedef *PPM);

#endif /* INC_STCORE_PPM_H_ */
