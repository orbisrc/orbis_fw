/*
 * stio.c
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
#include <stfsio.h>
#include "fatfs.h"
#include "core/common.h"
#include "stconfig.h"


const char * STsdCardStateName[] = {

		  "SD not yet initialized or disabled  "
		  ,"SD initialized and ready for use    "
		  ,"SD Timeout state                    "
		  ,"SD process ongoing                  "
		  ,"SD Programming State                "
		  ,"SD Receiving State                  "
		  ,"SD Transfert State                  "
		  ,"SD is in error state                "

};



const char * STdirName[] = {
							"model"
							, "sound"
							, "logs"
							, "telemetry"
};

uint8_t STcardIOinit(void)
{

	return 0;

}

uint8_t STmkfs(void)
{
//	char dMessage[DEBUG_MESSAGE_MAX] = {0};
	uint8_t work[_MAX_SS] = {0};

	/*
	 *
	 */

#if DEBUG_UART_MESSAGE == 1

		common_printf("FS card formating......");

#endif
	__disable_irq();

 	uint8_t retSD = f_mkfs("", FM_FAT32, 0, work, sizeof work);

 	__enable_irq();


#if DEBUG_UART_MESSAGE == 1

	snprintf(dMessage, 255, "FS card format - %s \r\n", FScodeString[retSD]);
	common_printf(dMessage);

#endif

	return retSD;


}

/*
 * Open/Create directory
 */
uint8_t STopenDir(char * Dir)
{
	uint8_t retSD = 0;

	DIR dp;

	retSD = f_opendir(&dp, Dir);

#if DEBUG_UART_MESSAGE == 1

	snprintf(dMessage, 255, "FS open %s - %s \r\n", Dir ,FScodeString[retSD]);
	common_printf(dMessage);

#endif

	if (retSD == FR_NO_PATH)
	{

		retSD = f_mkdir(Dir);

#if DEBUG_UART_MESSAGE == 1

		snprintf(dMessage, 255, "FS create %s - %s \r\n", Dir, FScodeString[retSD]);
		common_printf(dMessage);

#endif

		if (retSD == FR_OK)
		{
			retSD = f_opendir(&dp, Dir);

#if DEBUG_UART_MESSAGE == 1

			snprintf(dMessage, 255, "FS open %s - %s \r\n", Dir, FScodeString[retSD]);
			common_printf(dMessage);

#endif
		}

	}else
	{

#if DEBUG_UART_MESSAGE == 1

		snprintf(dMessage, 255, "FS create %s - %s \r\n", Dir, FScodeString[retSD]);
		common_printf(dMessage);

#endif

		return retSD;
	}

	return retSD;
}

uint8_t STopenFile(char * Name, FIL *file)
{
	uint8_t retSD = 0;

//	f_open(file, path, FA_CREATE_NEW | FA_WRITE);

	return retSD;
}

uint8_t STwriteToEnd(char * Str, FIL *file)
{
	uint8_t retSD = 0;

	return retSD;
}
