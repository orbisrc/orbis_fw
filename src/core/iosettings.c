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
#include "w25qxxConf.h"
#include "gui/stdispdriver.h"
#include "gui/stapp.h"
#include "core/auxiliary.h"
#include "core/rccurve.h"
#include "core/common.h"
#include "fatfs.h"
#include "multiprotocol.h"

ModelSettingsTypeDef ModelSettings[MODEL_FLASH_NUM] = {0};
CommonSettingsTypedef CommonSettings;

/* Verify that ModelSettings fits in 2 sectors (8KB = 2 × 4096) */
_Static_assert(sizeof(ModelSettings) <= 2 * 4096,
	"ModelSettings exceeds 2 W25Q16 sectors (8KB)");

/*
 * Compute additive checksum over all fields of CommonSettings except the
 * Checksum field itself (which is always the last field in the struct).
 */
static uint32_t STcomputeSettingsChecksum(const CommonSettingsTypedef *s)
{
	const uint8_t *data = (const uint8_t *)s;
	uint32_t size = sizeof(CommonSettingsTypedef) - sizeof(uint32_t);
	uint32_t sum = 0xA5A5A5A5UL;

	for (uint32_t i = 0; i < size; i++)
		sum += data[i];

	return sum;
}

static uint8_t STvalidateCommonSettings(const CommonSettingsTypedef *s)
{
	return s->Checksum == STcomputeSettingsChecksum(s);
}

/* --- Async Flash Save State Machine --- */
static FlashFsmState  flash_fsm        = FSM_FLASH_IDLE;
static uint8_t        save_requested   = 0;
static uint8_t        flash_page_idx   = 0;    /* current page index during write models */
static uint8_t        flash_page_total = 0;    /* total pages for models */

/* Snapshots: we don't write directly from live buffers during async save */
static CommonSettingsTypedef  snap_common;
static ModelSettingsTypeDef   snap_models[MODEL_FLASH_NUM];

void STrequestSettingsSave(void)
{
	save_requested = 1;
}

uint8_t STisFlashSaving(void)
{
	return flash_fsm != FSM_FLASH_IDLE;
}

void STflashSaveTick(void)
{
	switch (flash_fsm) {
	case FSM_FLASH_IDLE:
		if (!save_requested)
			return;
		save_requested = 0;
		flash_fsm = FSM_FLASH_SNAPSHOT;
		break;

	case FSM_FLASH_SNAPSHOT:
		STsaveCommonSettings(&CommonSettings);
		STsaveProfile(&ModelSettings[CommonSettings.CurrentModelID]);
		CommonSettings.Checksum = STcomputeSettingsChecksum(&CommonSettings);
		memcpy(&snap_common, &CommonSettings, sizeof(CommonSettings));
		memcpy(snap_models,  ModelSettings,   sizeof(ModelSettings));
		flash_fsm = FSM_FLASH_ERASE_COMMON_B;
		break;

	case FSM_FLASH_ERASE_COMMON_B:
		STw25qxxEraseSectorStart(SYSTEM_SETTINGS_FLASH_SECTOR_B);
		flash_fsm = FSM_FLASH_WAIT_ERASE_B;
		break;

	case FSM_FLASH_WAIT_ERASE_B:
		if (STw25qxxIsBusy())
			return;
		flash_page_idx = 0;
		flash_fsm = FSM_FLASH_WRITE_COMMON_B;
		break;

	case FSM_FLASH_WRITE_COMMON_B: {
		uint32_t first_page = W25qxx_SectorToPage(SYSTEM_SETTINGS_FLASH_SECTOR_B);
		uint32_t offset     = flash_page_idx * w25qxx.PageSize;
		uint32_t remaining  = sizeof(snap_common) > offset
							  ? sizeof(snap_common) - offset : 0;
		if (remaining == 0) {
			flash_fsm = FSM_FLASH_ERASE_COMMON_A;
			break;
		}
		uint32_t chunk = remaining < w25qxx.PageSize ? remaining : w25qxx.PageSize;
		STw25qxxWritePageStart((uint8_t*)&snap_common + offset,
							   first_page + flash_page_idx, chunk);
		flash_fsm = FSM_FLASH_WAIT_WRITE_B;
		break;
	}

	case FSM_FLASH_WAIT_WRITE_B:
		if (STw25qxxIsBusy())
			return;
		flash_page_idx++;
		flash_fsm = FSM_FLASH_WRITE_COMMON_B;  /* loop back and check remaining */
		break;

	case FSM_FLASH_ERASE_COMMON_A:
		STw25qxxEraseSectorStart(SYSTEM_SETTINGS_FLASH_SECTOR_A);
		flash_fsm = FSM_FLASH_WAIT_ERASE_A;
		break;

	case FSM_FLASH_WAIT_ERASE_A:
		if (STw25qxxIsBusy())
			return;
		flash_page_idx = 0;
		flash_fsm = FSM_FLASH_WRITE_COMMON_A;
		break;

	case FSM_FLASH_WRITE_COMMON_A: {
		uint32_t first_page = W25qxx_SectorToPage(SYSTEM_SETTINGS_FLASH_SECTOR_A);
		uint32_t offset     = flash_page_idx * w25qxx.PageSize;
		uint32_t remaining  = sizeof(snap_common) > offset
							  ? sizeof(snap_common) - offset : 0;
		if (remaining == 0) {
			flash_fsm = FSM_FLASH_ERASE_MODELS;
			break;
		}
		uint32_t chunk = remaining < w25qxx.PageSize ? remaining : w25qxx.PageSize;
		STw25qxxWritePageStart((uint8_t*)&snap_common + offset,
							   first_page + flash_page_idx, chunk);
		flash_fsm = FSM_FLASH_WAIT_WRITE_A;
		break;
	}

	case FSM_FLASH_WAIT_WRITE_A:
		if (STw25qxxIsBusy())
			return;
		flash_page_idx++;
		flash_fsm = FSM_FLASH_WRITE_COMMON_A;  /* loop back */
		break;

	case FSM_FLASH_ERASE_MODELS:
		STw25qxxEraseSectorStart(MODEL_PROFILE_FLASH_SECTOR);
		flash_fsm = FSM_FLASH_WAIT_ERASE_MODELS;
		break;

	case FSM_FLASH_WAIT_ERASE_MODELS:
		if (STw25qxxIsBusy())
			return;
		flash_page_idx   = 0;
		flash_page_total = (sizeof(snap_models) + w25qxx.PageSize - 1) / w25qxx.PageSize;
		flash_fsm = FSM_FLASH_WRITE_MODELS;
		break;

	case FSM_FLASH_WRITE_MODELS: {
		if (flash_page_idx >= flash_page_total) {
			flash_fsm = FSM_FLASH_DONE;
			break;
		}
		uint32_t first_page = W25qxx_SectorToPage(MODEL_PROFILE_FLASH_SECTOR);
		uint32_t offset     = flash_page_idx * w25qxx.PageSize;
		uint32_t remaining  = sizeof(snap_models) - offset;
		uint32_t chunk      = remaining < w25qxx.PageSize ? remaining : w25qxx.PageSize;
		STw25qxxWritePageStart((uint8_t*)snap_models + offset,
							   first_page + flash_page_idx, chunk);
		flash_fsm = FSM_FLASH_WAIT_WRITE_MODELS;
		break;
	}

	case FSM_FLASH_WAIT_WRITE_MODELS:
		if (STw25qxxIsBusy())
			return;
		flash_page_idx++;
		flash_fsm = FSM_FLASH_WRITE_MODELS;  /* loop back */
		break;

	case FSM_FLASH_DONE:
		flash_fsm = FSM_FLASH_IDLE;
		break;
	}
}

/* --- W25Q16 Async Helper Functions --- */
#define W25QXX_DUMMY_BYTE 0xA5

uint8_t STw25qxxIsBusy(void)
{
	uint8_t status;
	HAL_GPIO_WritePin(_W25QXX_CS_GPIO, _W25QXX_CS_PIN, GPIO_PIN_RESET);
	W25qxx_Spi(0x05);  /* READ STATUS REGISTER-1 */
	status = W25qxx_Spi(W25QXX_DUMMY_BYTE);
	HAL_GPIO_WritePin(_W25QXX_CS_GPIO, _W25QXX_CS_PIN, GPIO_PIN_SET);
	return status & 0x01;  /* BUSY bit (bit 0) */
}

void STw25qxxEraseSectorStart(uint32_t SectorAddr)
{
	uint32_t addr = SectorAddr * w25qxx.SectorSize;

	/* WriteEnable without delay */
	HAL_GPIO_WritePin(_W25QXX_CS_GPIO, _W25QXX_CS_PIN, GPIO_PIN_RESET);
	W25qxx_Spi(0x06);  /* WREN */
	HAL_GPIO_WritePin(_W25QXX_CS_GPIO, _W25QXX_CS_PIN, GPIO_PIN_SET);

	/* Sector Erase 0x20 */
	HAL_GPIO_WritePin(_W25QXX_CS_GPIO, _W25QXX_CS_PIN, GPIO_PIN_RESET);
	W25qxx_Spi(0x20);
	W25qxx_Spi((addr & 0xFF0000) >> 16);
	W25qxx_Spi((addr & 0xFF00) >> 8);
	W25qxx_Spi(addr & 0xFF);
	HAL_GPIO_WritePin(_W25QXX_CS_GPIO, _W25QXX_CS_PIN, GPIO_PIN_SET);
	/* Flash starts erase internally - no waiting here */
}

void STw25qxxWritePageStart(uint8_t *pBuffer, uint32_t page_addr, uint32_t num_bytes)
{
	if (num_bytes == 0 || num_bytes > w25qxx.PageSize)
		num_bytes = w25qxx.PageSize;

	uint32_t addr = page_addr * w25qxx.PageSize;

	/* WriteEnable without delay */
	HAL_GPIO_WritePin(_W25QXX_CS_GPIO, _W25QXX_CS_PIN, GPIO_PIN_RESET);
	W25qxx_Spi(0x06);  /* WREN */
	HAL_GPIO_WritePin(_W25QXX_CS_GPIO, _W25QXX_CS_PIN, GPIO_PIN_SET);

	/* Page Program 0x02 */
	HAL_GPIO_WritePin(_W25QXX_CS_GPIO, _W25QXX_CS_PIN, GPIO_PIN_RESET);
	W25qxx_Spi(0x02);
	W25qxx_Spi((addr & 0xFF0000) >> 16);
	W25qxx_Spi((addr & 0xFF00) >> 8);
	W25qxx_Spi(addr & 0xFF);
	HAL_SPI_Transmit(&_W25QXX_SPI, pBuffer, num_bytes, 100);  /* ~50us @ 42MHz */
	HAL_GPIO_WritePin(_W25QXX_CS_GPIO, _W25QXX_CS_PIN, GPIO_PIN_SET);
	/* Flash starts programming internally - no waiting here */
}

void STmodelProfileInit(void)
{
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
			ModelSettings[i].FailsafeValue[j] = BASE_CENTRAL;
		}

		snprintf(ModelSettings[i].Name, MAX_RC_NAME, "Model %d", i);
		strlcpy(ModelSettings[i].Timer1Name, "Timer1", MAX_RC_NAME);
		strlcpy(ModelSettings[i].Timer2Name, "Timer2", MAX_RC_NAME);
		ModelSettings[i].AlarmEnableT1 = 0;
		ModelSettings[i].AlarmEnableT2 = 0;
		ModelSettings[i].SecondSetpointT1 = 0;
		ModelSettings[i].SecondSetpointT2 = 0;
		ModelSettings[i].MinuteSetpointT1 = 10;
		ModelSettings[i].MinuteSetpointT2 = 10;
		ModelSettings[i].DiscretBufferItemNumberT1 = 31;
		ModelSettings[i].DiscretBufferItemNumberT2 = 31;
		ModelSettings[i].TxMode = 1;
		ModelSettings[i].TxSubProtocol = 0;
		ModelSettings[i].TxProtocol = 0;
	}

	for (i = 0; i < ADC1_CH_NUM; i++)
	{
		CommonSettings.ADCcentr[i] = ADC_CENTRAL;
		CommonSettings.ADCmin[i] = ADC_MIN;
		CommonSettings.ADCmax[i] = ADC_MAX;
	}

	CommonSettings.DisplayBrightness = BRIGHTNESS_MAX;
	CommonSettings.CurrentModelID = 1;
	CommonSettings.VibroDuration = 0;
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
		ModelSettings->FailsafeValue[i] = RCChanelGetFailsafeValue(&RCChanel[i]);
	}
	/*
	 * Timer settings
	 */
	strlcpy(ModelSettings->Timer1Name, RCStimerGetName(&RCTimer1), MAX_RC_NAME);
	strlcpy(ModelSettings->Timer2Name, RCStimerGetName(&RCTimer2), MAX_RC_NAME);
	ModelSettings->AlarmEnableT1 = RCStimerGetAlarmState(&RCTimer1);
	ModelSettings->AlarmEnableT2 = RCStimerGetAlarmState(&RCTimer2);
	ModelSettings->SecondSetpointT1 = RCStimerGetSecondSetpoint(&RCTimer1);
	ModelSettings->SecondSetpointT2 = RCStimerGetSecondSetpoint(&RCTimer2);
	ModelSettings->MinuteSetpointT1 = RCStimerGetMinuteSetpoint(&RCTimer1);
	ModelSettings->MinuteSetpointT2 = RCStimerGetMinuteSetpoint(&RCTimer2);
	ModelSettings->DiscretBufferItemNumberT1 = RCStimerGetBuffItemNumber(&RCTimer1);
	ModelSettings->DiscretBufferItemNumberT2 = RCStimerGetBuffItemNumber(&RCTimer2);
	/*
	 * Transmitter
	 */
	ModelSettings->TxMode = 1;
	ModelSettings->TxSubProtocol = multiprotocolGetSubProtocol(&sbus);
	ModelSettings->TxProtocol = multiprotocolGetProtocol(&sbus);
}

void STloadProfile(ModelSettingsTypeDef *ModelSettings)
{
	uint16_t i = 0;

	/*
	 * RC channel settings
	 */
	for (i = 0; i < MAX_RC_CHANNEL; i++)
	{
		RCChanelSetTrim(ModelSettings->TrimmerValue[i], &RCChanel[i]);
		RCChanelSetLowRate(ModelSettings->LowRate[i], &RCChanel[i]);
		RCChanelSetHightRate(ModelSettings->HighRate[i], &RCChanel[i]);
		RCChanelSetInvertState(ModelSettings->Invert[i], &RCChanel[i]);
		RCChanelBufferSetItem(ModelSettings->ChannelBinding[i], &RCChanel[i]);
		RCChanelSetFailsafeValue(ModelSettings->FailsafeValue[i], &RCChanel[i]);

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

	AIsetScaleMax((AIgetScaleMax(&AnalogChannel[BatteryADC]) + CommonSettings.BatteryAdjustment), &AnalogChannel[BatteryADC]);
	/*
	 * Transmitter
	 */
	multiprotocolSetSubProtocol(ModelSettings->TxSubProtocol, &sbus);
	multiprotocolSetProtocol(ModelSettings->TxProtocol, &sbus);
}

void STcopySettings(ModelSettingsTypeDef *Target, ModelSettingsTypeDef *Source)
{
	memcpy(Target, Source, sizeof(ModelSettingsTypeDef));
}

void STreadSettingsFromFlash(void)
{
#if MODEL_PROFILE_STORAGE == 0

	uint32_t FlashPointer = FLASH_SET_PAGE_ADDR;
	uint32_t *DataStructPointer;

	DataStructPointer = (uint32_t *)&ModelSettings[0];

	for (uint16_t i = 0; i < sizeof(ModelSettings) / 4; i++)
	{
		*DataStructPointer = *(__IO uint32_t *)FlashPointer;
		FlashPointer += 4;
		DataStructPointer++;
	}

#elif MODEL_PROFILE_STORAGE == 1

	uint8_t valid = 0;

	/*
	 * Try primary CommonSettings (sector A).
	 */
	W25qxx_ReadBytes((uint8_t *)&CommonSettings, SYSTEM_SETTINGS_FLASH_START_ADDRESS, sizeof(CommonSettings));

	if (STvalidateCommonSettings(&CommonSettings))
	{
		valid = 1;
	}
	else
	{
		/*
		 * Primary corrupted (e.g. power loss during write).
		 * Try backup CommonSettings (sector B).
		 */
		W25qxx_ReadBytes((uint8_t *)&CommonSettings, SYSTEM_SETTINGS_FLASH_START_ADDRESS_B, sizeof(CommonSettings));

		if (STvalidateCommonSettings(&CommonSettings))
		{
			valid = 1;

			/* Restore primary from backup so next boot is normal. */
			W25qxx_EraseSector(SYSTEM_SETTINGS_FLASH_SECTOR_A);
			W25qxx_WriteSector((uint8_t *)&CommonSettings, SYSTEM_SETTINGS_FLASH_SECTOR_A, 0, sizeof(CommonSettings));
		}
	}

	if (!valid)
	{
		/*
		 * Both sectors corrupted or first start — initialise defaults.
		 */
		STmodelProfileInit();

		STloadCommonSettings(&CommonSettings);

		STloadProfile(&ModelSettings[CommonSettings.CurrentModelID]);

		return;
	}

	/*
	 * Read model profiles.
	 */
	W25qxx_ReadBytes((uint8_t *)&ModelSettings, MODEL_PROFILE_FLASH_START_ADDRESS, sizeof(ModelSettings));

	STloadCommonSettings(&CommonSettings);

	STloadProfile(&ModelSettings[CommonSettings.CurrentModelID]);

#endif
}

void STsaveSettingsToFlash(void)
{

	STsaveCommonSettings(&CommonSettings);

	STsaveProfile(&ModelSettings[CommonSettings.CurrentModelID]);

#if MODEL_PROFILE_STORAGE == 0
	static FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t PageError;
	uint16_t i;
	uint32_t *DataStructPointer;
	uint32_t FlashPointer;

	DataStructPointer = (uint32_t *)&ModelSettings[0];

	FlashPointer = FLASH_SET_PAGE_ADDR;

	/* Fill EraseInit structure*/
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_2;
	EraseInitStruct.Sector = FLASH_SECTOR_6;
	EraseInitStruct.NbSectors = 1; // NbOfSectors

	HAL_FLASH_Unlock();

	HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);

	for (i = 0; i < sizeof(ModelSettings) / 4; i++)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, FlashPointer, *DataStructPointer);

		DataStructPointer++;
		FlashPointer += 4;
	}

	HAL_FLASH_Lock();
#elif MODEL_PROFILE_STORAGE == 1

	/*
	 * Compute checksum before writing.
	 */
	CommonSettings.Checksum = STcomputeSettingsChecksum(&CommonSettings);

	/*
	 * Atomic write for CommonSettings:
	 * 1. Write to backup sector B first.
	 * 2. Then erase + write primary sector A.
	 * If power is lost between steps 1 and 2, sector B is still valid
	 * and settings will be recovered on next boot.
	 */
	W25qxx_EraseSector(SYSTEM_SETTINGS_FLASH_SECTOR_B);
	W25qxx_WriteSector((uint8_t *)&CommonSettings, SYSTEM_SETTINGS_FLASH_SECTOR_B, 0, sizeof(CommonSettings));

	W25qxx_EraseSector(SYSTEM_SETTINGS_FLASH_SECTOR_A);
	W25qxx_WriteSector((uint8_t *)&CommonSettings, SYSTEM_SETTINGS_FLASH_SECTOR_A, 0, sizeof(CommonSettings));

	/*
	 * Save model profiles.
	 */
	W25qxx_EraseSector(MODEL_PROFILE_FLASH_SECTOR);
	W25qxx_WriteSector((uint8_t *)ModelSettings, MODEL_PROFILE_FLASH_SECTOR, 0, sizeof(ModelSettings));

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

	for (i = 0; i < ADC1_CH_NUM; i++)
	{
		AIsetADCMin(Settings->ADCmin[i], &AnalogChannel[i]);
		AIsetADCMax(Settings->ADCmax[i], &AnalogChannel[i]);
		AIsetADCCenter(Settings->ADCcentr[i], &AnalogChannel[i]);
	}

	STLCDsetBrightness(Settings->DisplayBrightness);
	STappSetBeeperState(Settings->BeeperEnable, &STApp);
	AIsetL(Settings->BatteryAlarmValue, &AnalogChannel[BatteryADC]);
}

void STsaveCommonSettings(CommonSettingsTypedef *Settings)
{
	uint16_t i = 0;

	for (i = 0; i < ADC1_CH_NUM; i++)
	{
		Settings->ADCmin[i] = AIgetADCMin(&AnalogChannel[i]);
		Settings->ADCmax[i] = AIgetADCMax(&AnalogChannel[i]);
		Settings->ADCcentr[i] = AIgetADCCenter(&AnalogChannel[i]);
	}

	Settings->DisplayBrightness = STLCDgetBrightness();
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
	RTC_HandleTypeDef rtcHandle = {.Instance = RTC};

	uint32_t value = HAL_RTCEx_BKUPRead(&rtcHandle, OBJECT_RESET_REASON);

	HAL_RTCEx_BKUPWrite(&rtcHandle, OBJECT_RESET_REASON, BOOTLOADER_RESET);

	return value;
}

void rebootToBootloader(void)
{
	RTC_HandleTypeDef rtcHandle = {.Instance = RTC};

	HAL_RTCEx_BKUPWrite(&rtcHandle, OBJECT_RESET_REASON, BOOTLOADER_REQUEST);

	__disable_irq();

	NVIC_SystemReset();
}

/*
 * Request from startup script
 */
void JumpToBootloader(void)
{
	if (startBootloaderCheck() != BOOTLOADER_REQUEST)
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

	SysMemBootJump = (void (*)(void))(*((uint32_t *)(addr + 4)));

	__set_MSP(*(uint32_t *)addr);

	SysMemBootJump();
}

void AllReset(void)
{
	W25qxx_EraseSector(SYSTEM_SETTINGS_FLASH_SECTOR_A);
	W25qxx_EraseSector(SYSTEM_SETTINGS_FLASH_SECTOR_B);
	W25qxx_EraseSector(MODEL_PROFILE_FLASH_SECTOR);

	NVIC_SystemReset();
}
