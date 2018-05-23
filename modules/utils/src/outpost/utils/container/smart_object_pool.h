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

#ifndef OUTPOST_UTILS_SMART_OBJECT_POOL_H_
#define OUTPOST_UTILS_SMART_OBJECT_POOL_H_

#include "smart_buffer.h"

#include <outpost/rtos/mutex_guard.h>
#include <outpost/utils/container/list.h>

namespace outpost
{
namespace utils
{
/**
 * Base class of the SmartObjectPool for passing by reference.
 *
 */
class SmartBufferPoolBase
{
public:
    virtual bool
    allocate(SmartBufferPointer& pointer) = 0;
    virtual size_t
    numberOfElements() const = 0;
    virtual size_t
    numberOfFreeElements() const = 0;

    virtual ~SmartBufferPoolBase()
    {
    }
};

/**
 * A SmartBufferPool holds poolable objects and allows for allocating these when needed.
 *
 * When looking for an unused element,
 * the element next to the one that was allocated last is considered first.
 *
 */
template <size_t E, size_t N>
class SmartBufferPool : public SmartBufferPoolBase
{
public:
    SmartBufferPool() : lastIndex(0)
    {
        for (size_t i = 0; i < N; i++)
        {
            mBuffer[i].setPointer(mDataBuffer[i], E);
        }
    }

    virtual ~SmartBufferPool()
    {
    }

    bool
    allocate(SmartBufferPointer& pointer)
    {
        outpost::rtos::MutexGuard lock(mMutex);
        bool res = false;
        size_t searchLastIndex = (lastIndex - 1) % N;
        size_t i = lastIndex;
        do
        {
            if (!mBuffer[i].isUsed())
            {
                pointer = SmartBufferPointer(&mBuffer[i]);
                res = true;
                lastIndex = i;
            }
            i = (i + 1) % N;
        } while (i != searchLastIndex && !res);
        return res;
    }

    void
    print() const
    {
        for (unsigned int i = 0; i < N; i++)
        {
            printf("SmartBuffer: %u (Address %p) : ", i, &mBuffer[i]);
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

    size_t
    numberOfElements() const
    {
        return N;
    }

    size_t
    numberOfFreeElements() const
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
    SmartBuffer mBuffer[N];

    size_t lastIndex;

    outpost::rtos::Mutex mMutex;
};

}  // namespace utils
}  // namespace outpost

#endif /* MODULES_UTILS_OBJECT_POOL_H_ */
