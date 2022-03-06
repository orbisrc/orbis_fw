/*
 * rcchannel.c
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
#include "core/ppm.h"
#include "core/rcchannel.h"
#include "core/rccurve.h"
#include "stdlib.h"
#include "periphery/adc.h"
#include "core/filter.h"
#include "core/analog.h"
#include "core/buffer.h"

RCChanelTypeDef RCChanel[MAX_RC_CHANNEL] = {0};

void RCChanelHandler(RCChanelTypeDef *Chanel)
{

	Chanel->ADCvalueScale = Chanel->ADCInputValue;

	/*
	 * Basic scaling
	 */
	RCChanelBaseScale(Chanel);

	/*
	 *
	 */
	if (Chanel->CurveType != 0 && Chanel->Curve != NULL)
	{
		Chanel->BaseValue = Chanel->Curve[Chanel->BaseValue];
	}

	/*
	 * Rescaling after 0-100% rate
	 */
	RCChanelRateReScale(Chanel);

	/*
	 * Inverting
	 */

	if (Chanel->Invert)
	{
		Chanel->Value = Chanel->BaseScaleMax - Chanel->Value;
	}

	RCChanelGUIScale(Chanel);

	RCChanelAddTrimToResultValue(Chanel);
}

/*
 *
 */
void RCChanelBaseScale(RCChanelTypeDef *Chanel)
{
	if (Chanel->ADCvalueScale < Chanel->ADCCentral)
	{
		Chanel->BaseValue = (uint16_t)((Chanel->ADCvalueScale - Chanel->ADCInputMin) * (Chanel->BaseCentral - Chanel->BaseScaleMin) / (Chanel->ADCCentral - Chanel->ADCInputMin) + Chanel->BaseScaleMin);
	}
	else
	{
		Chanel->BaseValue = (uint16_t)((Chanel->ADCvalueScale - Chanel->ADCCentral) * (Chanel->BaseScaleMax - Chanel->BaseCentral) / (Chanel->ADCInputMax - Chanel->ADCCentral) + Chanel->BaseCentral);
	}
}

void RCChanelAddTrimToResultValue(RCChanelTypeDef *Chanel)
{
	Chanel->Value = Chanel->Value + Chanel->Trim;
}

void RCChanelSetInput(uint32_t Value, RCChanelTypeDef *Chanel)
{

	if (Value > Chanel->ADCInputMax)
	{
		Chanel->ADCInputValue = Chanel->ADCInputMax;
	}
	else if (Value < Chanel->ADCInputMin)
	{
		Chanel->ADCInputValue = Chanel->ADCInputMin;
	}
	else
	{
		Chanel->ADCInputValue = Value;
	}

	RCChanelHandler(Chanel);
}

void RCChanelADCminCalibrate(RCChanelTypeDef *Chanel)
{
	Chanel->ADCInputMin = Chanel->ADCvalue;
}

void RCChanelADCmaxCalibrate(RCChanelTypeDef *Chanel)
{
	Chanel->ADCInputMax = Chanel->ADCvalue;
}

void RCChanelADCcentreCalibrate(RCChanelTypeDef *Chanel)
{
	Chanel->ADCCentral = Chanel->ADCvalue;
}

void RCChanelRateReScale(RCChanelTypeDef *Chanel)
{
	if (Chanel->BaseValue < Chanel->BaseCentral)
	{
		Chanel->Value = (uint16_t)((Chanel->BaseValue - Chanel->BaseScaleMin) * (Chanel->BaseCentral - Chanel->ScaleMin) / (Chanel->BaseCentral - Chanel->BaseScaleMin) + Chanel->ScaleMin);
	}
	else
	{
		Chanel->Value = (uint16_t)((Chanel->BaseValue - Chanel->BaseCentral) * (Chanel->ScaleMax - Chanel->BaseCentral) / (Chanel->BaseScaleMax - Chanel->BaseCentral) + Chanel->BaseCentral);
	}
}

void RCChanelGUIScale(RCChanelTypeDef *Chanel)
{
	if (Chanel->BaseValue < Chanel->BaseCentral)
	{
		Chanel->guiValue = (uint16_t)((Chanel->Value - Chanel->BaseScaleMin) * (Chanel->guiCentral - Chanel->guiScaleMin) / (Chanel->BaseCentral - Chanel->BaseScaleMin) + Chanel->guiScaleMin);
	}
	else
	{
		Chanel->guiValue = (uint16_t)((Chanel->Value - Chanel->BaseCentral) * (Chanel->guiScaleMax - Chanel->guiCentral) / (Chanel->BaseScaleMax - Chanel->BaseCentral) + Chanel->guiCentral);
	}
}

void RCChanelSetLowRate(uint16_t Value, RCChanelTypeDef *Chanel)
{
	Chanel->LowRate = Value;
	Chanel->ScaleMin = (Chanel->BaseCentral - (uint16_t)((Chanel->BaseCentral - Chanel->BaseScaleMin) * Chanel->LowRate / BASE_RATE));
}

void RCChanelSetHightRate(uint16_t Value, RCChanelTypeDef *Chanel)
{
	Chanel->HighRate = Value;
	Chanel->ScaleMax = (Chanel->BaseCentral + (uint16_t)((Chanel->BaseScaleMax - Chanel->BaseCentral) * Chanel->HighRate / BASE_RATE));
}

uint16_t RCChanelGetBaseValue(RCChanelTypeDef *Chanel)
{
	return (uint16_t)Chanel->BaseValue;
}

uint16_t RCChanelGetValue(RCChanelTypeDef *Chanel)
{
	return (uint16_t)Chanel->Value;
}

int16_t RCChanelGetGUIValue(RCChanelTypeDef *Chanel)
{
	return Chanel->guiValue;
}

uint16_t RCChanelGetLowRate(RCChanelTypeDef *Chanel)
{
	return Chanel->LowRate;
}

uint16_t RCChanelGetHighRate(RCChanelTypeDef *Chanel)
{
	return Chanel->HighRate;
}

uint16_t RCChanelGetInvertState(RCChanelTypeDef *Chanel)
{
	return Chanel->Invert;
}

void RCChanelSetInvertState(uint16_t Value, RCChanelTypeDef *Chanel)
{
	if (Value >= 1)
	{
		Chanel->Invert = 1;
	}

	if (Value < 1)
	{
		Chanel->Invert = 0;
	}
}

int16_t RCChanelGetTrim(RCChanelTypeDef *Chanel)
{
	return Chanel->Trim;
}

void RCChanelSetTrim(int32_t Value, RCChanelTypeDef *Chanel)
{
	Chanel->Trim = Value;
}

void RCChanelSetADCCentr(uint32_t Value, RCChanelTypeDef *Chanel)
{
	Chanel->ADCCentral = Value;
}

void RCChanelSetADCMin(uint32_t Value, RCChanelTypeDef *Chanel)
{
	Chanel->ADCInputMin = Value;
}

void RCChanelSetADCMax(uint32_t Value, RCChanelTypeDef *Chanel)
{
	Chanel->ADCInputMax = Value;
}

uint16_t RCChanelGetADCMin(RCChanelTypeDef *Chanel)
{
	return Chanel->ADCInputMin;
}

uint16_t RCChanelGetADCMax(RCChanelTypeDef *Chanel)
{
	return Chanel->ADCInputMax;
}

uint16_t RCChanelGetADCCentr(RCChanelTypeDef *Chanel)
{
	return Chanel->ADCCentral;
}

uint16_t RCChanelGetPPMvalue(RCChanelTypeDef *Chanel)
{
	return 0;
}

void RCChanelSetDeadZone(uint16_t Value, RCChanelTypeDef *Chanel)
{
	Chanel->CentralDeadZone = Value;
}

uint16_t RCChanelGetDeadZone(RCChanelTypeDef *Chanel)
{
	return Chanel->CentralDeadZone;
}

/*
 * 0 - Linear
 * 1 - QuickExpo
 * 2 - CustomExpo
 * 3 - VTR
 */
uint16_t RCChanelGetCurveType(RCChanelTypeDef *Chanel)
{
	return Chanel->CurveType;
}

/*
 * 0 - Linear
 * 1 - QuickExpo
 * 2 - CustomExpo
 * 3 - VTR
 */
void RCChanelSetCurveType(uint16_t Value, RCChanelTypeDef *Chanel)
{
	Chanel->CurveType = Value;
}

/*
 *
 */
uint16_t RCChanelGetExpoX(RCChanelTypeDef *Chanel)
{
	return Chanel->ExpCurveX;
}

uint16_t RCChanelGetExpoY(RCChanelTypeDef *Chanel)
{
	return Chanel->ExpCurveY;
}

void RCChanelSetExpoY(uint16_t Value, RCChanelTypeDef *Chanel)
{
	Chanel->ExpCurveY = Value;
}

/*
 * https://en.wikipedia.org/wiki/B%C3%A9zier_curve
 */
void RCChanelSetExpoX(uint16_t Value, RCChanelTypeDef *Chanel)
{
	Chanel->ExpCurveX = Value;
}

void RCChanelBufferSetItem(uint16_t Value, RCChanelTypeDef *Chanel)
{
	if (Value <= CHANNEL_BUFFER_LENGTH)
	{
		Chanel->ChannelBufferItem = Value;
	}
}

uint16_t RCChanelBufferGetItem(RCChanelTypeDef *Chanel)
{
	return Chanel->ChannelBufferItem;
}

void RCChanelMain()
{
	uint16_t i = 0;

	for (i = 0; i < MAX_RC_CHANNEL; i++) //
	{
		RCChanelSetInput((uint32_t)ChannelBuffer[RCChanelBufferGetItem(&RCChanel[i])], &RCChanel[i]); //
	}
}

void RCChanelInit(RCChanelTypeDef *Chanel)
{
	if (Chanel->ADCInputMin > Chanel->ADCInputMax)
	{
		Chanel->ADCInputMin = Chanel->ADCInputMax;
	}

	if (Chanel->ScaleMin > Chanel->ScaleMax)
	{
		Chanel->ScaleMin = Chanel->ScaleMax;
	}

	Chanel->ADCInputValue = Chanel->ADCCentral;

	RCChanelSetLowRate(Chanel->LowRate, Chanel);
	RCChanelSetHightRate(Chanel->HighRate, Chanel);
}

void RCChanelHandlerInit()
{
	uint16_t i = 0;

	for (i = 0; i < MAX_RC_CHANNEL; i++)
	{
		RCChanel[i].BaseScaleMin = (uint16_t)BASE_CH_MIN;
		RCChanel[i].BaseScaleMax = (uint16_t)BASE_CH_MAX;
		RCChanel[i].BaseCentral = (uint16_t)BASE_CENTRAL;
		RCChanel[i].ADCInputMin = (uint16_t)ADC_MIN;
		RCChanel[i].ADCInputMax = (uint16_t)ADC_MAX;
		RCChanel[i].ADCCentral = (uint16_t)ADC_CENTRAL;
		RCChanel[i].HighRate = (uint16_t)BASE_RATE;
		RCChanel[i].LowRate = (uint16_t)BASE_RATE;
		RCChanel[i].guiScaleMin = (int16_t)GUI_CH_MIN;
		RCChanel[i].guiScaleMax = (int16_t)GUI_CH_MAX;
		RCChanel[i].guiCentral = (int16_t)GUI_CH_CENTRAL;
		RCChanel[i].CurveType = Linear;
		RCChanel[i].ExpCurveX = 0;
		RCChanel[i].ChannelBufferItem = i;
		RCChanel[i].Curve = ___Curve[i];
	}

	for (i = 0; i < MAX_RC_CHANNEL; i++)
	{
		RCChanelInit(&RCChanel[i]);
	}

	CurvesInit();
}
