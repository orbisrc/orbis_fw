/*
 * rccurve.h
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

#ifndef ST_RCCURVE_H_
#define ST_RCCURVE_H_

#include "stm32f4xx_hal.h"
#include "stconfig.h"

/*
 * https://en.wikipedia.org/wiki/B%C3%A9zier_curve
 */

typedef struct
{
	uint16_t Px0;
	uint16_t Py0;
	uint16_t Px1;
	uint16_t Py1;
	uint16_t Px2;
	uint16_t Py2;
} STbezierCurveTypeDef;

extern STbezierCurveTypeDef STcurveBezierConf;
extern STbezierCurveTypeDef THcurveBezierConf;
extern uint16_t ___Curve[MAX_RC_CHANNEL][CURVE_SIZE];

void LinearBuffFill(uint16_t *__Buff);

void THcurveFill(void);

void STcurveFill(uint16_t Channel);

void CurvesInit(void);

void STBezierCurveInit(STbezierCurveTypeDef *STbezierCurve);

void STsetBezierCurvePx1(int16_t x, STbezierCurveTypeDef *STbezierCurve);

void STsetBezierCurvePy1(int16_t x, STbezierCurveTypeDef *STbezierCurve);

void STsetBezierQuickRait(uint16_t x, STbezierCurveTypeDef *STbezierCurve);

uint16_t STgetBezierCurvePx1(STbezierCurveTypeDef *STbezierCurve);

uint16_t STgetBezierCurvePy1(STbezierCurveTypeDef *STbezierCurve);

uint16_t STgetBezierCurveY(uint16_t x, STbezierCurveTypeDef *STbezierCurve);

void STcurveBezierFill(uint16_t Channel);

void STthCurveBezierFill(void);

void STthPointCurveFill(void);

#endif /* ST_RCCURVE_H_ */
