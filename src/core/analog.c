/*
 * analog.c
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

#include "core/analog.h"
#include "core/filter.h"
#include "core/iosettings.h"
#include "main.h"
#include "adc.h"

/*
 *
 * 0 - ADC1 IN0
 * 1 - ADC1 IN1
 * 2 - ADC1 IN2
 * 3 - ADC1 IN3
 *
 *
 * 4 - ADC1 IN10
 * 5 - ADC1 IN11
 * 6 - ADC1 IN12
 */

AnalogInputTypeDef AnalogChannel[ADC1_CH_NUM] = {0};

void AnalogInputInit (void)
{

	uint16_t i = 0;

	for(i=0;i<ADC1_CH_NUM;i++)
	{
		AnalogChannel[i].ADCInputMin = (uint16_t)ADC_INPUT_MIN;
		AnalogChannel[i].ADCInputMax = (uint16_t)ADC_INPUT_MAX;
		AnalogChannel[i].ADCInputCenter = (uint16_t)ADC_INPUT_CENTRAL;
		AnalogChannel[i].ScaleMin = (uint16_t)ADC_MIN;
		AnalogChannel[i].ScaleMax = (uint16_t)ADC_MAX;
		AnalogChannel[i].ScaleCenter = (uint16_t)ADC_CENTRAL;
		AnalogChannel[i].EMA = EMA_ALPHA;

		__Filter_init(&__FIRFilter[i]);
	}

	/*
	 * Battery voltage channel
	 */
	AnalogChannel[BatteryADC].ADCInputMin = ADC_INPUT_MIN;
	AnalogChannel[BatteryADC].ADCInputMax = ADC_INPUT_MAX;
	AnalogChannel[BatteryADC].ADCInputCenter = (uint16_t)ADC_INPUT_CENTRAL;
	AnalogChannel[BatteryADC].ScaleMin = BATTERY_VOLTAGE_MIN;
	AnalogChannel[BatteryADC].ScaleMax = BATTERY_VOLTAGE_MAX;
	AnalogChannel[BatteryADC].ScaleCenter = (uint16_t)(BATTERY_VOLTAGE_MAX + BATTERY_VOLTAGE_MIN)/2;
	AnalogChannel[BatteryADC].EMA = BATTERY_EMA_ALPHA;
	AIsetL(CommonSettings.BatteryAlarmValue, &AnalogChannel[BatteryADC]);

//	AIsetADCEnableInverting(&AnalogChannel[AileronADC]);
//	AIsetADCEnableInverting(&AnalogChannel[ThrottleADC]);

//	HAL_ADC_Start_IT  (&hadc1);
}

void AnalogInputMain (void)
{
	uint16_t i = 0;

	for(i=0;i<ADC1_CH_NUM;i++)
	{

#if FILTRATION_USE == 0
		/*
		 * None
		 */
		AIsetADCinput((uint16_t) ADCbuff[i] ,&AnalogChannel[i]);
#elif FILTRATION_USE == 1
		/*
		 * FIR
		 */
		AIsetADCinput((uint16_t) __Filter_get(&__FIRFilter[i]) ,&AnalogChannel[i]);
#endif
	}
}

void AnalogChannelHandler(AnalogInputTypeDef *Analog)
{
	uint16_t Data = 0;

	/*
	 *  https://en.wikipedia.org/wiki/Moving_average
	 */
#if MOVING_AVARAGE == 0

	Analog->ADCvalue = Analog->ADCInputValue;

#elif MOVING_AVARAGE == 1

	/*
	 * SMA
	 */
	uint32_t SMAsumm = 0;
	uint16_t i;

	for(i = 0; i < MA_ITEMS_NUM ; i++)
	{
		SMAsumm = SMAsumm + Analog->MAbuff[i];
	}

	Analog->ADCvalue = (uint16_t)(SMAsumm/MA_ITEMS_NUM);

#elif MOVING_AVARAGE == 3

	/*
	 * MWA
	 */

	uint64_t MWAsumm = 0;
	uint64_t MWAgp = 0;
	uint16_t i;

	for(i = 0; i < MA_ITEMS_NUM - 1; i++)
	{
		if ( i <= Analog->MApointer)
		{
			MWAsumm = MWAsumm + (MA_ITEMS_NUM  - Analog->MApointer + i) * Analog->MAbuff[i];

		} else
		{
			MWAsumm = MWAsumm + (i - Analog->MApointer) * Analog->MAbuff[i];
		}

		MWAgp = MWAgp + MA_ITEMS_NUM - i;

	}

	Analog->ADCvalue =  (uint16_t)(MWAsumm / MWAgp);

//	Analog->ADCvalue =  (uint16_t) (2 /(MA_ITEMS_NUM * ( MA_ITEMS_NUM + 1))) * MWAsumm;

#elif MOVING_AVARAGE == 4

	/*
	 * EMA
	 */
	Analog->fValue = (Analog->EMA * Analog->ADCInputValue  + (1 - Analog->EMA) * Analog->fValue);

	Analog->ADCvalue = (uint16_t)Analog->fValue;

#endif


	if(Analog->ADCvalue < Analog->ADCInputMin)
	{
		Data = Analog->ADCInputMin;

	}
	else if (Analog->ADCvalue > Analog->ADCInputMax)
	{
		Data = Analog->ADCInputMax;
	}
	else
	{
		Data = Analog->ADCvalue;
	}


	/*
	 * Scaling
	 */
	if(Data < Analog->ADCInputCenter)
	{
		Analog->Value = (uint16_t)((Data - Analog->ADCInputMin) * (Analog->ScaleCenter - Analog->ScaleMin)
									/(Analog->ADCInputCenter - Analog->ADCInputMin) + Analog->ScaleMin);
	}
	else
	{
		Analog->Value = (uint16_t)((Data - Analog->ADCInputCenter) * (Analog->ScaleMax - Analog->ScaleCenter)
									/(Analog->ADCInputMax - Analog->ADCInputCenter) + Analog->ScaleCenter);
	}

	/*
	 * Inverting signal. Need for stick
	 */
	if(Analog->ADCInputInvert)
	{
		Analog->Value = Analog->ScaleMax -  Analog->Value;
	}


	/*
	 * Alarm and warning
	 */
	if (Analog->Value >= Analog->H && Analog->Value <= Analog->L)
	{
		Analog->Warning = 1;
	}
	else
	{
		Analog->Warning = 0;
	}

	if (Analog->Value >= Analog->HH && Analog->Value <= Analog->LL)
	{
		Analog->Alarm = 1;
	}
	else
	{
		Analog->Alarm = 0;
	}
}

void AIsetADCinput(uint16_t Value, AnalogInputTypeDef *Analog)
{
#if MOVING_AVARAGE == 0

	Analog->ADCInputValue = Value;

#elif MOVING_AVARAGE == 1

	Analog->MAbuff[Analog->MApointer++] = Value;

	if (Analog->MApointer >= MA_ITEMS_NUM )
	{
		Analog->MApointer = 0;
	}


#elif MOVING_AVARAGE == 3

//	int16_t i = 0;

//	for(i = MA_ITEMS_NUM - 1 ; i > 0; i--)
//	{
//		Analog->MAbuff[i] = Analog->MAbuff[i - 1] ;
//	}
//
//	Analog->MAbuff[0] = Value;

	Analog->MAbuff[Analog->MApointer++] = Value;

	if (Analog->MApointer >= MA_ITEMS_NUM )
	{
		Analog->MApointer = 0;
	}


#elif MOVING_AVARAGE == 4

	Analog->ADCInputValue = Value;

#endif


	AnalogChannelHandler(Analog);
}

void AIsetADCMin(uint16_t Value,AnalogInputTypeDef *Analog)
{
	Analog->ADCInputMin = Value;
}

void AIsetADCMax(uint16_t Value, AnalogInputTypeDef *Analog)
{
	Analog->ADCInputMax = Value;
}

void AIsetADCCenter(uint16_t Value, AnalogInputTypeDef *Analog)
{
	Analog->ADCInputCenter = Value;
}

uint16_t AIgetADCMin(AnalogInputTypeDef *Analog)
{
	return Analog->ADCInputMin;
}

uint16_t AIgetADCMax(AnalogInputTypeDef *Analog)
{
	return Analog->ADCInputMax;
}

uint16_t AIgetADCCenter(AnalogInputTypeDef *Analog)
{
	return Analog->ADCInputCenter;
}

void AIsetScaleMin(uint16_t Value, AnalogInputTypeDef *Analog)
{
	Analog->ScaleMin = Value;
}

void AIsetScaleMax(uint16_t Value, AnalogInputTypeDef *Analog)
{
	Analog->ScaleMax = Value;
}

void AIsetScaleCentre(uint16_t Value, AnalogInputTypeDef *Analog)
{
	Analog->ScaleCenter = Value;
}

uint16_t AIgetScaleMin(AnalogInputTypeDef *Analog)
{
	return Analog->ScaleMin;
}

uint16_t AIgetScaleMax(AnalogInputTypeDef *Analog)
{
	return Analog->ScaleMax;
}

uint16_t AIgetScaleCentre(AnalogInputTypeDef *Analog)
{
	return Analog->ScaleCenter;
}

void AIsetHH (uint16_t Value, AnalogInputTypeDef *Analog)
{
	Analog->HH = Value;
}

void AIsetH (uint16_t Value, AnalogInputTypeDef *Analog)
{
	Analog->H = Value;
}

void AIsetL (uint16_t Value, AnalogInputTypeDef *Analog)
{
	Analog->L = Value;
}

void AIsetLL (uint16_t Value, AnalogInputTypeDef *Analog)
{
	Analog->LL = Value;
}

uint16_t AIgetHH (AnalogInputTypeDef *Analog)
{
	return Analog->HH;
}

uint16_t AIgetH (AnalogInputTypeDef *Analog)
{
	return Analog->H;
}

uint16_t AIgetL (AnalogInputTypeDef *Analog)
{
	return Analog->L;
}

uint16_t AIgetLL (AnalogInputTypeDef *Analog)
{
	return Analog->LL;
}

uint16_t AIgetAlarm (AnalogInputTypeDef *Analog)
{
	return Analog->Alarm;
}

uint16_t AIgetWarning (AnalogInputTypeDef *Analog)
{
	return Analog->Warning;
}

uint16_t AIgetValue(AnalogInputTypeDef *Analog)
{
	return Analog->Value;
}

uint32_t AIgetInputValue(AnalogInputTypeDef *Analog)
{
	return Analog->ADCInputValue;
}

uint16_t AIgetADCValue(AnalogInputTypeDef *Analog)
{
	return Analog->ADCvalue;
}

void AIsetADCEnableInverting(AnalogInputTypeDef *Analog)
{
	Analog->ADCInputInvert = 1;
}
