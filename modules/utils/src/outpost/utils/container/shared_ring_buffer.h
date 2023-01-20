/*
 * Copyright (c) 2017-2018, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2017-2018, Jan-Gerd Mess (DLR RY-AVS)
 * - 2018, Fabian Greif (DLR RY-AVS)
 * - 2021, Felix Passenberg (DLR RY-AVS)
 */

/**
 * \file
 * \author  Jan-Gerd Mess
 *
 * \brief FIFO ring buffer data structure for SharedBufferPointers.
 */

#ifndef MU_COMMON_UTILS_SMART_RING_BUFFER_H
#define MU_COMMON_UTILS_SMART_RING_BUFFER_H

#include "shared_buffer.h"

#include <outpost/base/slice.h>
#include <outpost/rtos/mutex_guard.h>

#include <stddef.h>
#include <stdint.h>
#include <string.h>

namespace outpost
{
namespace utils
{
/**
 * \ingroup SharedBuffer
 * \brief Ring buffer data structure for SharedBufferPointers.
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
        mBuffer(buffer), mFlags(flags), mReadIndex(0), mNumberOfUsedElements(0)
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
        outpost::rtos::MutexGuard lock(mMutex);
        size_t freeSlots = mBuffer.getNumberOfElements() - mNumberOfUsedElements;
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
        outpost::rtos::MutexGuard lock(mMutex);
        return mNumberOfUsedElements;
    }

    /**
     * \brief Stores an element to the next free slot (if available) and advances the writeIndex by
     * one.
     *
     * \param p SharedBufferPointer to be stored
     * \param flags Additional flags for the SharedBufferPointer that may be set.
     *
     * \return Returns true if the element could be stored in the SharedRingBuffer, otherwise false.
     */
    inline bool
    append(const SharedBufferPointer& p, uint8_t flags = 0)
    {
        outpost::rtos::MutexGuard lock(mMutex);
        bool appended = false;
        if ((mNumberOfUsedElements < mBuffer.getNumberOfElements()))
        {
            // calculate write index
            int writeIndex = increment(mReadIndex, mNumberOfUsedElements);

            mFlags[writeIndex] = flags;
            mBuffer[writeIndex] = p;
            ++mNumberOfUsedElements;

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
        outpost::rtos::MutexGuard lock(mMutex);
        return (mNumberOfUsedElements == 0);
    }

    /**
     * \brief Sets the current element's flags on the SharedRingBuffer
     * \param flags Flags to be set at the current read pointer.
     */
    inline bool
    setFlags(uint8_t flags, size_t index = 0)
    {
        outpost::rtos::MutexGuard lock(mMutex);
        if (!isEmpty() && index < mNumberOfUsedElements)
        {
            size_t position = increment(mReadIndex, index);
            mFlags[position] = flags;
            return true;
        }
        return false;
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
        outpost::rtos::MutexGuard lock(mMutex);
        bool elementRemoved = false;

        if (mNumberOfUsedElements > 0)
        {
            mBuffer[mReadIndex] = SharedBufferPointer();
            mFlags[mReadIndex] = 0;
            mReadIndex = increment(mReadIndex, 1);
            --mNumberOfUsedElements;
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
    peek(size_t index = 0) const
    {
        outpost::rtos::MutexGuard lock(mMutex);
        if (index < mNumberOfUsedElements)
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
    peekFlags(size_t index = 0) const
    {
        outpost::rtos::MutexGuard lock(mMutex);
        if (index < mNumberOfUsedElements)
        {
            size_t position = increment(mReadIndex, index);
            return mFlags[position];
        }
        return 0;
    }

    /**
     * \brief Resets the SharedRingBuffer, deleting all references.
     */
    inline void
    reset()
    {
        outpost::rtos::MutexGuard lock(mMutex);
        for (size_t i = 0; i < mNumberOfUsedElements; i++)
        {
            size_t position = increment(mReadIndex, i);
            mBuffer[position] = SharedBufferPointer();
            mFlags[position] = 0;
        }
        mReadIndex = 0;
        mNumberOfUsedElements = 0;
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
    size_t mNumberOfUsedElements;

    mutable outpost::rtos::Mutex mMutex;
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
        SharedRingBuffer(outpost::asSlice(mBufferStorage), outpost::asSlice(mFlags)),
        mBufferStorage(),
        mFlags()
    {
    }

    /**
     * \brief Standard destructor
     */
    virtual ~SharedRingBufferStorage() = default;

private:
    std::array<SharedBufferPointer, totalNumberOfElements> mBufferStorage;
    std::array<uint8_t, totalNumberOfElements> mFlags;
};

}  // namespace utils
}  // namespace outpost

#endif
