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
 * - 2017, Muhammad Bassam (DLR RY-AVS)
 */

#ifndef UNITTEST_HAL_SPACEWIRE_STUB_H
#define UNITTEST_HAL_SPACEWIRE_STUB_H

#include <outpost/hal/spacewire.h>

#include <list>
#include <map>
#include <memory>
#include <vector>

namespace unittest
{
namespace hal
{
/**
 * SpaceWire interface stub.
 *
 * Provides direct access to the send- and receive queue. The blocking mode
 * is ignored, every action happens instantly.
 *
 * To use this class fill the mPacketsToReceive member variable which should
 * be received during the test. In the mSentPackets all sent packets can be
 * collected after the test.
 *
 * \author  Fabian Greif
 */
class SpaceWireStub : public outpost::hal::SpaceWire
{
public:
    struct Packet
    {
        std::vector<uint8_t> data;
        EndMarker end;
    };

    /**
     * \param  maximumLength
     *      Maximum length of single SpaceWire packet.
     */
    SpaceWireStub(size_t maximumLength);

    virtual ~SpaceWireStub();

    virtual size_t
    getMaximumPacketLength() const;

    virtual bool
    open();

    virtual void
    close();

    virtual bool
    up(outpost::time::Duration timeout);

    virtual void
    down(outpost::time::Duration timeout);

    virtual bool
    isUp();

    virtual Result::Type
    requestBuffer(TransmitBuffer*& buffer, outpost::time::Duration timeout);

    virtual Result::Type
    send(TransmitBuffer* buffer, outpost::time::Duration timeout);

    virtual Result::Type
    receive(ReceiveBuffer& buffer, outpost::time::Duration timeout);

    virtual void
    releaseBuffer(const ReceiveBuffer& buffer);

    // Ignored in this implementation.
    virtual void
    flushReceiveBuffer();

    /**
     * Check that no transmit buffers are currently used.
     *
     * \retval  true    All transmit buffers have been returned by sending.
     * \retval  false   One or more transmit buffers are still in use by
     *                  the application.
     */
    bool
    noUsedTransmitBuffers()
    {
        return mTransmitBuffers.empty();
    }

    /**
     * Check that no receive buffer are in used.
     */
    bool
    noUsedReceiveBuffers()
    {
        return mReceiveBuffers.empty();
    }

    /**
     * Filled with all packets sent through SpaceWire interface.
     */
    std::list<Packet> mSentPackets;

    /**
     * Packets which can be received through the receive() function.
     *
     * Fill with data before starting the operation.
     */
    std::list<Packet> mPacketsToReceive;

private:
    struct TransmitBufferEntry
    {
        inline TransmitBufferEntry(size_t maximumLength) :
            buffer(maximumLength, 0),
            header(outpost::asSlice(buffer))
        {
        }

        std::vector<uint8_t> buffer;
        TransmitBuffer header;
    };

    struct ReceiveBufferEntry
    {
        ReceiveBufferEntry(std::vector<uint8_t>&& input, EndMarker end) :
            buffer(std::move(input)),
            header(outpost::asSlice(buffer), end)
        {
        }

        std::vector<uint8_t> buffer;
        ReceiveBuffer header;
    };

    const size_t mMaximumLength;
    bool mOpen;
    bool mUp;

    /**
     * Mapping between the transmit buffer given to the user and its storage
     * location.
     */
    std::map<TransmitBuffer*, std::unique_ptr<TransmitBufferEntry>> mTransmitBuffers;
    std::map<const uint8_t*, std::unique_ptr<ReceiveBufferEntry>> mReceiveBuffers;
};

}  // namespace hal
}  // namespace unittest

#endif
