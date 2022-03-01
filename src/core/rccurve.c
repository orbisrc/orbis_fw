/*
 * rccurve.c
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

#include <core/rcchannel.h>
#include <core/rccurve.h>
#include "stdlib.h"
#include "math.h"
#include "stconfig.h"

STbezierCurveTypeDef STcurveBezierConf = {0};
STbezierCurveTypeDef THcurveBezierConf = {0};

uint16_t ___Curve[MAX_RC_CHANNEL][CURVE_SIZE] = {0};
STbezierCurveTypeDef ___CurveConf[MAX_RC_CHANNEL] = {0};

void CurvesInit(void)
{

	uint16_t i = 0;

	for (i = 0; i < MAX_RC_CHANNEL; i++)
	{
		LinearBuffFill(___Curve[i]);
		STBezierCurveInit(&___CurveConf[i]);
		STcurveFill(i);
	}
}

void STBezierCurveInit(STbezierCurveTypeDef *STbezierCurve)
{
	STbezierCurve->Px0 = BASE_CH_MIN;
	STbezierCurve->Py0 = BASE_CH_MIN;
	STbezierCurve->Px1 = BASE_CH_MAX / 4;
	STbezierCurve->Py1 = BASE_CH_MAX / 4;
	STbezierCurve->Px2 = BASE_CH_MAX / 2;
	STbezierCurve->Py2 = BASE_CH_MAX / 2;
}

void LinearBuffFill(uint16_t *__Buff)
{
	uint16_t i = 0;

	for (i = 0; i < CURVE_SIZE; i++)
	{
		*__Buff = i;
		__Buff++;
	}
}

void STcurveFill(uint16_t Channel)
{
	switch (RCChanel[Channel].CurveType)
	{
	case Linear:
		LinearBuffFill(RCChanel[Channel].Curve);
		break;

	case QuickExponential:

		STsetBezierQuickRait(RCChanelGetExpoX(&RCChanel[Channel]), &___CurveConf[Channel]);
		STcurveBezierFill(Channel);

		break;

	case Exponential:

		STsetBezierCurvePx1(RCChanelGetExpoX(&RCChanel[Channel]), &___CurveConf[Channel]);
		STsetBezierCurvePy1(RCChanelGetExpoY(&RCChanel[Channel]), &___CurveConf[Channel]);
		STcurveBezierFill(Channel);

		break;

	default:
		break;
	}
}

void STsetBezierCurvePx1(int16_t x, STbezierCurveTypeDef *STbezierCurve)
{
	STbezierCurve->Px1 = (uint16_t)(x - EXPO_RATE_MIN) * (BASE_CH_MAX / 2 - BASE_CH_MIN) / (EXPO_RATE_MAX - EXPO_RATE_MIN + BASE_CH_MIN);
}

void STsetBezierCurvePy1(int16_t y, STbezierCurveTypeDef *STbezierCurve)
{
	STbezierCurve->Py1 = (uint16_t)(y - EXPO_RATE_MIN) * (BASE_CH_MAX / 2 - BASE_CH_MIN) / (EXPO_RATE_MAX - EXPO_RATE_MIN + BASE_CH_MIN);
}

void STsetBezierQuickRait(uint16_t x, STbezierCurveTypeDef *STbezierCurve)
{

	STsetBezierCurvePx1(x, STbezierCurve);

	STbezierCurve->Py1 = STbezierCurve->Px2 - STbezierCurve->Px1;
}

uint16_t STgetBezierCurvePx1(STbezierCurveTypeDef *STbezierCurve)
{
	return STbezierCurve->Px1;
}

void STcurveBezierFill(uint16_t Channel)
{
	uint16_t i = 0;

	for (i = 0; i < (STCURVE_SIZE - 1) / 2; i++)
	{
		___Curve[Channel][i] = STgetBezierCurveY(i, &___CurveConf[Channel]);
		___Curve[Channel][STCURVE_SIZE - i - 1] = STCURVE_SIZE - 1 - STgetBezierCurveY(i, &___CurveConf[Channel]);
	}
}

uint16_t STgetBezierCurveY(uint16_t x, STbezierCurveTypeDef *STbezierCurve)
{
	double t = 0;

	uint16_t Px0 = STbezierCurve->Px0;
	uint16_t Py0 = STbezierCurve->Py0;
	uint16_t Px1 = STbezierCurve->Px1;
	uint16_t Py1 = STbezierCurve->Py1;
	uint16_t Px2 = STbezierCurve->Px2;
	uint16_t Py2 = STbezierCurve->Py2;
	uint16_t y = 0;

	if ((Px0 - 2 * Px1 + Px2) != 0)
	{
		t = (Px0 - Px1 + sqrt((Px0 - 2 * Px1 + Px2) * x + pow(Px1, 2)) - Px0 * Px2) / (Px0 - 2 * Px1 + Px2);

		y = (uint16_t)(Py0 * pow((1 - t), 2) + 2 * t * Py1 * (1 - t) + Py2 * pow(t, 2));
	}
	else
	{
		y = x;
	}
	return y;
}