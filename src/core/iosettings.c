/*
 * iosettings.c
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

#include <stdio.h>
#include <string.h>
#include <core/iosettings.h>
#include <core/rcchannel.h>
#include <core/analog.h>
#include "stm32f4xx_hal.h"
#include "periphery/adc.h"
#include "w25qxx.h"
#include "gui/stdispdriver.h"
#include "gui/stapp.h"
#include "core/auxiliary.h"
#include "core/rccurve.h"
#include "core/common.h"
#include "fatfs.h"

ModelSettingsTypeDef ModelSettings[MODEL_FLASH_NUM] = { 0 };
CommonSettingsTypedef CommonSettings;

void STmodelProfileInit(void) {
	uint16_t i = 0;
	uint16_t j = 0;

	for (i = 0; i < MODEL_FLASH_NUM; i++)
	{
		for (j = 0; j < MAX_RC_CHANNEL; j++)
		{

			ModelSettings[i].ID = i;
			ModelSettings[i].LowRate[j] = 100;
			ModelSettings[i].HighRate[j] = 100;
			ModelSettings[i].TrimmerValue[j] = 0;
			ModelSettings[i].Invert[j] = 0;
			ModelSettings[i].ChannelBinding[j] = j;
			ModelSettings[i].CurveType[j] = 0;
			ModelSettings[i].ExpCurveX[j] = 0;
			ModelSettings[i].ExpCurveY[j] = 0;
		}

		snprintf(ModelSettings[i].Name, MAX_RC_NAME, "Model %d", i);
		strncpy(ModelSettings[i].Timer1Name, "Timer1", MAX_RC_NAME);
		strncpy(ModelSettings[i].Timer2Name, "Timer2", MAX_RC_NAME);
		ModelSettings[i].AlarmEnableT1 = 0;
		ModelSettings[i].AlarmEnableT2 = 0;
		ModelSettings[i].SecondSetpointT1 = 0;
		ModelSettings[i].SecondSetpointT2 = 0;
		ModelSettings[i].MinuteSetpointT1 = 10;
		ModelSettings[i].MinuteSetpointT2 = 10;
		ModelSettings[i].DiscretBufferItemNumberT1 = 31;
		ModelSettings[i].DiscretBufferItemNumberT2 = 31;
	}

	for(i = 0;i < ADC1_CH_NUM;i++)
	{
		CommonSettings.ADCcentr[i] = ADC_CENTRAL;
		CommonSettings.ADCmin[i] = ADC_MIN;
		CommonSettings.ADCmax[i] = ADC_MAX;
	}

	CommonSettings.DispplayBrightness = BRIGHTNESS_MAX;
	CommonSettings.CurrentModelID = 1;
	CommonSettings.FistStartDO = 0xBEAF;
	CommonSettings.BatteryAlarmValue = BATTERY_ALARM_LOW;
	CommonSettings.InactivityDuration = INACTIVITY_TIME;
	CommonSettings.BeeperEnable = 1;
	CommonSettings.TrimBeeperEnable = 1;
	CommonSettings.ScreenInactivityTime = 5;
	CommonSettings.ScreenSaverTime = 7;
	CommonSettings.USBmode = __VCOM;
	CommonSettings.BatteryAdjustment = 0;

	STloadCommonSettings(&CommonSettings);

	STsaveSettingsToFlash();
}

/*
 * ������� �������� ������� � ������� �������
 */
void STsaveProfile(ModelSettingsTypeDef *ModelSettings)
{
	uint16_t i = 0;
	/*
	 * RC channel settings
	 */
	for (i = 0; i < MAX_RC_CHANNEL; i++)
	{
		ModelSettings->LowRate[i] = RCChanelGetLowRate(&RCChanel[i]);
		ModelSettings->HighRate[i] = RCChanelGetHighRate(&RCChanel[i]);
		ModelSettings->TrimmerValue[i] = RCChanelGetTrim(&RCChanel[i]);
		ModelSettings->Invert[i] = RCChanelGetInvertState(&RCChanel[i]);
		ModelSettings->ChannelBinding[i] = RCChanelBufferGetItem(&RCChanel[i]);
		ModelSettings->CurveType[i] = RCChanelGetCurveType(&RCChanel[i]);
		ModelSettings->ExpCurveX[i] = RCChanelGetExpoX(&RCChanel[i]);
		ModelSettings->ExpCurveY[i] = RCChanelGetExpoY(&RCChanel[i]);
	}
	/*
	 * Timer settings
	 */
	strncpy(ModelSettings->Timer1Name, RCStimerGetName(&RCTimer1), MAX_RC_NAME);
	strncpy(ModelSettings->Timer2Name, RCStimerGetName(&RCTimer2), MAX_RC_NAME);
	ModelSettings->AlarmEnableT1 = RCStimerGetAlarmState(&RCTimer1);
	ModelSettings->AlarmEnableT2 = RCStimerGetAlarmState(&RCTimer2);
	ModelSettings->SecondSetpointT1 = RCStimerGetSecondSetpoint(&RCTimer1);
	ModelSettings->SecondSetpointT2 = RCStimerGetSecondSetpoint(&RCTimer2);
	ModelSettings->MinuteSetpointT1 = RCStimerGetMinuteSetpoint(&RCTimer1);
	ModelSettings->MinuteSetpointT2 = RCStimerGetMinuteSetpoint(&RCTimer2);
	ModelSettings->DiscretBufferItemNumberT1 = RCStimerGetBuffItemNumber(&RCTimer1);
	ModelSettings->DiscretBufferItemNumberT2 = RCStimerGetBuffItemNumber(&RCTimer2);
}

/*
 * ������� �������� ������� �� �������� �������
 */
void STloadProfile(ModelSettingsTypeDef *ModelSettings)
{
	uint16_t i = 0;

	/*
	 * RC channel settings
	 */
	for (i = 0; i < MAX_RC_CHANNEL; i++) {
		RCChanelSetTrim(ModelSettings->TrimmerValue[i], &RCChanel[i]);
		RCChanelSetLowRate(ModelSettings->LowRate[i], &RCChanel[i]);
		RCChanelSetHightRate(ModelSettings->HighRate[i], &RCChanel[i]);
		RCChanelSetInvertState(ModelSettings->Invert[i], &RCChanel[i]);
		RCChanelBufferSetItem(ModelSettings->ChannelBinding[i], &RCChanel[i]);

		/*
		 * Curves
		 */
		RCChanelSetCurveType(ModelSettings->CurveType[i], &RCChanel[i]);
		RCChanelSetExpoX(ModelSettings->ExpCurveX[i], &RCChanel[i]);
		RCChanelSetExpoY(ModelSettings->ExpCurveY[i], &RCChanel[i]);
		STcurveFill(i);
	}

	/*
	 * Timer settings
	 */
	RCStimerSetName(ModelSettings->Timer1Name, &RCTimer1);
	RCStimerSetName(ModelSettings->Timer2Name, &RCTimer2);
	RCStimerSetAlarmState(ModelSettings->AlarmEnableT1, &RCTimer1);
	RCStimerSetAlarmState(ModelSettings->AlarmEnableT2, &RCTimer2);
	RCStimerSetSecondSetpoint(ModelSettings->SecondSetpointT1, &RCTimer1);
	RCStimerSetSecondSetpoint(ModelSettings->SecondSetpointT2, &RCTimer2);
	RCStimerSetMinuteSetpoint(ModelSettings->MinuteSetpointT1, &RCTimer1);
	RCStimerSetMinuteSetpoint(ModelSettings->MinuteSetpointT2, &RCTimer2);
	RCStimerSetBuffItemNumber(ModelSettings->DiscretBufferItemNumberT1, &RCTimer1);
	RCStimerSetBuffItemNumber(ModelSettings->DiscretBufferItemNumberT2, &RCTimer2);

	AIsetScaleMax((AIgetScaleMax( &AnalogChannel[BatteryADC]) + CommonSettings.BatteryAdjustment), &AnalogChannel[BatteryADC]);
}

void STcopySettings(ModelSettingsTypeDef *Target, ModelSettingsTypeDef *Source) {
	memcpy(Target, Source, sizeof(ModelSettingsTypeDef));
}

/*
 * ������ �������� ������� �� �����
 */
void STreadSettingsFromFlash(void)
{
#if MODEL_PROFILE_STORAGE == 0

	uint32_t FlashPointer = FLASH_SET_PAGE_ADDR;
	uint32_t *DataStructPointer;

	DataStructPointer = (uint32_t *) &ModelSettings[0];

	for (uint16_t i = 0; i < sizeof(ModelSettings) / 4; i++) {
		*DataStructPointer = *(__IO uint32_t*) FlashPointer;
		FlashPointer += 4;
		DataStructPointer++;
	}

#elif MODEL_PROFILE_STORAGE == 1

	uint8_t *SettingsPointer;

	/*
	 * Read system settings
	 */
	SettingsPointer = (void*)&CommonSettings;
	W25qxx_ReadBytes(SettingsPointer, SYSTEM_SETTINGS_FLASH_START_ADDRESS, sizeof(CommonSettings));

	/*
	 * Read model profile
	 */

	SettingsPointer = (void*)&ModelSettings;
	W25qxx_ReadBytes(SettingsPointer, MODEL_PROFILE_FLASH_START_ADDRESS, sizeof(ModelSettings));

	/*
	 * If first use
	 */
	if (CommonSettings.FistStartDO != 0xBEAF)
	{
		STmodelProfileInit();

		STloadCommonSettings(&CommonSettings);

		STloadProfile(&ModelSettings[CommonSettings.CurrentModelID]);

	} else
	{
		STloadCommonSettings(&CommonSettings);

		STloadProfile(&ModelSettings[CommonSettings.CurrentModelID]);
	}

#endif


}

/*
 * ������ �� ���� �������� �������
 */
void STsaveSettingsToFlash(void) {


	STsaveCommonSettings(&CommonSettings);

	STsaveProfile(&ModelSettings[CommonSettings.CurrentModelID]);

#if MODEL_PROFILE_STORAGE == 0
	static FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t PageError;
	uint16_t i;
	uint32_t *DataStructPointer;
	uint32_t FlashPointer;

	DataStructPointer = (uint32_t *) &ModelSettings[0];

	FlashPointer = FLASH_SET_PAGE_ADDR;

	  /* Fill EraseInit structure*/
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_2;
	EraseInitStruct.Sector = FLASH_SECTOR_6;
	EraseInitStruct.NbSectors = 1; //NbOfSectors

	HAL_FLASH_Unlock();

	HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);

	for (i = 0; i < sizeof(ModelSettings) / 4; i++)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, FlashPointer,	*DataStructPointer);

		DataStructPointer++;
		FlashPointer += 4;
	}

	HAL_FLASH_Lock();
#elif MODEL_PROFILE_STORAGE == 1

	uint8_t *SettingsPointer;

	SettingsPointer = (void*)&CommonSettings;

	W25qxx_EraseSector(SYSTEM_SETTINGS_FLASH_SECTOR);

	W25qxx_WriteSector(SettingsPointer, SYSTEM_SETTINGS_FLASH_SECTOR, 0, sizeof(CommonSettings));

	SettingsPointer = (void*)ModelSettings;

	W25qxx_EraseSector(MODEL_PROFILE_FLASH_SECTOR);

	W25qxx_WriteSector(SettingsPointer, MODEL_PROFILE_FLASH_SECTOR, 0, sizeof(ModelSettings));

#endif
}


void ProfileStorageInit(void)
{
#if MODEL_PROFILE_STORAGE == 0

#elif MODEL_PROFILE_STORAGE == 1

	W25qxx_Init();

#endif
}


void STloadCommonSettings(CommonSettingsTypedef *Settings)
{
	uint16_t i = 0;
	
	for(i = 0;i < ADC1_CH_NUM;i++)
	{
		AIsetADCMin(Settings->ADCmin[i], &AnalogChannel[i]);
		AIsetADCMax(Settings->ADCmax[i], &AnalogChannel[i]);
		AIsetADCCenter(Settings->ADCcentr[i], &AnalogChannel[i]);
	}

	STLCDsetBrightness(Settings->DispplayBrightness);
	STappSetBeeperState(Settings->BeeperEnable, &STApp);
	AIsetL(Settings->BatteryAlarmValue, &AnalogChannel[BatteryADC]);
}


void STsaveCommonSettings(CommonSettingsTypedef *Settings)
{
	uint16_t i = 0;

	for(i = 0;i < ADC1_CH_NUM;i++)
	{
		Settings->ADCmin[i] = AIgetADCMin(&AnalogChannel[i]);
		Settings->ADCmax[i] = AIgetADCMax(&AnalogChannel[i]);
		Settings->ADCcentr[i] = AIgetADCCenter(&AnalogChannel[i]);
	}

	Settings->DispplayBrightness = STLCDgetBrightness();
	Settings->BatteryAlarmValue = AIgetL(&AnalogChannel[BatteryADC]);
	Settings->BeeperEnable = STappGetBeeperState(&STApp);
}

uint16_t STgetCurrentModelID()
{
	return CommonSettings.CurrentModelID;
}

uint8_t STsaveSettingsToSDcard(void)
{



	return 0;
}

uint8_t STreadSettingsFromSDcard(void)
{
	return FR_OK;
}

uint32_t startBootloaderCheck(void)
{
    RTC_HandleTypeDef rtcHandle = { .Instance = RTC };

    uint32_t value = HAL_RTCEx_BKUPRead(&rtcHandle, OBJECT_RESET_REASON);

    HAL_RTCEx_BKUPWrite(&rtcHandle, OBJECT_RESET_REASON, BOOTLOADER_RESET);

    return value;
}

void rebootToBootloader(void)
{
    RTC_HandleTypeDef rtcHandle = { .Instance = RTC };

    HAL_RTCEx_BKUPWrite(&rtcHandle, OBJECT_RESET_REASON, BOOTLOADER_REQUEST);

    __disable_irq();

	NVIC_SystemReset();
}

/*
 * Request from startup script
 */
void JumpToBootloader(void)
{
	  if(startBootloaderCheck() != BOOTLOADER_REQUEST)
	  {
		  return;
	  }
		    void (*SysMemBootJump)(void);
		    volatile uint32_t addr = 0x1FFF0000;

		    HAL_RCC_DeInit();

		    SysTick->CTRL = 0;
		    SysTick->LOAD = 0;
		    SysTick->VAL = 0;

		    __enable_irq();

		    SysMemBootJump = (void (*)(void)) (*((uint32_t *)(addr + 4)));

		    __set_MSP(*(uint32_t *)addr);

		    SysMemBootJump();
}

void AllReset(void)
{
	W25qxx_EraseSector(SYSTEM_SETTINGS_FLASH_SECTOR);

	W25qxx_EraseSector(MODEL_PROFILE_FLASH_SECTOR);

	NVIC_SystemReset();
}
