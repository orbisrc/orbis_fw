/*
 *
 *	Copyright 2022 Denis Davydov
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

#include "multiprotocol.h"
#include "core/rcchannel.h"
#include "stconfig.h"

SBUS_HandlerTypedef sbus = {0};

void multiprotocolInit()
{
    sbus.protocol = PROTO_FLYSKY;
    sbus.subProtocol = Flysky;
}

void multiprotocolAssignmentValues()
{
    static uint16_t ruler = 0;
    uint16_t i = 0;

    for (i = 0; i < MAX_RC_CHANNEL - 1; i++)
    {
        multiprotocolSetChannel(&sbus, i, (RCChanelGetValue(&RCChanel[i]) << 1)); // 
    }

    if (ruler >= 0x7FF)
    {
        ruler = 0;
    }
}

void multiprotocolSetChannel(SBUS_HandlerTypedef *sbus, uint16_t channelNumber, uint16_t value)
{
    sbus->outputBuffer[channelNumber] = value;
}

void makeOutputStream(SBUS_HandlerTypedef *sbus)
{
    sbus->outStream[0] = MULTIPROTOCOL_HEADER;
    sbus->outStream[1] = (uint8_t)((sbus->protocol & 0x001F) | (sbus->rangeCheckBit << 5) | (sbus->autoBindBit << 6) | (sbus->bind << 6));
    sbus->outStream[2] = (uint8_t)((sbus->rxNum & 0x000F) | (sbus->subProtocol << 4) | (sbus->lowPower << 7));
    sbus->outStream[3] = 0;
    sbus->outStream[4] = (uint8_t)((sbus->outputBuffer[0] & 0x07FF));
    sbus->outStream[5] = (uint8_t)((sbus->outputBuffer[0] & 0x07FF) >> 8 | (sbus->outputBuffer[1] & 0x07FF) << 3);
    sbus->outStream[6] = (uint8_t)((sbus->outputBuffer[1] & 0x07FF) >> 5 | (sbus->outputBuffer[2] & 0x07FF) << 6);
    sbus->outStream[7] = (uint8_t)((sbus->outputBuffer[2] & 0x07FF) >> 2);
    sbus->outStream[8] = (uint8_t)((sbus->outputBuffer[2] & 0x07FF) >> 10 | (sbus->outputBuffer[3] & 0x07FF) << 1);
    sbus->outStream[9] = (uint8_t)((sbus->outputBuffer[3] & 0x07FF) >> 7 | (sbus->outputBuffer[4] & 0x07FF) << 4);
    sbus->outStream[10] = (uint8_t)((sbus->outputBuffer[4] & 0x07FF) >> 4 | (sbus->outputBuffer[5] & 0x07FF) << 7);
    sbus->outStream[11] = (uint8_t)((sbus->outputBuffer[5] & 0x07FF) >> 1);
    sbus->outStream[12] = (uint8_t)((sbus->outputBuffer[5] & 0x07FF) >> 9 | (sbus->outputBuffer[6] & 0x07FF) << 2);
    sbus->outStream[13] = (uint8_t)((sbus->outputBuffer[6] & 0x07FF) >> 6 | (sbus->outputBuffer[7] & 0x07FF) << 5);
    sbus->outStream[14] = (uint8_t)((sbus->outputBuffer[7] & 0x07FF) >> 3);
    sbus->outStream[15] = (uint8_t)((sbus->outputBuffer[8] & 0x07FF));
    sbus->outStream[16] = (uint8_t)((sbus->outputBuffer[8] & 0x07FF) >> 8 | (sbus->outputBuffer[9] & 0x07FF) << 3);
    sbus->outStream[17] = (uint8_t)((sbus->outputBuffer[9] & 0x07FF) >> 5 | (sbus->outputBuffer[10] & 0x07FF) << 6);
    sbus->outStream[18] = (uint8_t)((sbus->outputBuffer[10] & 0x07FF) >> 2);
    sbus->outStream[19] = (uint8_t)((sbus->outputBuffer[10] & 0x07FF) >> 10 | (sbus->outputBuffer[11] & 0x07FF) << 1);
    sbus->outStream[20] = (uint8_t)((sbus->outputBuffer[11] & 0x07FF) >> 7 | (sbus->outputBuffer[12] & 0x07FF) << 4);
    sbus->outStream[21] = (uint8_t)((sbus->outputBuffer[12] & 0x07FF) >> 4 | (sbus->outputBuffer[13] & 0x07FF) << 7);
    sbus->outStream[22] = (uint8_t)((sbus->outputBuffer[13] & 0x07FF) >> 1);
    sbus->outStream[23] = (uint8_t)((sbus->outputBuffer[13] & 0x07FF) >> 9 | (sbus->outputBuffer[14] & 0x07FF) << 2);
    sbus->outStream[24] = (uint8_t)((sbus->outputBuffer[14] & 0x07FF) >> 6 | (sbus->outputBuffer[15] & 0x07FF) << 5);
    sbus->outStream[25] = (uint8_t)((sbus->outputBuffer[15] & 0x07FF) >> 3);
    sbus->outStream[26] = (uint8_t)0x08;
}