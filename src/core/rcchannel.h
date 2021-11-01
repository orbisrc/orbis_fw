/*
 * rcchannel.h
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

#ifndef ST_RCCHANNEL_H_
#define ST_RCCHANNEL_H_

#include <core/filter.h>
#include "stm32f4xx_hal.h"
#include "stconfig.h"

typedef enum {
	ADCNonCplt, ADCCplt
} RCCganelEnumTypedef;

/*
 * ���� ������ ����������
 */
typedef enum {
	Linear, QuickExponential, Exponential, Curve, VTR
} RCCurveTypedef;

typedef enum {
	rcch_No, rccsh_Yes
} RCchTypedef;

/*
 * PPM ������
 */
typedef enum {
	PPM_Ch0, PPM_Ch1, PPM_Ch2, PPM_Ch3, PPM_Ch4, PPM_Ch5, PPM_Ch6, PPM_Ch7
} PPM_CHNameTypeDef;

/*
 * ��������� ������ ����������
 */
typedef struct {
	uint16_t Value; 				/* Process value*/

	/* ADC */
	uint32_t ADCInputValue; 		/* Input from ADC*/
	uint16_t ADCInputMin; 			/* Min ADC value*/
	uint16_t ADCInputMax; 			/* Max ADC value*/
	uint16_t ADCCentral; 			/* ADC central value*/
	uint16_t ADCvalue; 				/* ADC value after smoothing */
	uint16_t ADCvalueScale;			/* ADC value after smoothing and scaling*/
	uint16_t ADCinvert;				/* ADC data invert*/

	/* Base scale */
	uint16_t BaseScaleMin; 			/* Min value*/
	uint16_t BaseScaleMax; 			/* Max value */
	uint16_t BaseCentral; 			/* Max value */
	uint16_t BaseValue; 			/* Base value for channel generate */
	uint16_t CentralValue; 			/* Channel center*/
	uint16_t ScaleMin; 				/* Min value after rate*/
	uint16_t ScaleMax; 				/* Max value after rate*/
	uint16_t CentralDeadZone; 		/* Dead zone*/
	uint16_t LowRate; 				/* Throttle or Steering rates 0 - 100% ( 0 - 1000 ) */
	uint16_t HighRate; 				/* Throttle or Steering rates 0 - 100% ( 0 - 1000 ) */
	int16_t Trim; 					/* Channel trimmer -100...0...100 */
	uint16_t Invert;

//	/* PPM */
//	uint16_t PPMvalue; 				/* �������� ��� ���������� PPM. 500...1500 ���*/
//	uint16_t PPMmin; 				/* */
//	uint16_t PPMmax; 				/* */


	/* Curves  */
	uint16_t CurveType;
	int16_t ExpCurveX;
	int16_t ExpCurveY;
	uint16_t *Curve; 			/* Curve Array */

	/* Buffer */
	uint16_t ChannelBufferItem;

} RCChanelTypeDef;

extern RCChanelTypeDef  RCChanel[MAX_RC_CHANNEL];


/*
 * � ���� ����� ������������ ����� ������ ���������� ������. ���������� � ����������.
 * � ��������� ������ ���������� ���� ������� ������� ������, �� �������� ������� �������� ��� �����������
 */

void RCChanelPutADCData(RCChanelTypeDef *Chanel);

void RCChanelHandler(RCChanelTypeDef *Chanel);	// ���������� ������� ����������

void RCChanelInit(RCChanelTypeDef *Chanel);				// ������������� ������

void RCChanelHandlerInit(void);

void RCChanelMain(void);

void RCChanelADCminCalibrate (RCChanelTypeDef *Chanel);

void RCChanelADCmaxCalibrate (RCChanelTypeDef *Chanel);

void RCChanelADCcentreCalibrate (RCChanelTypeDef *Chanel);

void RCChanelSetInput(uint32_t Value, RCChanelTypeDef *Chanel);	// �������� ������ � ADC � ����������

void RCChanelSetADCMin(uint32_t Value, RCChanelTypeDef *Chanel);// ��������� �������� ����� �� ����� � ADC

void RCChanelSetADCMax(uint32_t Value, RCChanelTypeDef *Chanel);

void RCChanelSetADCCentr(uint32_t Value, RCChanelTypeDef *Chanel);

void RCChanelSetCenter(uint32_t Value, RCChanelTypeDef *Chanel); // ��������� ������� ��������������� ������������ ��������� ������

void RCChanelSetTrim(int32_t Value, RCChanelTypeDef *Chanel);// ���������� �������� �������� -100.....100

void RCChanelSetLowRate(uint16_t Value, RCChanelTypeDef *Chanel);// ��������� �������� ������ ������� ����� 0...100%

void RCChanelSetHightRate(uint16_t Value, RCChanelTypeDef *Chanel);	// ��������� �������� ������� ������� ����� 0...100%

void RCChanelSetInvertState(uint16_t Value, RCChanelTypeDef *Chanel);// ������������� �����

void RCChanelRateReScale(RCChanelTypeDef *Chanel);// ���������� �������� �������� � ������ ������� �����

void RCChanelBaseScale(RCChanelTypeDef *Chanel);// �������������� ��� ������ �����

void RCChanelPPMscale(RCChanelTypeDef *Chanel);	// ���������� �������� ��� PPM ����������

uint16_t RCChanelGetPPMvalue(RCChanelTypeDef *Chanel);// ������� ���������� �������� PMM

uint16_t RCChanelGetLowRate(RCChanelTypeDef *Chanel);// ������� ��������� �������� ������ ������� �����

uint16_t RCChanelGetHighRate(RCChanelTypeDef *Chanel);// ������� ��������� �������� ������� ������� �����

int16_t RCChanelGetTrim(RCChanelTypeDef *Chanel);// ������� ���������� �������� �������� ������

uint16_t RCChanelGetValue(RCChanelTypeDef *Chanel);	// ������� ���������� ���������� �������� ������

uint16_t RCChanelGetBaseValue(RCChanelTypeDef *Chanel);	// ������� ���������� ���������� �������� ��� ����� ������� �����

uint16_t RCChanelGetInvertState(RCChanelTypeDef *Chanel);// ������� ���������� ��������� �������������� 1 - ������������, 0 - �� �����������

uint16_t RCChanelGetADCMin(RCChanelTypeDef *Chanel);// ����������� �������� � ADC

uint16_t RCChanelGetADCMax(RCChanelTypeDef *Chanel);// ������������ �������� � ADC

uint16_t RCChanelGetADCCentr(RCChanelTypeDef *Chanel);// ������������ �������� � ADC

void RCChanelSetDeadZone(uint16_t Value, RCChanelTypeDef *Chanel);// ���������� �������� ������� ����

uint16_t RCChanelGetDeadZone(RCChanelTypeDef *Chanel);// ������� �������� ������� ����

/*
 * ������ ����������
 */

uint16_t RCChanelGetCurveType(RCChanelTypeDef *Chanel);	// ��� ������ ����������

void RCChanelSetCurveType(uint16_t Value, RCChanelTypeDef *Chanel);	// ���������� ��� ������ ����������

uint16_t RCChanelGetExpoX(RCChanelTypeDef *Chanel);	// ���������� P1 ������ �����

void RCChanelSetExpoX(uint16_t Value, RCChanelTypeDef *Chanel);	// ���������� ���������� P1 ������ �����

uint16_t RCChanelGetExpoY(RCChanelTypeDef *Chanel);	// ���������� P1 ������ �����

void RCChanelSetExpoY(uint16_t Value, RCChanelTypeDef *Chanel);	// ���������� ���������� P1 ������ �����

void RCChanelSetInvertState(uint16_t Value, RCChanelTypeDef *Chanel);// ������������� �����. 1 - ����������� 0 - �� �������������

void RCChanelAddCurvePoint(RCChanelTypeDef *Chanel);

void RCChanelDelCurvePoint(RCChanelTypeDef *Chanel);

uint16_t RCChanelGetCurveNumber(RCChanelTypeDef *Chanel);

void RCChanelSetPointPosition(uint16_t Value, uint16_t Point, RCChanelTypeDef *Chanel);

void RCChanelSetPointRatio(uint16_t Value, uint16_t Point, RCChanelTypeDef *Chanel);

uint16_t RCChanelGetPointPosition(uint16_t Point, RCChanelTypeDef *Chanel);

uint16_t RCChanelGetPointRatio(uint16_t Point, RCChanelTypeDef *Chanel);

uint16_t RCChanelPointNumChange(RCChanelTypeDef *Chanel);

/*
 * ���������� �������
 */
uint32_t RCChanelFilterHandler(uint32_t Value, RCChanelTypeDef *Chanel);// ���������� ������� ������

void RCChanelFilterOn(RCChanelTypeDef *Chanel);	// �������� ���������� �������

void RCChanelFilterOff(RCChanelTypeDef *Chanel);// ��������� ���������� �������

void RCChanelFilterSetN(uint16_t Value, RCChanelTypeDef *Chanel);// ���������� �������� ���� ���������� �������

uint16_t RCChanelFilterGetN(RCChanelTypeDef *Chanel);// ��������� ������� �������� ���� ���������� �������

uint16_t RCChanelFilterGetState(RCChanelTypeDef *Chanel);// 1 - �������, 0 - ��������

void RCChanelFilterSetState(uint16_t Value, RCChanelTypeDef *Chanel);// 1 - �������, 0 - ��������


/*
 *  Buffer
 */

void RCChanelBufferSetItem(uint16_t Value, RCChanelTypeDef *Chanel);

uint16_t RCChanelBufferGetItem(RCChanelTypeDef *Chanel);

#endif /* ST_RCCHANNEL_H_ */
