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

#include "spacewire_multi_protocol.h"

#include <outpost/utils/minmax.h>

namespace outpost
{
namespace hal
{
template <uint32_t numberOfQueues, uint32_t maxPacketSize>
bool
SpacewireMultiProtocol<numberOfQueues, maxPacketSize>::send(
        const outpost::Slice<const uint8_t>& buffer, outpost::time::Duration timeout)
{
    outpost::time::SpacecraftElapsedTime start = mClock.now();
    SpaceWire::TransmitBuffer* transmitBuffer;
    auto result = mReceiver.mSpw.requestBuffer(transmitBuffer, timeout);
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
        memcpy(&transmitBuffer->getData()[0], &buffer[0], buffer.getNumberOfElements());
        transmitBuffer->setLength(buffer.getNumberOfElements());
    }

    outpost::time::SpacecraftElapsedTime now = mClock.now();
    if (now > start + timeout)
    {
        return false;
    }
    else
    {
        timeout = timeout - (now - start);
    }

    result = mReceiver.mSpw.send(transmitBuffer, timeout);
    return result == SpaceWire::Result::Type::success;
}

template <uint32_t numberOfQueues, uint32_t maxPacketSize>
uint32_t
SpacewireMultiProtocol<numberOfQueues, maxPacketSize>::SpaceWireReceiver::receive(
        outpost::Slice<uint8_t>& buffer, outpost::time::Duration timeout)
{
    SpaceWire::ReceiveBuffer receiveBuffer;
    auto result = mSpw.receive(receiveBuffer, timeout);
    if (result != SpaceWire::Result::Type::success)
    {
        return 0;
    }

    uint32_t receivedSize = receiveBuffer.getLength();

    uint32_t copySize = outpost::utils::min<uint32_t>(receivedSize, buffer.getNumberOfElements());
    memcpy(&buffer[0], &receiveBuffer.getData()[0], copySize);

    mSpw.releaseBuffer(receiveBuffer);
    return receivedSize;
}

}  // namespace hal
}  // namespace outpost
