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

#include "packet_reader.h"

#include <outpost/utils/coding/crc16.h>

outpost::sip::PacketReader::PacketReader(outpost::Slice<uint8_t> bufferIn) :
    mLength(0),
    mWorkerId(0),
    mCounter(0),
    mType(0),
    mPayloadDataLength(0),
    mCrc(0),
    mBuffer(bufferIn)
{
}

outpost::sip::PacketReader::~PacketReader()
{
}

outpost::sip::PacketReader::readResults
outpost::sip::PacketReader::readPacket()
{
    if (deserialize() == false)
    {
        // Length failed
        return outpost::sip::PacketReader::readResults::lengthError;
    }

    // compute CRC
    const uint16_t crcComputed = checkCrc();

    // check CRC
    if (mCrc != crcComputed)
    {
        // CRC failed
        return outpost::sip::PacketReader::readResults::crcError;
    }

    return outpost::sip::PacketReader::readResults::success;
}

bool
outpost::sip::PacketReader::deserialize()
{
    // check buffer size
    if (mBuffer.getNumberOfElements() < outpost::sip::minPacketSize)
    {
        // read out error
        return false;
    }

    outpost::Deserialize packet(mBuffer);

    mLength = packet.read<uint16_t>();
    mWorkerId = packet.read<uint8_t>();
    mCounter = packet.read<uint8_t>();
    mType = packet.read<uint8_t>();
    mPayloadDataLength = mLength - outpost::sip::packetOverhead;
    if (mPayloadDataLength > maxPayloadLength)
    {
        // Length failed
        return false;
    }
    packet.skip(mPayloadDataLength);
    mCrc = packet.read<uint16_t>();

    return true;
}

uint16_t
outpost::sip::PacketReader::checkCrc()
{
    return outpost::Crc16Ccitt::calculate(mBuffer.skipLast(packetCrcSize));
}
