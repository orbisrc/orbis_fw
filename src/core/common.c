/*
 * common.c
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

#include "tim.h"
#include "core/common.h"
#include "core/encoder.h"
#include "core/periphery.h"
#include "gui/gui.h"
#include "core/rcchannel.h"
#include "core/analog.h"
#include "core/discrete.h"
#include "core/iosettings.h"
#include "core/auxiliary.h"
#include "core/AlarmWarning.h"
#include "core/iosettings.h"
#include "usbd_customhid.h"
#include "usb/usbdatatransfer.h"
#include "core/ppm.h"
#include "usbd_desc.h"
#include "core/buffer.h"
#include "usbd_cdc_if.h"
#include "gui_lvgl/lv_gui.h"

#define RC_LOOP_DIVIDER 80

/*
 * Diagnose
 */

// ADC
__IO uint32_t ADCloopFreq = {0};
__IO uint32_t ADCloopDuration = {0};
LoopFreqHandlerTypedef ADCloop = {0};

// RC Channel
__IO uint32_t RCchLoopFreq = {0};
__IO uint32_t RCchLoopDuration = {0};
LoopFreqHandlerTypedef RCchLoop = {0};

// Main
__IO uint32_t MainLoopFreq = {0};
__IO uint32_t MainLoopDuration = {0};
LoopFreqHandlerTypedef MainLoop = {0};

#if DEBUG_UART_MESSAGE == 1

char dMessage[DEBUG_MESSAGE_MAX] = {0};

#endif

void CommonInit(void)
{
	Encoder_Init();
	BTN_Init();
	BPR_Init();
	AnalogInputInit();
	DiscreteInputInit();
	RCChanelHandlerInit();
	ProfileStorageInit();
	RCtimerInitHandler();

	STreadSettingsFromFlash();

	// GUI_Init();
	//	HAL_Delay(1000);
	PPMhandlerInit();

	lv_gui_init();

	/*
	 * Start loop
	 */
	HAL_TIM_Base_Start_IT(&htim7); /* ADC */

	HAL_Delay(1);

	/*
	 * Start loop
	 */
	HAL_TIM_Base_Start_IT(&htim6);	/* Analog channel
								  * RC channel
								  * Buffer mapping
								  *
								  */
	HAL_TIM_Base_Start_IT(&htim13); /* Beeper */
	HAL_TIM_Base_Start_IT(&htim14); /* Vibro */

	ShortVibro();
	ShortBeep();

	/*
	 * Set stick central, after transmitter powerup.
	 */

	HAL_Delay(1000);

	AIsetADCCenter(AIgetADCValue(&AnalogChannel[0]), &AnalogChannel[AileronADC]);
	AIsetADCCenter(AIgetADCValue(&AnalogChannel[1]), &AnalogChannel[ElevatorADC]);
	//	AIsetADCCenter(AIgetADCValue(&AnalogChannel[2]),&AnalogChannel[ThrottleADC]);
	AIsetADCCenter(AIgetADCValue(&AnalogChannel[3]), &AnalogChannel[RudderADC]);
}

void CommonRun(void)
{
	uint32_t StartTimePoint = 0;

	StartTimePoint = HAL_GetTick();

	// GUI();
	DiscreteInputMain();
	DiscretBufferHandler();
	AlarmWarningHandler();
	BeeperHandler(&MainBeeper);
	RCtimerMain();
	lv_gui();

	/*
	 * Подсчет частоты вызова основного потока выполнения
	 */
	MainLoopDuration = HAL_GetTick() - StartTimePoint;
	CommonLoopFreqCheck(&MainLoop);
}

void CommonLoopFreqCheck(LoopFreqHandlerTypedef *LoopFreqCheck)
{
	uint32_t Period = {0};

	if (LoopFreqCheck->LoopFreqTemp != 0)
	{
		if (LoopFreqCheck->LoopFreqCounter >= 999)
		{
			Period = HAL_GetTick() - LoopFreqCheck->LoopFreqTemp;

			LoopFreqCheck->LoopFreq = 1000000 / Period;

			LoopFreqCheck->LoopFreqTemp = HAL_GetTick();

			LoopFreqCheck->LoopFreqCounter = 0;
		}
		else
		{
			LoopFreqCheck->LoopFreqCounter++;
		}
	}
	else
	{
		LoopFreqCheck->LoopFreqTemp = HAL_GetTick();
	}
}

double CommonLoopGetFreq(LoopFreqHandlerTypedef *LoopFreqCheck)
{
	return LoopFreqCheck->LoopFreq;
}

void RCloop(void)
{
	static uint16_t USBtxDiv = {0};

	RCchLoopDuration = HAL_GetTick();

	AnalogInputMain();
	ChannelBufferHandler();
	RCChanelMain();

	PPMassignmentValues();

	RCchLoopDuration = HAL_GetTick() - RCchLoopDuration;

	if (USBtxDiv < RC_LOOP_DIVIDER)
	{
		USBtxDiv++;
	}
	else
	{
		USB_Handler();
		USBtxDiv = 0;
	}

	CommonLoopFreqCheck(&RCchLoop);
}

uint8_t common_printf(const char *str)
{
	//	uint8_t result = 0;

	if (CommonSettings.USBmode == __VCOM)
	{
		while (CDC_Transmit_FS((uint8_t *)str, strlen(str)) == USBD_BUSY)
		{
		}

		//		result = CDC_Transmit_FS((uint8_t *)str, strlen(str));

		return USBD_OK;
	}

	return USBD_FAIL;
}
