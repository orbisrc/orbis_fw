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

typedef enum _RCCurveTypedef
{
	Linear,
	QuickExponential,
	Exponential,
	Curve,
	VTR
} RCCurveTypedef;

typedef enum 
{
  RC_OK       = 0x00U,
  RC_ERROR    = 0x01U,
  RC_BUSY     = 0x02U,
  RC_TIMEOUT  = 0x03U
} RC_StatusTypeDef;

typedef struct _RCChanelTypeDef
{
	uint16_t Value;	  /* Process value*/
	int16_t guiValue; /* Value for GUI*/

	/* ADC */
	uint32_t ADCInputValue; /* Input from ADC*/
	uint16_t ADCInputMin;	/* Min ADC value*/
	uint16_t ADCInputMax;	/* Max ADC value*/
	uint16_t ADCCentral;	/* ADC central value*/
	uint16_t ADCvalue;		/* ADC value after smoothing */
	uint16_t ADCvalueScale; /* ADC value after smoothing and scaling*/
	uint16_t ADCinvert;		/* ADC data invert*/

	/* Base scale */
	uint16_t BaseScaleMin;	  /* Min value*/
	uint16_t BaseScaleMax;	  /* Max value */
	uint16_t BaseCentral;	  /* Central  value */
	int16_t guiScaleMin;	  /* Min value*/
	int16_t guiScaleMax;	  /* Max value */
	int16_t guiCentral;		  /* Central  value */
	uint16_t BaseValue;		  /* Base value for channel generate */
	uint16_t CentralValue;	  /* Channel center*/
	uint16_t ScaleMin;		  /* Min value after rate*/
	uint16_t ScaleMax;		  /* Max value after rate*/
	uint16_t CentralDeadZone; /* Dead zone*/
	uint16_t LowRate;		  /* Throttle or Steering rates 0 - 100% ( 0 - 1000 ) */
	uint16_t HighRate;		  /* Throttle or Steering rates 0 - 100% ( 0 - 1000 ) */
	int16_t Trim;			  /* Channel trimmer -100...0...100 */
	uint16_t Invert;
	uint16_t failsafeValue; /* Failsafe value*/

	/* Curves  */
	uint16_t CurveType;
	int16_t ExpCurveX;
	int16_t ExpCurveY;
	uint16_t *Curve; /* Curve Array */

	/* Buffer */
	uint16_t ChannelBufferItem; /* Channel number from common input buffer */

} RCChanelTypeDef;

extern RCChanelTypeDef RCChanel[MAX_RC_CHANNEL];

extern uint16_t ___is_failsafe_changed;

void RCChanelPutADCData(RCChanelTypeDef *Chanel);

void RCChanelHandler(RCChanelTypeDef *Chanel);

void RCChanelInit(RCChanelTypeDef *Chanel);

void RCChanelHandlerInit(void);

void RCChanelMain(void);

void RCChanelADCminCalibrate(RCChanelTypeDef *Chanel);

void RCChanelADCmaxCalibrate(RCChanelTypeDef *Chanel);

void RCChanelADCcentreCalibrate(RCChanelTypeDef *Chanel);

void RCChanelSetInput(uint32_t Value, RCChanelTypeDef *Chanel);

void RCChanelSetADCMin(uint32_t Value, RCChanelTypeDef *Chanel);

void RCChanelSetADCMax(uint32_t Value, RCChanelTypeDef *Chanel);

void RCChanelSetADCCenter(uint32_t Value, RCChanelTypeDef *Chanel);

void RCChanelSetCenter(uint32_t Value, RCChanelTypeDef *Chanel);

void RCChanelSetTrim(int32_t Value, RCChanelTypeDef *Chanel);

void RCChanelSetLowRate(uint16_t Value, RCChanelTypeDef *Chanel);

void RCChanelSetHightRate(uint16_t Value, RCChanelTypeDef *Chanel);

void RCChanelSetInvertState(uint16_t Value, RCChanelTypeDef *Chanel);

void RCChanelRateReScale(RCChanelTypeDef *Chanel);

void RCChanelBaseScale(RCChanelTypeDef *Chanel);

void RCChanelAddTrimToResultValue(RCChanelTypeDef *Chanel);

void RCChanelGUIScale(RCChanelTypeDef *Chanel);

void RCChanelPPMscale(RCChanelTypeDef *Chanel);

uint16_t RCChanelGetPPMvalue(RCChanelTypeDef *Chanel);

uint16_t RCChanelGetLowRate(RCChanelTypeDef *Chanel);

uint16_t RCChanelGetHighRate(RCChanelTypeDef *Chanel);

int16_t RCChanelGetTrim(RCChanelTypeDef *Chanel);

int16_t RCChanelGetGUIValue(RCChanelTypeDef *Chanel);

uint16_t RCChanelGetValue(RCChanelTypeDef *Chanel);

uint16_t RCChanelGetBaseValue(RCChanelTypeDef *Chanel);

uint16_t RCChanelGetInvertState(RCChanelTypeDef *Chanel);

uint16_t RCChanelGetADCMin(RCChanelTypeDef *Chanel);

uint16_t RCChanelGetADCMax(RCChanelTypeDef *Chanel);

uint16_t RCChanelGetADCCentr(RCChanelTypeDef *Chanel);

void RCChanelSetDeadZone(uint16_t Value, RCChanelTypeDef *Chanel);

uint16_t RCChanelGetDeadZone(RCChanelTypeDef *Chanel);

uint16_t RCChanelGetCurveType(RCChanelTypeDef *Chanel);

void RCChanelSetCurveType(uint16_t Value, RCChanelTypeDef *Chanel);

uint16_t RCChanelGetExpoX(RCChanelTypeDef *Chanel);

void RCChanelSetExpoX(uint16_t Value, RCChanelTypeDef *Chanel);

uint16_t RCChanelGetExpoY(RCChanelTypeDef *Chanel);

void RCChanelSetExpoY(uint16_t Value, RCChanelTypeDef *Chanel);

void RCChanelSetInvertState(uint16_t Value, RCChanelTypeDef *Chanel);

void RCChanelAddCurvePoint(RCChanelTypeDef *Chanel);

void RCChanelDelCurvePoint(RCChanelTypeDef *Chanel);

uint16_t RCChanelGetCurveNumber(RCChanelTypeDef *Chanel);

void RCChanelSetPointPosition(uint16_t Value, uint16_t Point, RCChanelTypeDef *Chanel);

void RCChanelSetPointRatio(uint16_t Value, uint16_t Point, RCChanelTypeDef *Chanel);

uint16_t RCChanelGetPointPosition(uint16_t Point, RCChanelTypeDef *Chanel);

uint16_t RCChanelGetPointRatio(uint16_t Point, RCChanelTypeDef *Chanel);

uint16_t RCChanelPointNumChange(RCChanelTypeDef *Chanel);

uint32_t RCChanelFilterHandler(uint32_t Value, RCChanelTypeDef *Chanel);

void RCChanelFilterOn(RCChanelTypeDef *Chanel);

void RCChanelFilterOff(RCChanelTypeDef *Chanel);

void RCChanelFilterSetN(uint16_t Value, RCChanelTypeDef *Chanel);

uint16_t RCChanelFilterGetN(RCChanelTypeDef *Chanel);

uint16_t RCChanelFilterGetState(RCChanelTypeDef *Chanel);

void RCChanelFilterSetState(uint16_t Value, RCChanelTypeDef *Chanel);

void RCChanelBufferSetItem(uint16_t Value, RCChanelTypeDef *Chanel);

uint16_t RCChanelBufferGetItem(RCChanelTypeDef *Chanel);

void RCChanelSetFailsafeValue(uint16_t Value, RCChanelTypeDef *Chanel);

uint16_t RCChanelGetFailsafeValue(RCChanelTypeDef *Chanel);

void failsafeNewValueSetted();

void failsafeValueChanged();

uint16_t isFailsafeChanged();

#endif /* ST_RCCHANNEL_H_ */
