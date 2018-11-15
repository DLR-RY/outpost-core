/*
 * Copyright (c) 2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2017, Jan-Gerd Mess (DLR RY-AVS)
 */
// ----------------------------------------------------------------------------

/**
 * \file
 * \author  Jan-Gerd Mess
 *
 * \brief FIFO ring buffer data structure for SharedBuffers.
 */

#ifndef MU_COMMON_UTILS_SMART_RING_BUFFER_H
#define MU_COMMON_UTILS_SMART_RING_BUFFER_H

#include "shared_buffer.h"

#include <outpost/base/slice.h>

#include <stddef.h>
#include <stdint.h>
#include <string.h>

namespace outpost
{
namespace utils
{
/**
 * \ingroup SharedBuffer
 * \brief Ring buffer data structure for SharedBuffers.
 *
 *
 * \author  Jan-Gerd Mess
 */
class SharedRingBuffer
{
public:
    /**
     * \brief Constructor for a SharedRingBuffer based on a Slice of SharedBufferPointers and a
     * Slice byte array of flags. Both have to have the same length. Flags can be used by
     * superordinate data structures to mark elements, e.g. as being in use or ready for deletion.
     */
    inline SharedRingBuffer(outpost::Slice<SharedBufferPointer> buffer,
                            outpost::Slice<uint8_t> flags) :
        mBuffer(buffer),
        mFlags(flags),
        mReadIndex(0),
        mNumberOfElements(0)
    {
    }

    /**
     * \brief Standard destructor
     */
    virtual ~SharedRingBuffer() = default;

    SharedRingBuffer(const SharedRingBuffer& o) = delete;

    SharedRingBuffer&
    operator=(const SharedRingBuffer& o) = delete;

    /**
     * \brief Getter function for the number of free slots in the SharedRingBuffer
     *
     * \return Returns the number of unoccupied slots in the SharedRingBuffer
     */
    inline size_t
    getFreeSlots() const
    {
        size_t freeSlots = mBuffer.getNumberOfElements() - mNumberOfElements;
        return freeSlots;
    }

    /**
     * \brief Getter function for the number of currently occupied slots in the SharedRingBuffer.
     *
     * \return Returns the number of elements currently stored in the SharedRingBuffer
     */
    inline size_t
    getUsedSlots() const
    {
        return mNumberOfElements;
    }

    /**
     * \brief Stores an element to the first unoccupied index it finds and updates the writeIndex.
     *
     * \param p SharedBufferPointer to be stored
     * \param flags Additional flags for the SharedBufferPointer that may be set.
     *
     * \return Returns true if the element could be stored in the SharedRingBuffer, otherwise false.
     */
    inline bool
    append(const SharedBufferPointer& p, uint8_t flags = 0)
    {
        bool appended = false;
        if ((mNumberOfElements < mBuffer.getNumberOfElements()))
        {
            // calculate write index
            int writeIndex = increment(mReadIndex, mNumberOfElements);

            mFlags[writeIndex] = flags;
            mBuffer[writeIndex] = p;
            ++mNumberOfElements;

            appended = true;
        }

        return appended;
    }

    /**
     * \brief Checks if the buffer is empty.
     *
     * \return Returns true  if the buffer is empty, otherwise false.
     */
    inline bool
    isEmpty() const
    {
        return (mNumberOfElements == 0);
    }

    /**
     * \brief Reads one element from the SharedRingBuffer
     * \return Returns the element at the current read pointer. The user has to perform all validity
     * checks.
     */
    inline const SharedBufferPointer&
    read() const
    {
        return mBuffer[mReadIndex];
    }

    /**
     * \brief Reads one element from the SharedRingBuffer
     * \return Returns the element at the current read pointer. The user has to perform all validity
     * checks.
     */
    inline SharedBufferPointer&
    read()
    {
        return mBuffer[mReadIndex];
    }

    /**
     * \brief Reads the current element's flags from the SharedRingBuffer
     * \return Returns the byte of flags at the current read pointer.
     */
    inline uint8_t
    readFlags() const
    {
        return mFlags[mReadIndex];
    }

    /**
     * \brief Sets the current element's flags on the SharedRingBuffer
     * \param flags Flags to be set at the current read pointer.
     */
    inline void
    setFlags(uint8_t flags)
    {
        mFlags[mReadIndex] = flags;
    }

    /**
     * \brief Deletes the element at the current read pointer from the SharedRingBuffer.
     *
     * \return Returns true if the element was removed, false if the buffer is empty and no element
     * was removed.
     */
    inline bool
    pop()
    {
        bool elementRemoved = false;

        if (mNumberOfElements > 0)
        {
            mBuffer[mReadIndex] = SharedBufferPointer();
            mReadIndex = increment(mReadIndex, 1);
            --mNumberOfElements;
            elementRemoved = true;
        }

        return elementRemoved;
    }

    /**
     * \brief Provides the means to access one specific element.
     *
     * Indexing starts that the current read pointer.
     *
     * - Head: index = 0
     * - Tail: index = getUsedcount()
     *
     * \param index Index of the element to be read
     * \return SharedBufferPointer found at the given index (can be invalid)
     */
    inline const SharedBufferPointer&
    peek(size_t index) const
    {
        if (index <= mBuffer.getNumberOfElements())
        {
            size_t position = increment(mReadIndex, index);
            return mBuffer[position];
        }

        return mEmpty;
    }

    /**
     * \brief Provides the means to access one specific element's flags.
     *
     * Indexing starts that the current read pointer.
     *
     * - Head: index = 0
     * - Tail: index = getUsedcount()
     *
     * \param index Index of the element whose flags shall be read
     * \return Flags found at the given index (can be invalid)
     */
    inline uint8_t
    peekFlags(size_t index) const
    {
        size_t position = increment(mReadIndex, index);
        return mFlags[position];
    }

    /**
     * \brief Resets the SharedRingBuffer, deleting all references.
     */
    inline void
    reset()
    {
        mReadIndex = 0;
        mNumberOfElements = 0;
        for (size_t i = 0; i < mBuffer.getNumberOfElements(); i++)
        {
            mBuffer[i] = SharedBufferPointer();
        }
    }

private:
    inline size_t
    increment(size_t index, size_t count) const
    {
        size_t next = (index + count) % mBuffer.getNumberOfElements();
        return next;
    }

    // Dummy item. Used when the buffer is empty.
    SharedBufferPointer mEmpty;

    const outpost::Slice<SharedBufferPointer> mBuffer;
    const outpost::Slice<uint8_t> mFlags;

    size_t mReadIndex;
    size_t mNumberOfElements;
};

/**
 * \ingroup SharedBuffer
 * Storage provider for the SharedRingBuffer.
 *
 * \tparam totalNumberOfElements Maximum number of elements to be stored in the SharedRingBuffer
 *
 */
template <size_t totalNumberOfElements>
class SharedRingBufferStorage : public SharedRingBuffer
{
public:
    /**
     * \brief Standard constructor
     */
    inline SharedRingBufferStorage() :
        SharedRingBuffer(outpost::asSlice(mBufferStorage), outpost::asSlice(mFlags))
    {
    }

    /**
     * \brief Standard detructor
     */
    virtual ~SharedRingBufferStorage() = default;

private:
    SharedBufferPointer mBufferStorage[totalNumberOfElements];
    uint8_t mFlags[totalNumberOfElements];
};

}  // namespace utils
}  // namespace outpost

#endif
