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
 * Authors:
 * - 2021, Norbert Toth (DLR RY-AVS)
 */

#include "packet_writer.h"

#include <outpost/sip/sip.h>
#include <outpost/utils/coding/crc16.h>

outpost::sip::PacketWriter::PacketWriter(outpost::Slice<uint8_t> bufferIn) :
    mLength(0),
    mWorkerId(0),
    mCounter(0),
    mType(0),
    mPayloadBuffer(outpost::Slice<uint8_t>::empty()),
    mBufferToWrite(bufferIn)
{
}

outpost::sip::PacketWriter::~PacketWriter()
{
}

outpost::Slice<uint8_t>
outpost::sip::PacketWriter::get()
{
    outpost::Serialize packet(mBufferToWrite);
    packet.reset();

    // TODO: check payload and buffer size
    mLength = outpost::sip::packetOverhead + mPayloadBuffer.getNumberOfElements();
    packet.store<uint16_t>(mLength);
    // store WorkerId
    packet.store<uint8_t>(mWorkerId);
    // store counter
    packet.store<uint8_t>(mCounter++);
    // store Type
    packet.store<uint8_t>(mType);
    // store payloadData
    packet.store(mPayloadBuffer);

    // compute CRC
    const uint16_t crc = outpost::Crc16Ccitt::calculate(packet.asSlice());

    // store crc
    packet.store<uint16_t>(crc);

    return packet.asSlice();
}
