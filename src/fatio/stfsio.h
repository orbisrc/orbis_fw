/*
 * stio.h
 *
 *  Created on: 12 сент. 2020 г.
 *      Author: ivnss
 */

#ifndef INC_STFATIO_STFSIO_H_
#define INC_STFATIO_STFSIO_H_

#include "fatfs.h"

extern const char * STdirName[];

extern const char * STsdCardStateName[];

enum {
	model
	, sound
	, logs
	, telemetry
};

uint8_t STcardIOinit(void);

uint8_t STmkfs(void);

uint8_t STopenDir(char * Dir);

uint8_t STopenFile(char * Name, FIL *file);

uint8_t STwriteToEnd(char * Str, FIL *file);

uint8_t STwriteToEnd(char * Name, FIL *file);

uint8_t STmkfs(void);

#endif /* INC_STFATIO_STFSIO_H_ */
