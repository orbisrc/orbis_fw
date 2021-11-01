/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */



#include "fatfs.h"
#include <stdio.h>
#include <string.h>
#include "core/common.h"
#include "core/iosettings.h"
#include "stconfig.h"
#include "rtc.h"

uint8_t retSD;    		/* Return value for SD */
uint8_t	FSmout;
char SDPath[4] = {0};   /* SD logical drive path */
FATFS SDFatFS;    		/* File system object for SD logical drive */
FIL SDFile;      		/* File object for SD */

/* USER CODE BEGIN Variables */

const char * FScodeString [] = {
		"Succeeded"
		,"A hard error occurred in the low level disk I/O layer"
		,"Assertion failed"
		,"The physical drive cannot work"
		,"Could not find the file"
		,"Could not find the path"
		,"The path name format is invalid"
		,"Access denied due to prohibited access or directory full"
		,"Access denied due to prohibited access"
		,"The file/directory object is invalid"
		,"The physical drive is write protected"
		,"The logical drive number is invalid"
		,"The volume has no work area"
		,"There is no valid FAT volume"
		,"The f_mkfs() aborted due to any problem"
		,"Could not get a grant to access the volume within defined period"
		,"The operation is rejected according to the file sharing policy"
		,"LFN working buffer could not be allocated"
		,"Number of open files > _FS_LOCK"
		,"Given parameter is invalid"
};

void MX_FATFS_Init(void)
{

	/*
	 * Mount FS if not use mass storage usb mode
	 */

	if (CommonSettings.USBmode != __SDcardReader)
	{

	  /*## FatFS: Link the SD driver ###########################*/
		retSD = FATFS_LinkDriver(&SD_Driver, SDPath);

#if DEBUG_UART_MESSAGE == 1

		char dMessage[DEBUG_MESSAGE_MAX] = {0};

		snprintf(dMessage, DEBUG_MESSAGE_MAX, "FS link driver - %s \r\n", FScodeString[retSD]);
		common_printf(dMessage);

#endif

		if(retSD == FR_OK)
		{
		retSD = f_mount(&SDFatFS, SDPath, 1);

#if DEBUG_UART_MESSAGE == 1

			snprintf(dMessage, DEBUG_MESSAGE_MAX, "FS mount - %s \r\n", FScodeString[retSD]);
			common_printf(dMessage);
#endif
		}


	} 	else
	{
#if DEBUG_UART_MESSAGE == 1

		common_printf("Don't mount FS. Mass storage mode enabled. \r\n");

#endif
	}




//

}

DWORD get_fattime(void)
{

//	bit31:25 	Year
//	bit24:21 	Month, 1..12
//	bit20:16	Day in month 1..31
//	bit15:11 	Hour, 0..23
//	bit10:5 	Minute, 0..59
//	bit4:0 		Second 0..29

	RTC_TimeTypeDef CurrentTime = { 0 };
	RTC_DateTypeDef CurrentDate = { 0 };

	HAL_RTC_GetTime(&hrtc, &CurrentTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate (&hrtc, &CurrentDate, RTC_FORMAT_BIN);

	return ((DWORD)((DWORD)CurrentDate.Year - 1980) << 25 | (DWORD)CurrentDate.Month << 21 | (DWORD)CurrentDate.Date << 16
		  | (DWORD)CurrentTime.Hours << 11 | (DWORD)CurrentTime.Minutes << 5 | (DWORD)CurrentTime.Seconds/2);
}

uint8_t FS_MountCheck(void)
{
	return FSmout;
}

/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
