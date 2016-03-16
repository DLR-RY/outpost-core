/*
 * Copyright (c) 2015, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.5.
 *
 * It is distributed under the terms of the GNU General Public License with a
 * linking exception. See the file "LICENSE" for the full license governing
 * this code.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
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

void
SpaceWireStub::up(Blocking /*blockingMode*/)
{
    if (mOpen)
    {
        mUp = true;
    }
}

void
SpaceWireStub::down(Blocking /*blockingMode*/)
{
    mUp = false;
}

bool
SpaceWireStub::isUp()
{
    return mUp;
}


SpaceWireStub::Result
SpaceWireStub::requestBuffer(TransmitBuffer*& buffer,
                             Blocking /*blockingMode*/)
{
    Result result = success;

    std::unique_ptr<TransmitBufferEntry> entry(new TransmitBufferEntry(mMaximumLength));
    buffer = &entry->header;
    mTransmitBuffers.emplace(make_pair(&(entry->header), std::move(entry)));

    return result;
}

SpaceWireStub::Result
SpaceWireStub::send(TransmitBuffer* buffer)
{
    Result result = success;
    if (mUp)
    {
        try
        {
            std::unique_ptr<TransmitBufferEntry>& entry = mTransmitBuffers.at(buffer);
            mSentPackets.emplace_back(Packet { std::vector<uint8_t>(&entry->buffer.front(),
                                                                    &entry->buffer.front() + entry->header.length),
                                               entry->header.end });
            mTransmitBuffers.erase(buffer);
        }
        catch (std::out_of_range&)
        {
            result = failure;
        }
    }
    else
    {
        result = failure;
    }

    return result;
}

SpaceWireStub::Result
SpaceWireStub::receive(ReceiveBuffer& buffer,
                       Blocking /*blockingMode*/)
{
    Result result = success;
    if (mUp)
    {
        std::unique_ptr<ReceiveBufferEntry> entry(new ReceiveBufferEntry(std::move(mPacketsToReceive.front().data),
                                                                         mPacketsToReceive.front().end));
        buffer = entry->header;
        mReceiveBuffers.emplace(make_pair(entry->header.data, std::move(entry)));
        mPacketsToReceive.pop_front();
    }
    else
    {
        result = failure;
    }

    return result;
}

void
SpaceWireStub::releaseBuffer(const ReceiveBuffer& buffer)
{
    mReceiveBuffers.erase(buffer.data);
}

void
SpaceWireStub::flushReceiveBuffer()
{
    // Do nothing
}
