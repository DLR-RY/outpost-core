/*
 * Copyright (c) 2014-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2014-2017, Fabian Greif (DLR RY-AVS)
 */
// ----------------------------------------------------------------------------

#include "channel.h"

using namespace l3test::script;

Channel::Channel()
{
}

Channel::~Channel()
{
}

// ----------------------------------------------------------------------------
void
Channel::append(const uint8_t* data, size_t numberOfBytes)
{
    currentPacket.reserve(currentPacket.size() + numberOfBytes);

    for (size_t i = 0; i < numberOfBytes; ++i)
    {
        currentPacket.push_back(data[i]);
    }
}

void
Channel::finishPacket()
{
    packets.push_back(currentPacket);
    currentPacket.clear();
}

// ----------------------------------------------------------------------------
bool
Channel::hasPacket() const
{
    return (packets.size() > 0);
}

size_t
Channel::getNumberOfPackets() const
{
    return packets.size();
}

size_t
Channel::getPacketLength() const
{
    return packets.front().size();
}

Channel::Packet&
Channel::getPacket()
{
    return packets.front();
}

size_t
Channel::getPacket(uint8_t* data, size_t numberOfBytes) const
{
    auto length = std::min(numberOfBytes, getPacketLength());
    for (size_t i = 0; i < length; ++i)
    {
        data[i] = packets.front()[i];
    }

    return length;
}

void
Channel::nextPacket()
{
    packets.pop_front();
}
