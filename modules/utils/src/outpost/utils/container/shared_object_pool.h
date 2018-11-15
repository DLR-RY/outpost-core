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
 */

#ifndef OUTPOST_UTILS_SMART_OBJECT_POOL_H_
#define OUTPOST_UTILS_SMART_OBJECT_POOL_H_

#include "shared_buffer.h"

#include <outpost/rtos/mutex_guard.h>
#include <outpost/utils/container/list.h>

namespace outpost
{
namespace utils
{
/**
 * \ingroup SharedBuffer
 * \brief Base class of the SharedObjectPool for passing instances by reference.
 *
 */
class SharedBufferPoolBase
{
public:
    /**
     * \brief Allocation of an unused SharedBufferPoiner from the pool.
     *
     * \param pointer Reference to the SharedBufferPointer
     * \return Returns true if a valid SharedBudderPointer was found, otherwise false.
     */
    virtual bool
    allocate(SharedBufferPointer& pointer) = 0;

    /**
     * \brief Getter function for the overall number of elements in the pool.
     *
     * \return Returns the overall number of items in the pool, including occupied and unused
     * buffers.
     */
    virtual size_t
    numberOfElements() const = 0;

    /**
     * \brief Getter function for the number of free elements in the pool.
     *
     * \return Returns the number of unused items in the pool.
     */
    virtual size_t
    numberOfFreeElements() const = 0;

    /**
     * \brief Default destructor.
     *
     * Should not be called unless absolutely certain that all buffers in the pool are unused,
     * since this might free their underlying memory.
     */
    virtual ~SharedBufferPoolBase() = default;
};

/**
 * \ingroup SharedBuffer
 * \brief A SharedBufferPool holds SharedBuffer instances and allows for allocating matching
 * SharedBufferPointer instances these when needed.
 *
 * \tparam E Length of a single element in bytes
 * \tparam N Number of elements
 */
template <size_t E, size_t N>
class SharedBufferPool : public SharedBufferPoolBase
{
public:
    SharedBufferPool() : mLastIndex(0)
    {
        for (size_t i = 0; i < N; i++)
        {
            mBuffer[i].setPointer(outpost::Slice<uint8_t>::unsafe(mDataBuffer[i], E));
        }
    }

    /**
     * \brief Default destructor.
     *
     * Should not be called unless absolutely certain that all buffers in the pool are unused.
     */
    virtual ~SharedBufferPool() = default;

    /**
     * \brief Allocation of an unused SharedBufferPoiner from the pool.
     *
     *  When looking for an unused element,
     *  the element next to the one that was allocated last is considered first.
     *
     * \param pointer Reference to the SharedBufferPointer
     * \return Returns true if a valid SharedBudderPointer was found, otherwise false.
     */
    bool
    allocate(SharedBufferPointer& pointer) override
    {
        outpost::rtos::MutexGuard lock(mMutex);
        bool res = false;
        size_t searchLastIndex = (mLastIndex - 1) % N;
        size_t i = mLastIndex;
        do
        {
            if (!mBuffer[i].isUsed())
            {
                pointer = SharedBufferPointer(&mBuffer[i]);
                res = true;
                mLastIndex = i;
            }
            i = (i + 1) % N;
        } while (i != searchLastIndex && !res);
        return res;
    }

    /**
     * \brief Prints the current state (used, unused) of all SharedBufferPointers in the pool.
     *
     * Used for optimization and debugging only.
     */
    void
    print() const
    {
        for (unsigned int i = 0; i < N; i++)
        {
            printf("SharedBuffer: %u (Address %p) : ", i, &mBuffer[i]);
            if (mBuffer[i].isUsed())
            {
                printf("used (%lu)\n", mBuffer[i].getReferenceCount());
            }
            else
            {
                printf("free\n");
            }
        }
        printf("\n");
    }

    /**
     * \brief Getter function for the overall number of elements in the pool.
     *
     * \return Returns the overall number of items in the pool, including occupied and unused
     * buffers.
     */
    inline size_t
    numberOfElements() const override
    {
        return N;
    }

    /**
     * \brief Getter function for the number of free elements in the pool.
     *
     * \return Returns the number of unused items in the pool.
     */
    size_t
    numberOfFreeElements() const override
    {
        size_t temp = 0;
        for (size_t i = 0; i < N; i++)
        {
            if (!mBuffer[i].isUsed())
            {
                temp++;
            }
        }
        return temp;
    }

protected:
    uint8_t mDataBuffer[N][E] __attribute__((aligned(4)));
    SharedBuffer mBuffer[N];

    size_t mLastIndex;

    outpost::rtos::Mutex mMutex;
};

}  // namespace utils
}  // namespace outpost

#endif /* MODULES_UTILS_OBJECT_POOL_H_ */
