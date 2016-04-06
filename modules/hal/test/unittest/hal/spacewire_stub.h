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

#ifndef UNITTEST_HAL_SPACEWIRE_STUB_H
#define UNITTEST_HAL_SPACEWIRE_STUB_H

#include <vector>
#include <map>
#include <memory>
#include <list>

#include <cobc/hal/spacewire.h>

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
class SpaceWireStub : public cobc::hal::SpaceWire
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

    virtual
    ~SpaceWireStub();

    virtual size_t
    getMaximumPacketLength() const;

    virtual bool
    open();

    virtual void
    close();

    virtual bool
    up(cobc::time::Duration timeout);

    virtual void
    down(cobc::time::Duration timeout);

    virtual bool
    isUp();


    virtual Result::Type
    requestBuffer(TransmitBuffer*& buffer,
                  cobc::time::Duration timeout);

    virtual Result::Type
    send(TransmitBuffer* buffer);

    virtual Result::Type
    receive(ReceiveBuffer& buffer,
            cobc::time::Duration timeout);

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
        inline
        TransmitBufferEntry(size_t maximumLength) :
            buffer(maximumLength, 0),
            header(&buffer.front())
        {
        }

        std::vector<uint8_t> buffer;
        TransmitBuffer header;
    };

    struct ReceiveBufferEntry
    {
        ReceiveBufferEntry(std::vector<uint8_t>&& input, EndMarker end) :
            buffer(std::move(input)),
            header()
        {
            header.data = &buffer.front();
            header.length = buffer.size();
            header.end = end;
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
    std::map<TransmitBuffer*, std::unique_ptr<TransmitBufferEntry> > mTransmitBuffers;
    std::map<const uint8_t*, std::unique_ptr<ReceiveBufferEntry> > mReceiveBuffers;
};

}
}

#endif
