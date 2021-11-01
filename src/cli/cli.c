/*
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

#include "cli.h"
#include "main.h"
#include "usbd_cdc_if.h"
#include "iosettings.h"
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "gui/stapp.h"

#define ARRAYLEN(x) (sizeof(x) / sizeof((x)[0]))
#define MAX_COMMAND_LENGHT 32

uint8_t cliRxBuffer[APP_RX_DATA_SIZE] = {0};
uint8_t cliTxBuffer[APP_TX_DATA_SIZE] = {0};

#define CLI_COMMAND_DEF(name, description, args, cliCommand) \
    {                                                        \
        name,                                                \
            description,                                     \
            args,                                            \
            cliCommand                                       \
    }

const clicmd_t cliCmdTable[] = {
    CLI_COMMAND_DEF("dfu", "switch to DFU mode", "[name]", cliSetDFU),
    CLI_COMMAND_DEF("get_status", "get variable value", "[name]", cliGetStatus),
    CLI_COMMAND_DEF("get_channels", "get channels value", "[name]", cliGetChannels),
    CLI_COMMAND_DEF("set_screen", "Set active screen", "[name] <screen>", cliSetScreen),
    CLI_COMMAND_DEF("status", "get current status", "[name]", cliGetStatus)};

void cliGet(const char *cmdName, const char *cmdline)
{
    cliPrint("Response \r\n");

    return;
}

void cliGetStatus(const char *cmdName, const char *cmdline)
{
    cliPrint("Ready \r\n");

    return;
}

void cliGetChannels(const char *cmdName, const char *cmdline)
{
    cliPrint("Channels value is here \r\n");

    return;
}

void cliSetScreen(const char *name, const char *cmdline)
{
    char *format = "%d";
    uint16_t screenNumber = 0;

    sscanf(cmdline, format, &screenNumber);

    if (screenNumber != 0)
    {
        STappSetScreen(screenNumber, &STApp);
        return;
    }

    cliPrint("Enter screen number");
}

void cliGetScreen(const char *name, const char *cmdline)
{
}

uint16_t isEmpty(const char *string)
{
    return (string == NULL || *string == '\0') ? HAL_OK : HAL_ERROR;
}

void cliHandler(const char *cmdString)
{
    char command[MAX_COMMAND_LENGHT] = {0};

    sscanf(cmdString, "%s", command);

    const char *ptr = strchr(cmdString, ' ');

    for (uint8_t i = 0; i < sizeof(cliCmdTable) / sizeof(clicmd_t); i++)
    {
        if (!strcmp(command, cliCmdTable[i].name))
        {
            cliCmdTable[i].cliCommand(cmdString, ptr);

            return;
        }
    }

    cliPrint("Command not found \r\n");
}

void cliPrint(const char *printString)
{
    if (!isEmpty(printString))
    {
        return;
    }

    strncpy((char *)cliRxBuffer, printString, APP_RX_DATA_SIZE);

    while (CDC_Transmit_FS(cliRxBuffer, strlen((char *)cliRxBuffer)) == USBD_BUSY)
    {
    }
}

void cliSetDFU(const char *name, const char *cmdline)
{
    rebootToBootloader();
}
