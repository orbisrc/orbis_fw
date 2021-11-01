/*
 * analog.h
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
 *limitations under the License.
 */

#ifndef INC_STCORE_ANALOG_H_
#define INC_STCORE_ANALOG_H_

#include "stconfig.h"
#include "stm32f4xx_hal.h"


typedef struct {
	uint16_t Value; 				/* Process value*/

	float 	EMA;
	float 	fValue;

	/* ADC */
	uint32_t ADCInputValue; 		/* Input from ADC*/
	uint16_t ADCInputMin; 			/* Min ADC value*/
	uint16_t ADCInputMax; 			/* Max ADC value*/
	uint16_t ADCInputCenter;		/* Center ADC value*/
	uint16_t ADCInputInvert;		/* Inverting ADC value
	 	 	 	 	 	 	 	 	   0 - Disable
	 	 	 	 	 	 	 	 	   1 - Enable*/
	uint16_t ADCvalue; 				/* ADC value after smoothing */
	uint16_t ADCinvertingValue; 	/* ADC value after smoothing */

	/* Base scale */
	uint16_t ScaleMin; 				/* Min value after rate*/
	uint16_t ScaleMax; 				/* Max value after rate*/
	uint16_t ScaleCenter; 				/* Max value after rate*/

	/* Alarm */
	uint16_t HH; 					/* Hight Alarm */
	uint16_t H; 					/* Hight Warning */
	uint16_t L; 					/* Low Warning */
	uint16_t LL; 					/* Low Alarm */
	uint16_t Alarm;					/* Alarm */
	uint16_t Warning;				/* Warning */
#if MOVING_AVARAGE != 4
	uint16_t MAbuff[MA_ITEMS_NUM];
	uint16_t MApointer;
#endif

} AnalogInputTypeDef;

typedef enum
{
	AileronADC,
	ElevatorADC,
	ThrottleADC,
	RudderADC,
	PT1ADC,
	PT2ADC,
	BatteryADC
} ChannelNameTypedef;

extern AnalogInputTypeDef AnalogChannel[ADC1_CH_NUM];

void AnalogInputInit (void);

void AnalogInputMain (void);

void AnalogChannelHandler(AnalogInputTypeDef *Analog);

void AIsetADCinput(uint16_t Value, AnalogInputTypeDef *Analog);

void AIsetADCMin(uint16_t Value,AnalogInputTypeDef *Analog);

void AIsetADCMax(uint16_t Value, AnalogInputTypeDef *Analog);

void AIsetADCCenter(uint16_t Value, AnalogInputTypeDef *Analog);

void AIsetADCEnableInverting(AnalogInputTypeDef *Analog);

void AIsetADCDisableInverting(AnalogInputTypeDef *Analog);

uint16_t AIgetADCMin(AnalogInputTypeDef *Analog);

uint16_t AIgetADCMax(AnalogInputTypeDef *Analog);

uint16_t AIgetADCCenter(AnalogInputTypeDef *Analog);

void AIsetScaleMin(uint16_t Value, AnalogInputTypeDef *Analog);

void AIsetScaleMax(uint16_t Value, AnalogInputTypeDef *Analog);

void AIsetScaleCentre(uint16_t Value, AnalogInputTypeDef *Analog);

uint16_t AIgetScaleMin(AnalogInputTypeDef *Analog);

uint16_t AIgetScaleMax(AnalogInputTypeDef *Analog);

uint16_t AIgetScaleCenter(AnalogInputTypeDef *Analog);

void AIsetHH (uint16_t Value, AnalogInputTypeDef *Analog);

void AIsetH (uint16_t Value, AnalogInputTypeDef *Analog);

void AIsetL (uint16_t Value, AnalogInputTypeDef *Analog);

void AIsetLL (uint16_t Value, AnalogInputTypeDef *Analog);

uint16_t AIgetHH (AnalogInputTypeDef *Analog);

uint16_t AIgetH (AnalogInputTypeDef *Analog);

uint16_t AIgetL (AnalogInputTypeDef *Analog);

uint16_t AIgetLL (AnalogInputTypeDef *Analog);

uint16_t AIgetAlarm (AnalogInputTypeDef *Analog);

uint16_t AIgetWarning (AnalogInputTypeDef *Analog);

uint16_t AIgetValue(AnalogInputTypeDef *Analog);

uint32_t AIgetInputValue(AnalogInputTypeDef *Analog);

uint16_t AIgetADCValue(AnalogInputTypeDef *Analog);

#endif /* INC_STCORE_ANALOG_H_ */
