/*
 * Copyright (c) 2013-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2013-2017, Fabian Greif (DLR RY-AVS)
 */
// ----------------------------------------------------------------------------

#ifndef OUTPOST_HAL_SPACEWIRE_H
#define OUTPOST_HAL_SPACEWIRE_H

#include <stdint.h>
#include <cstddef>

#include <outpost/time/duration.h>
#include <outpost/container/bounded_array.h>

namespace outpost
{
namespace hal
{

/**
 * SpaceWire Interface
 *
 * \author    Fabian Greif
 */
class SpaceWire
{
public:

    enum EndMarker
    {
        partial = 0,    ///< Leave packet open to add additional data later
        eop = 1,        ///< End of packet
        eep = 2         ///< Error end of packet
    };

    struct Result
    {
        enum Type
        {
            success,
            failure,
            timeout
        };
    };

    /**
     * Send buffer.
     */
    class TransmitBuffer
    {
    public:
        inline
        TransmitBuffer() :
            mData(0),
            mLength(),
            mEnd(eop)
        {
        }

        inline explicit
        TransmitBuffer(outpost::BoundedArray<uint8_t> array) :
            mData(array.begin()),
            mLength(array.getNumberOfElements()),
            mEnd(eop)
        {
        }

        inline
        TransmitBuffer(const TransmitBuffer& other) :
            mData(other.mData),
            mLength(other.mLength),
            mEnd(other.mEnd)
        {
        }

        inline TransmitBuffer&
        operator=(const TransmitBuffer& other)
        {
            // This handles self assignment
            mData   = other.mData;
            mLength = other.mLength;
            mEnd    = other.mEnd;

            return *this;
        }

        inline size_t
        getLength() const
        {
            return mLength;
        }

        inline void
        setLength(size_t length)
        {
            mLength = length;
        }

        inline outpost::BoundedArray<uint8_t>
        getData()
        {
            outpost::BoundedArray<uint8_t> array(mData, mLength);
            return array;
        }

        inline EndMarker
        getEndMarker() const
        {
            return mEnd;
        }

        inline void
        setEndMarker(EndMarker end)
        {
            mEnd = end;
        }

        /**
         * Access elements of the array.
         *
         * \warning
         *      No out-of-bound error checking is performed.
         */
        inline uint8_t&
        operator[](size_t index)
        {
            return mData[index];
        }

    private:
        /**
         * Points to preallocated memory section.
         * Maximum size is implementation specific.
         */
        uint8_t* mData;
        size_t mLength;
        EndMarker mEnd;
    };

    class ReceiveBuffer
    {
    public:
        ReceiveBuffer() :
            mData(outpost::BoundedArray<const uint8_t>::empty()),
            mEnd(eop)
        {
        }

        ReceiveBuffer(outpost::BoundedArray<const uint8_t> data,
                      EndMarker end) :
            mData(data),
            mEnd(end)
        {
        }

        const ReceiveBuffer&
        operator=(const ReceiveBuffer& other)
        {
            // This handles self assignment
            mData = other.mData;
            mEnd = other.mEnd;

            return *this;
        }

        inline outpost::BoundedArray<const uint8_t>
        getData() const
        {
            return mData;
        }

        inline size_t
        getLength() const
        {
            return mData.getNumberOfElements();
        }

        inline EndMarker
        getEndMarker() const
        {
            return mEnd;
        }

        inline void
        setEndMarker(const EndMarker marker)
        {
            mEnd = marker;
        }

        /**
         * Access elements of the array.
         *
         * \warning
         *      No out-of-bound error checking is performed.
         */
        inline const uint8_t&
        operator[](size_t index) const
        {
            return mData[index];
        }

    private:
        outpost::BoundedArray<const uint8_t> mData;
        EndMarker mEnd;
    };

    virtual
    ~SpaceWire();

    /**
     * Get the maximum length of a SpaceWire packet.
     */
    virtual size_t
    getMaximumPacketLength() const = 0;

    /**
     * Open a configured SpaceWire device.
     *
     * \retval  true    If the device could be opened.
     * \retval  false   otherwise.
     */
    virtual bool
    open() = 0;

    virtual void
    close() = 0;

    virtual bool
    up(outpost::time::Duration timeout) = 0;

    virtual void
    down(outpost::time::Duration timeout) = 0;

    virtual bool
    isUp() = 0;


    /**
     * Request a send buffer.
     *
     * The SpaceWire link is blocked until the buffer is returned
     * by sending the buffer via send()!
     *
     * \param[out]  buffer
     *      Pointer to a send buffer. 0 in a case of a failure.
     * \param[in]   timeout
     *      Time to wait for a free transmit buffer.
     */
    virtual Result::Type
    requestBuffer(TransmitBuffer*& buffer,
                  outpost::time::Duration timeout) = 0;

    /**
     * Send a configured buffer.
     *
     * This releases the buffer.
     * The buffer must contain a complete message.
     *
     * \param[out]  buffer
     *      Pointer to a send buffer. Must be the same pointer which
     *      was requested via requestBuffer() earlier.
     * \param[in]   timeout
     *      Time to wait for a SpaceWire message to be sent. Only for blocking
     *      transmission
     */
    virtual Result::Type
    send(TransmitBuffer* buffer, outpost::time::Duration timeout) = 0;

    /**
     * Receive data.
     *
     * \param[out]  buffer
     *      Pointer to a receive buffer. 0 in a case of a failure.
     * \param[in]   timeout
     *      Time to wait for a SpaceWire message to arrive.
     */
    virtual Result::Type
    receive(ReceiveBuffer& buffer,
            outpost::time::Duration timeout) = 0;

    /**
     * Release receive buffer.
     *
     * \param[in]   buffer
     *      Send buffer. Must be the same pointer which was acquired
     *      by a receive() earlier.
     */
    virtual void
    releaseBuffer(const ReceiveBuffer& buffer) = 0;

    /**
     * Discard all messages currently waiting in the receive buffers.
     */
    virtual void
    flushReceiveBuffer() = 0;
};

}
}

#endif
