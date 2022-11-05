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
#include "radio.h"
#include "core/rcchannel.h"
#include "stconfig.h"

uint16_t getCurrentRadioChannelNumber()
{
    uint16_t channelNumber = 0;

    const uint16_t protocolItemNumber = getProtocolItemNumberByCode(multiprotocolGetProtocol(&sbus));
    const MULTITX_PROTO_ItemTypedef *protocol = getProtocolByItemNumber(protocolItemNumber);

    if (protocol->subProtocols != NULL)
    {
        const uint16_t selectedSubProtocolItem = getSubProtocolItemNumberByCode(protocol, multiprotocolGetSubProtocol(&sbus));
        channelNumber = protocol->subProtocols[selectedSubProtocolItem].chNumber;
    }
    return channelNumber;
}