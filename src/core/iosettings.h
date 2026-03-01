/*
 * iosettings.h
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

#ifndef ST_IOSETTINGS_H_
#define ST_IOSETTINGS_H_

#include <stdint.h>
#include "main.h"
#include "stconfig.h"

#define BOOTLOADER_REQUEST						0xDEADBEEF
#define BOOTLOADER_RESET						0x00000000
#define OBJECT_RESET_REASON						1

#define	SYSTEM_SETTINGS_FLASH_START_ADDRESS		0x000000	/* sector A (primary)  */
#define	SYSTEM_SETTINGS_FLASH_START_ADDRESS_B	0x001000	/* sector B (backup)   */
#define	MODEL_PROFILE_FLASH_START_ADDRESS		0x002000	/* sector 2            */

#define	SYSTEM_SETTINGS_FLASH_SECTOR_A			0x00		/* CommonSettings primary */
#define	SYSTEM_SETTINGS_FLASH_SECTOR_B			0x01		/* CommonSettings backup  */
#define	MODEL_PROFILE_FLASH_SECTOR				0x02
#define	MODEL_PROFILE_FLASH_SECTOR_2			0x03		/* Extra sector for large model data */

typedef struct{

	/*
	 * RC Channel
	 */
	char						Name[MAX_RC_NAME];				/* Name of current profile */
	uint16_t					ID;								/* Profile ID 1... MAX PROFILE_NUMBER*/
	int16_t						TrimmerValue[MAX_RC_CHANNEL];	/* Channel settings */
	uint16_t					LowRate[MAX_RC_CHANNEL];
	uint16_t					HighRate[MAX_RC_CHANNEL];
	uint16_t					Invert[MAX_RC_CHANNEL];
	uint16_t					ChannelBinding[MAX_RC_CHANNEL];
	uint16_t					CurveType[MAX_RC_CHANNEL];
	uint16_t					ExpCurveX[MAX_RC_CHANNEL];
	uint16_t					ExpCurveY[MAX_RC_CHANNEL];
	uint16_t					FailsafeValue[MAX_RC_CHANNEL];

	/*
	 * Timers
	 */
	char						Timer1Name[MAX_RC_NAME];		/* Name of timer1 */
	char						Timer2Name[MAX_RC_NAME];		/* Name of timer2 */
	uint16_t 					DiscretBufferItemNumberT1;
	uint16_t 					DiscretBufferItemNumberT2;
	uint16_t 					AlarmEnableT1;
	uint16_t 					AlarmEnableT2;
	uint16_t 					SecondSetpointT1;
	uint16_t 					SecondSetpointT2;
	uint16_t 					MinuteSetpointT1;
	uint16_t 					MinuteSetpointT2;
	/*
	 * Transmitters
	 */
	uint16_t 					TxMode;
	uint16_t 					TxProtocol;
	uint16_t 					TxSubProtocol;
	/*
	 *
	 */
}ModelSettingsTypeDef;

typedef struct{
	uint16_t					ADCmin[ADC1_CH_NUM];
	uint16_t					ADCmax[ADC1_CH_NUM];
	uint16_t					ADCcentr[ADC1_CH_NUM];
	uint16_t					CurrentModelID;
	uint16_t					VibroDuration;
	uint16_t					DisplayBrightness;
	uint16_t					FistStartDO;					/* First start flag. Need for first start wizard */
	uint16_t					BatteryAlarmValue;				/* Battery minimum*/
	uint16_t					InactivityDuration;
	uint16_t					BeeperEnable;
	uint16_t					TrimBeeperEnable;
	uint16_t					ScreenInactivityTime;			// minute
	uint16_t					ScreenSaverTime;				// minute

	uint16_t					USBmode;						/* 0 - VCOM
	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 * 1 - Joystick
	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 * 2 - SC card reader
	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 */
	int16_t					BatteryAdjustment;

	uint32_t				Checksum;				/* CRC-sum — MUST be the last field */

}CommonSettingsTypedef;

typedef enum {
	FSM_FLASH_IDLE = 0,
	FSM_FLASH_SNAPSHOT,           /* snapshot RAM → buffer         */
	FSM_FLASH_ERASE_COMMON_B,     /* start sector B erase           */
	FSM_FLASH_WAIT_ERASE_B,       /* wait for erase B to complete   */
	FSM_FLASH_WRITE_COMMON_B,     /* start writing page            */
	FSM_FLASH_WAIT_WRITE_B,       /* wait for write to complete     */
	FSM_FLASH_ERASE_COMMON_A,
	FSM_FLASH_WAIT_ERASE_A,
	FSM_FLASH_WRITE_COMMON_A,
	FSM_FLASH_WAIT_WRITE_A,
	FSM_FLASH_ERASE_MODELS,
	FSM_FLASH_WAIT_ERASE_MODELS,
	FSM_FLASH_WRITE_MODELS,       /* loop through pages             */
	FSM_FLASH_WAIT_WRITE_MODELS,
	FSM_FLASH_DONE,
} FlashFsmState;

extern ModelSettingsTypeDef ModelSettings[MODEL_FLASH_NUM];

extern CommonSettingsTypedef CommonSettings;

void STmodelProfileInit(void);


/*
 * SD card
 */

uint8_t STsaveSettingsToSDcard(void);

uint8_t STreadSettingsFromSDcard(void);

/*
 * Flash
 */
void STsaveSettingsToFlash(void);

void STreadSettingsFromFlash(void);

/* Async flash save API */
void STrequestSettingsSave(void);   /* request async save */
void STflashSaveTick(void);         /* call from main loop */
uint8_t STisFlashSaving(void);      /* 1 if save in progress */

/* W25Q16 async helper functions (non-blocking) */
uint8_t  STw25qxxIsBusy(void);
void     STw25qxxEraseSectorStart(uint32_t SectorAddr);
void     STw25qxxWritePageStart(uint8_t *pBuffer, uint32_t page_addr, uint32_t num_bytes);

/* W25Q16 DMA async write functions */
void     STw25qxxWritePageStartDMA(uint8_t *pBuffer, uint32_t page_addr, uint32_t num_bytes);
uint8_t  STw25qxxIsDMAComplete(void);
void     STw25qxxSetDMAComplete(void);


/*
 * Common
 */
void STloadCommonSettings(CommonSettingsTypedef *Settings);

void STsaveCommonSettings(CommonSettingsTypedef *Settings);

void STloadProfile(ModelSettingsTypeDef *ModelSettings);

void STsaveProfile(ModelSettingsTypeDef *ModelSettings);

void STcopyProfile(ModelSettingsTypeDef *Target, ModelSettingsTypeDef *Source);

uint16_t STgetCurrentModelID(void);

uint32_t startBootloaderCheck(void);

void rebootToBootloader(void);

void JumpToBootloader(void);

void ProfileStorageInit(void);

void AllReset(void);

#endif /* ST_IOSETTINGS_H_ */

