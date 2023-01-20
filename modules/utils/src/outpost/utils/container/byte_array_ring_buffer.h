/*
 * Copyright (c) 2014-2021, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2014-2015, 2017-2018, Fabian Greif (DLR RY-AVS)
 * - 2018, 2021 Jan-Gerd Mess (DLR RY-AVS)
 */

/**
 * \file
 * \author  Fabian Greif
 *
 * \brief FIFO ring buffer data structure.
 */

#ifndef OUTPOST_UTILS_BYTE_ARRAY_RING_BUFFER_H
#define OUTPOST_UTILS_BYTE_ARRAY_RING_BUFFER_H

#include <outpost/base/slice.h>

#include <stddef.h>
#include <stdint.h>
#include <string.h>  // for memcpy()

namespace outpost
{
namespace utils
{
/**
 * Ring buffer for byte arrays.
 *
 * \param elementSize
 *      Size of a single element.
 * \param totalNumberOfElements
 *      Number of elements to store
 *
 * \ingroup utils
 * \author  Fabian Greif
 */
template <size_t elementSize>
class ByteArrayRingBuffer
{
public:
    struct Element
    {
        size_t length = 0;
        uint8_t flags = 0;
        std::array<uint8_t, elementSize> buffer = {};
    };

    explicit inline ByteArrayRingBuffer(outpost::Slice<Element> buffer) :
        mBuffer(buffer), mReadIndex(0), mNumberOfElements(0)
    {
    }

    ByteArrayRingBuffer(const ByteArrayRingBuffer& o) = delete;

    ByteArrayRingBuffer&
    operator=(const ByteArrayRingBuffer& o) = delete;

    /**
     * Get the size of a single element
     *
     * \return Size of a single buffer in bytes
     */
    inline static size_t
    getElementSize()
    {
        return elementSize;
    }

    /**
     * Get the number of currently inactive count
     *
     * \return number of free count in the buffer
     */
    inline size_t
    getFreeSlots() const
    {
        size_t freeSlots = mBuffer.getNumberOfElements() - mNumberOfElements;
        return freeSlots;
    }

    /**
     * Get the number of currently active count.
     *
     * \return number of taken count in the buffer
     */
    inline size_t
    getUsedSlots() const
    {
        return mNumberOfElements;
    }

    /**
     * Write element to the buffer.
     *
     * \param[in] e new element of type T
     *
     * \retval false on overflow (element not added; buffer full)
     * \retval true  on success (element added)
     */
    inline bool
    append(outpost::Slice<const uint8_t> data, uint8_t flags = 0, bool pReset = true)
    {
        bool appended = false;
        if ((mNumberOfElements < mBuffer.getNumberOfElements())
            && (data.getNumberOfElements() <= elementSize))
        {
            // calculate write index
            int writeIndex = increment(mReadIndex, mNumberOfElements);

            mBuffer[writeIndex].flags = flags;
            if (pReset && mBuffer[writeIndex].length > data.getNumberOfElements())
            {
                memset(&mBuffer[writeIndex].buffer[data.getNumberOfElements()],
                       0,
                       mBuffer[writeIndex].length - data.getNumberOfElements());
            }
            mBuffer[writeIndex].length = data.getNumberOfElements();
            memcpy(&mBuffer[writeIndex].buffer, data.getDataPointer(), data.getNumberOfElements());
            ++mNumberOfElements;

            appended = true;
        }

        return appended;
    }

    /**
     * Check if the buffer is empty.
     *
     * \retval true  if the buffer is empty.
     * \retval false if the buffer contains at least one element.
     */
    inline bool
    isEmpty() const
    {
        return (mNumberOfElements == 0);
    }

    /**
     * Read element from the buffer.
     *
     */
    inline outpost::Slice<const uint8_t>
    read() const
    {
        return outpost::asSlice(mBuffer[mReadIndex].buffer).first(mBuffer[mReadIndex].length);
    }

    inline outpost::Slice<uint8_t>
    read()
    {
        return outpost::asSlice(mBuffer[mReadIndex].buffer).first(mBuffer[mReadIndex].length);
    }

    inline uint8_t
    readFlags() const
    {
        return mBuffer[mReadIndex].flags;
    }

    inline void
    setFlags(uint8_t flags)
    {
        mBuffer[mReadIndex].flags = flags;
    }

    /**
     *
     *
     * \retval false on underflow (no element read; buffer empty)
     * \retval true  on success
     */
    inline bool
    pop()
    {
        bool elementRemoved = false;

        if (mNumberOfElements > 0)
        {
            mReadIndex = increment(mReadIndex, 1);
            --mNumberOfElements;
            elementRemoved = true;
        }

        return elementRemoved;
    }

    /**
     * Provides the means to access a specific element.
     *
     * - Head: index = 0
     * - Tail: index = getUsedcount()
     *
     * \param[in] index of the element in question
     */
    inline outpost::Slice<const uint8_t>
    peek(size_t index) const
    {
        if (index <= mNumberOfElements)
        {
            size_t position = increment(mReadIndex, index);
            return outpost::asSlice(mBuffer[position].buffer).first(mBuffer[position].length);
        }

        return outpost::Slice<const uint8_t>::empty();
    }

    inline uint8_t
    peekFlags(size_t index) const
    {
        if (index <= mNumberOfElements)
        {
            size_t position = increment(mReadIndex, index);
            return mBuffer[position].flags;
        }
        return 0;
    }

    /**
     * Quick way of resetting the FIFO.
     */
    inline void
    reset()
    {
        mReadIndex = 0;
        mNumberOfElements = 0;
    }

    /**
     * Reset all elements from the given number onwards.
     *
     * E.g. resetElementsFrom(0) would reset the complete buffer,
     * resetElementsFrom(getUsedSlots()) does nothing.
     *
     * \param count
     *      Number of elements to skip before reseting elements.
     */
    inline void
    resetElementsFrom(size_t count)
    {
        if (mNumberOfElements > count)
        {
            mNumberOfElements = count;
        }
    }

private:
    inline size_t
    increment(size_t index, size_t count) const
    {
        size_t next = (index + count) % mBuffer.getNumberOfElements();
        return next;
    }

    const outpost::Slice<Element> mBuffer;
    size_t mReadIndex;
    size_t mNumberOfElements;
};

/**
 * Storage provider for ByteArrayRingBuffer.
 *
 * \author  Fabian Greif
 */
template <size_t elementSize, size_t numberOfElements>
class ByteArrayRingBufferStorage : public ByteArrayRingBuffer<elementSize>
{
public:
    inline ByteArrayRingBufferStorage() :
        ByteArrayRingBuffer<elementSize>(outpost::asSlice(mBufferStorage))
    {
    }

private:
    std::array<typename ByteArrayRingBuffer<elementSize>::Element, numberOfElements>
            mBufferStorage = {};
};

}  // namespace utils
}  // namespace outpost

#endif
