/*
 * Copyright (c) 2021, German Aerospace Center (DLR)
 *
 * This file is part of the development version of the Simple Interface
 * Protocol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *  Authors:
 *  - 2021, Norbert Toth (DLR RY-AVS)
 */

#ifndef OUTPOST_SIP_OUTPOST_SIP_H_
#define OUTPOST_SIP_OUTPOST_SIP_H_

#include <stdint.h>

namespace outpost
{
namespace sip
{
// unitId + counter + type + crc
// without length -> 2 bytes
const static uint8_t packetOverhead = 5;

// length of packet is not in packetLength
const static uint8_t packetLengthSize = 2;
const static uint8_t packetCrcSize = 2;

const static uint8_t payloadStartPosition = 5;

const static uint8_t minPacketSize = packetOverhead + packetCrcSize;

const static uint16_t maxPayloadLength = 256;

const static uint16_t maxPacketLength = maxPayloadLength + packetOverhead + packetLengthSize;

enum defaultRequestType
{
    linkTest = 0x01,
    hkRequest = 0x02
};

enum defaultResponseType
{
    framingErorr = 0x80,
    errorAck = 0x81,
    successAck = 0x82,
    hkResponse = 0x83
};
}  // namespace sip
}  // namespace outpost

#endif /* OUTPOST_SIP_OUTPOST_SIP_H_ */
