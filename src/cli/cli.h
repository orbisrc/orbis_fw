/*
 * cli.h
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
 *limitations under the License.
 */

#ifndef INC_CLI_CLI_H_
#define INC_CLI_CLI_H_

#include "main.h"
#include "stm32f4xx_hal.h"

typedef void cliCommand(const char* name, const char *cmdline);

typedef struct {
    const char *name;
    const char *description;
    const char *args;
    cliCommand *cliCommand;
} clicmd_t;

void cliHandler(const char *cmdString);

void cliPrint(const char *printString);

void cliGet(const char *cmdName, const char *cmdline);

void cliGetChannels(const char *cmdName, const char *cmdline);

void cliGetStatus(const char *cmdName, const char *cmdline);

void cliSetDFU(const char* name, const char *cmdline);

void cliSetScreen(const char* name, const char *cmdline);

void cliGetScreen(const char* name, const char *cmdline);

uint16_t isEmpty(const char *string);


#endif /* INC_CLI_CLI_H_ */
