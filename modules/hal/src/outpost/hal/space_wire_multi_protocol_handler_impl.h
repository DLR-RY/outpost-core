/*
 * Copyright (c) 2013-2018, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2019, Jan Malburg (DLR RY-AVS)
 */

#ifndef OUTPOST_HAL_SPACEWIRE_MULTI_PROTOCOL_HANDLER_IMPL_H_
#define OUTPOST_HAL_SPACEWIRE_MULTI_PROTOCOL_HANDLER_IMPL_H_

#include "space_wire_multi_protocol_handler.h"

#include <outpost/utils/minmax.h>

namespace outpost
{
namespace hal
{
template <uint32_t maxPackages, uint32_t maxPacketSize>
bool
SpaceWireMultiProtocolHandler<maxPackages, maxPacketSize>::send(
        const outpost::Slice<const uint8_t>& buffer, outpost::time::Duration timeout)
{
    outpost::time::SpacecraftElapsedTime startTime = mClock.now();
    SpaceWire::TransmitBuffer* transmitBuffer;
    auto result = mSpWHandle.getSpaceWire().requestBuffer(transmitBuffer, timeout);
    if (result != SpaceWire::Result::Type::success)
    {
        return false;
    }

    // better reject to long packages than cutting them
    if (transmitBuffer->getLength() < buffer.getNumberOfElements())
    {
        return false;
    }
    else
    {
        if (!transmitBuffer->getData().copyFrom(buffer))
        {
            // buffer invalid?
            return false;
        }
        transmitBuffer->setLength(buffer.getNumberOfElements());
        transmitBuffer->setEndMarker(outpost::hal::SpaceWire::EndMarker::eop);
    }

    if (timeout != outpost::time::Duration::zero() && timeout < outpost::time::Duration::myriad())
    {
        // update remaining timeout unless we are in non-blocking anyways
        outpost::time::SpacecraftElapsedTime now = mClock.now();
        if (now > startTime + timeout)
        {
            return false;
        }
        else
        {
            timeout = timeout - (now - startTime);
        }
    }

    result = mSpWHandle.getSpaceWire().send(transmitBuffer, timeout);
    if (result == SpaceWire::Result::Type::success)
    {
        return true;
    }
    else
    {
        return false;
    }
}

template <uint32_t maxPackages, uint32_t maxPacketSize>
bool
SpaceWireMultiProtocolHandler<maxPackages, maxPacketSize>::SpaceWireHandle::receive(
        SpWMessage& data, outpost::time::Duration timeout)
{
    if (!mSpw.isUp())
    {
        mSpw.up(timeout);
        // will be called again anyways, so we don't handle the remaining timeout stuff
        return false;
    }

    SpaceWire::ReceiveBuffer receiveBuffer;
    auto result = mSpw.receive(receiveBuffer, timeout);
    if (result != SpaceWire::Result::Type::success)
    {
        return false;
    }
    else
    {
        bool success = false;
        uint32_t receivedSize = receiveBuffer.getLength();

        if (receivedSize >= protocolByteOffset + 1)
        {
            outpost::utils::SharedBufferPointer pointer;
            if (mPool.allocate(pointer))
            {
                uint32_t copySize = outpost::utils::min<uint32_t>(receivedSize, maxPacketSize);
                if (!pointer.asSlice().copyFrom(receiveBuffer.getData().getDataPointer(), copySize))
                {
                    mSpw.releaseBuffer(receiveBuffer);
                    return false;
                }
                outpost::utils::SharedChildPointer child;
                pointer.getChild(child, 0, 0, copySize);
                data.buffer = child;
                data.id.protocol = pointer[protocolByteOffset];
                if (receivedSize > maxPacketSize)
                {
                    // we cut the package
                    data.id.end = outpost::hal::SpaceWire::EndMarker::partial;
                }
                else
                {
                    data.id.end = receiveBuffer.getEndMarker();
                }
                success = true;
            }
        }

        mSpw.releaseBuffer(receiveBuffer);
        return success;
    }
}

template <uint32_t maxPackages, uint32_t maxPacketSize>
inline SpaceWire&
SpaceWireMultiProtocolHandler<maxPackages, maxPacketSize>::SpaceWireHandle::getSpaceWire()
{
    return mSpw;
}

template <uint32_t maxPackages, uint32_t maxPacketSize>
void
SpaceWireMultiProtocolHandler<maxPackages, maxPacketSize>::start()
{
    outpost::swb::BusDistributor<MessageID>::start();
}

/**
 * Add a listener for timecode
 * @param queue the queue to add
 * @return false if queue == nullptr or all places for Listener are filled
 */
template <uint32_t maxPackages, uint32_t maxPacketSize>
bool
SpaceWireMultiProtocolHandler<maxPackages, maxPacketSize>::addTimeCodeListener(
        outpost::rtos::Queue<TimeCode>* queue)
{
    return mSpWHandle.getSpaceWire().addTimeCodeListener(queue);
}

}  // namespace hal
}  // namespace outpost

#endif
