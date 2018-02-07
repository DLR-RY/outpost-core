/*
 * Copyright (c) 2016-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2016-2017, Fabian Greif (DLR RY-AVS)
 */
// ----------------------------------------------------------------------------

#include "spacewire_stub.h"

using unittest::hal::SpaceWireStub;

SpaceWireStub::SpaceWireStub(size_t maximumLength) :
    mMaximumLength(maximumLength),
    mOpen(false),
    mUp(false)
{
}

SpaceWireStub::~SpaceWireStub()
{
}

size_t
SpaceWireStub::getMaximumPacketLength() const
{
    return mMaximumLength;
}

bool
SpaceWireStub::open()
{
    mOpen = true;
    return mOpen;
}

void
SpaceWireStub::close()
{
    mOpen = false;
    mUp = false;
}

bool SpaceWireStub::up(outpost::time::Duration /*timeout*/)
{
    if (mOpen)
    {
        mUp = true;
    }
    return mUp;
}

void SpaceWireStub::down(outpost::time::Duration /*timeout*/)
{
    mUp = false;
}

bool
SpaceWireStub::isUp()
{
    return mUp;
}

SpaceWireStub::Result::Type
SpaceWireStub::requestBuffer(TransmitBuffer*& buffer, outpost::time::Duration /*timeout*/)
{
    Result::Type result = Result::success;

    std::unique_ptr<TransmitBufferEntry> entry(new TransmitBufferEntry(mMaximumLength));
    buffer = &entry->header;
    mTransmitBuffers.emplace(make_pair(&(entry->header), std::move(entry)));

    return result;
}

SpaceWireStub::Result::Type
SpaceWireStub::send(TransmitBuffer* buffer, outpost::time::Duration /*timeout*/)
{
    Result::Type result = Result::success;
    if (mUp)
    {
        try
        {
            std::unique_ptr<TransmitBufferEntry>& entry = mTransmitBuffers.at(buffer);
            mSentPackets.emplace_back(
                    Packet{std::vector<uint8_t>(&entry->buffer.front(),
                                                &entry->buffer.front() + entry->header.getLength()),
                           entry->header.getEndMarker()});
            mTransmitBuffers.erase(buffer);
        }
        catch (std::out_of_range&)
        {
            result = Result::failure;
        }
    }
    else
    {
        result = Result::failure;
    }

    return result;
}

SpaceWireStub::Result::Type
SpaceWireStub::receive(ReceiveBuffer& buffer, outpost::time::Duration /*timeout*/)
{
    Result::Type result = Result::success;
    if (mUp)
    {
        std::unique_ptr<ReceiveBufferEntry> entry(new ReceiveBufferEntry(
                std::move(mPacketsToReceive.front().data), mPacketsToReceive.front().end));
        buffer = entry->header;
        mReceiveBuffers.emplace(make_pair(entry->header.getData().begin(), std::move(entry)));
        mPacketsToReceive.pop_front();
    }
    else
    {
        result = Result::failure;
    }

    return result;
}

void
SpaceWireStub::releaseBuffer(const ReceiveBuffer& buffer)
{
    mReceiveBuffers.erase(buffer.getData().begin());
}

void
SpaceWireStub::flushReceiveBuffer()
{
    // Do nothing
}
