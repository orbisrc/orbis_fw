/*
 * STconfig.h
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

#ifndef STCONFIG_H_
#define STCONFIG_H_

#include "version.h"
#include "target.h"

//#define __ORBIS_AIR__
//#define __ORBIS_SURFACE__

/**********************************
 *Common
 *********************************/

#define USB_MODE_NUMDER 3 /* 0 - Virtual COM          \
						   *  1 - Joystick            \
						   *  3 - Mass storage device \
						   */

#define DEBUG_UART_MESSAGE 0 /* 0 - Disable                \
							  *  1 - Enable														* \
							  */

#define DEBUG_UART_MESSAGE_GUI 0 /* 0 - Disable                \
								  *  1 - Enable														* \
								  */
#define DEBUG_MESSAGE_MAX 255

#define CPU_FREQ 84000000
#define RC_LOOP 4000
#define ADC_LOOP 4000
#define RC_LOOP_DIV (uint32_t)((CPU_FREQ / (RC_LOOP_PERIOD + 1) / RC_LOOP) - 1)
#define ADC_LOOP_DIV (uint32_t)((CPU_FREQ / (ADC_LOOP_PERIOD + 1) / ADC_LOOP) - 1)
#define RC_LOOP_PERIOD 999
#define ADC_LOOP_PERIOD 999

/**********************************
 *Memory
 *********************************/
#define MODEL_MEMORY_NUM 12
#define MODEL_FLASH_NUM MODEL_MEMORY_NUM + 1

#define MODEL_PROFILE_STORAGE 1 /* 0 - Internal flash \
								 * 1 - W25Qxx         \
								 */

#define MAX_RC_CHANNEL 16
#define MAX_RC_NAME 16

/**********************************
 * PPM
 *********************************/

#define PPM_CH_NUM 8
#define PPM_FULL_DURATION 22500
#define PPM_CHANNEL_DURATION 2000
#define PPM_PAUSE 300
#define PPM_TMIN 500
#define PPM_TMAX PPM_CHANNEL_DURATION - PPM_PAUSE
#define PPM_MIN PPM_TMIN + TRIM_CH_MAX
#define PPM_MAX PPM_TMAX - TRIM_CH_MAX

/**********************************
 * Display
 **********************************/

#define BRIGHTNESS_MIN 0
#define BRIGHTNESS_MAX 4095

/**********************************
 * Discret buffer
 **********************************/

#define DISCRETE_BUFFER_LENGTH 32

/**********************************
 * ADC channel
 **********************************/

#define ADC1_CH_NUM 7										  //
#define ADC3_CH_NUM 1										  //
#define ADC_MIN 0											  //
#define ADC_MAX 4095										  //
#define ADC_CENTRAL (ADC_MAX + ADC_MIN) / 2					  //
#define ADC_INPUT_MIN 100									  //
#define ADC_INPUT_MAX 4000									  // 4101			//
#define ADC_INPUT_CENTRAL (ADC_INPUT_MAX + ADC_INPUT_MIN) / 2 //
#define CHANNEL_BUFFER_LENGTH 32
#define ADC_DEADBAND 10

#if ADC_LOOP > 2000
#define EMA_ALPHA 0.02
#elif ADC_LOOP < 2000
#define EMA_ALPHA 0.05
#else
#define EMA_ALPHA 0.09
#endif

#define BATTERY_EMA_ALPHA 0.001
#define MA_ITEMS_NUM 200
#define MOVING_AVARAGE 4 /* 0 - None                \
						  *  1 - SMA                \
						  *  2 - CMA                \
						  *  3 - WMA                \
						  *  4 - EWA														* \
						  */
#define FILTRATION_USE 1 /* 0 - None \
						  *  1 - FIR \
						  */

/**********************************
 * Battery
 **********************************/

#define BATTERY_VOLTAGE_MAX 840		//
#define BATTERY_VOLTAGE_MIN 0		//
#define BATTERY_VOLTAGE_BAR_MIN 330 //
#define BATTERY_VOLTAGE_BAR_MAX 420 //

/**********************************
 * RC channel
 **********************************/

#define GUI_CH_CENTRAL 0									   //
#define GUI_CH_MIN -100										   //
#define GUI_CH_MAX 100										   //
#define BASE_CH_MIN 0										   //
#define BASE_CH_MAX 1000									   //
#define BASE_CENTRAL (uint16_t)(BASE_CH_MAX + BASE_CH_MIN) / 2 //
#define DEADBAND 3											   //
#define TRIM_CH_MIN -100									   //
#define TRIM_CH_MAX 100										   //
#define PULSE_BASE_MIN 100									   //
#define PULSE_BASE_MAX 900									   //
#define BASE_RATE 100										   //
#define STCURVE_SIZE BASE_CH_MAX + 1						   //
#define THCURVE_SIZE BASE_CH_MAX + 1						   //
#define CURVE_SIZE BASE_CH_MAX + 1							   //
#define FILTER_BUFFER_SIZE 100								   //
#define MAX_CURVE_POINT 6									   //
#define EXPO_RATE_MIN -100
#define EXPO_RATE_MAX 100

/**********************************
 * Discrete channel
 **********************************/

#define DISCRETE_CH_NUM 6			   //
#define DISCRETE_FILTER_DURATION 50	   //
#define DISCRETE_VALUE_HIGHT ADC_MAX   //
#define DISCRETE_VALUE_MID ADC_CENTRAL //
#define DISCRETE_VALUE_LOW ADC_MIN	   //

/**********************************
 * Lap list
 **********************************/
#define LINE_NUM 12 //
#define ROW_NUM 2	//
#define LAPS_NUM LINE_NUM *ROW_NUM

/**********************************
 * Beeper
 **********************************/
#define SHORT_BEEP_DURATION 120
#define LONG_BEEP_DURATION 600
#define SHORT_VIBRO_DURATION 800
#define LONG_VIBRO_DURATION 2000
#define BATTERY_ALARM_PAUSE_DURATION 800
#define BATTERY_ALARM_FULL_DURATION 10000
#define TIMER_ALARM_PAUSE_DURATION 800
#define TIMER_ALARM_FULL_DURATION 10000
#define INACTIVITY_ALARM_PAUSE_DURATION 800
#define INACTIVITY_ALARM_FULL_DURATION 5000

/**********************************
 * Basic settings
 **********************************/

#define BATTERY_ALARM_LOW 330  /**/
#define INACTIVITY_TIME 15	   /* minutes */
#define INACTIVITY_TIME_MIN 1  /* minutes */
#define INACTIVITY_TIME_MAX 90 /* minutes */
#define INACTIVITY_DEADBAND 40

/*
 * INFO
 */

#define HW_VERSION PCB_VERSION_STRING
#define SW_VERSION FW_VERSION_STRING
#define SW_DATE __DATE__

#endif /* STCONFIG_H_ */
