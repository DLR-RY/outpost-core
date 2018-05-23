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

#ifndef OUTPOST_UTILS_SMART_BUFFER_QUEUE_H_
#define OUTPOST_UTILS_SMART_BUFFER_QUEUE_H_

#include "smart_buffer.h"

#include <outpost/rtos/queue.h>

namespace outpost
{
namespace utils
{
class SmartBufferQueueBase : public outpost::rtos::Queue<size_t>
{
public:
    inline SmartBufferQueueBase(size_t N) : outpost::rtos::Queue<size_t>(N)
    {
    }

    virtual ~SmartBufferQueueBase() = default;

    virtual bool
    send(SmartBufferPointer& data) = 0;

    virtual bool
    receive(SmartBufferPointer& data,
            outpost::time::Duration timeout = outpost::time::Duration::infinity()) = 0;

    virtual uint16_t
    getNumberOfItems() = 0;
};

template <size_t N>
class SmartBufferQueue : public SmartBufferQueueBase
{
public:
    SmartBufferQueue() : SmartBufferQueueBase(N), mItemsInQueue(0), mLastIndex(0)
    {
        for (size_t i = 0; i < N; i++)
        {
            mIsUsed[i] = false;
        }
    }

    bool
    isEmpty()
    {
        outpost::rtos::MutexGuard lock(mMutex);
        for (size_t i = 0; i < N; i++)
        {
            if (mIsUsed[i])
            {
                return false;
            }
        }
        return true;
    }

    bool
    isFull()
    {
        outpost::rtos::MutexGuard lock(mMutex);
        /*for (size_t i = 0; i < N; i++)
        {
            if(!isUsed[i])
                return false;
        }
        return true;*/
        return mItemsInQueue == N;
    }

    virtual bool
    send(SmartBufferPointer& data) override
    {
        outpost::rtos::MutexGuard lock(mMutex);
        bool res = false;
        size_t i = mLastIndex;
        size_t endSearch = (mLastIndex - 1) % N;
        // for (size_t i = 0; i < N; i++)
        do
        {
            if (!mIsUsed[i])
            {
                mPointers[i] = data;
                mIsUsed[i] = true;
                mLastIndex = (i + 1) % N;
                if (outpost::rtos::Queue<size_t>::send(i))
                {
                    mItemsInQueue++;
                    res = true;
                }
                else
                {
                    mIsUsed[i] = false;
                    mPointers[i] = mEmpty;
                }
                break;
            }
            i = (i + 1) % N;
        } while (i != endSearch);
        return res;
    }

    virtual bool
    receive(SmartBufferPointer& data,
            outpost::time::Duration timeout = outpost::time::Duration::infinity()) override
    {
        bool res = false;
        size_t index;
        if (outpost::rtos::Queue<size_t>::receive(index, timeout))
        {
            outpost::rtos::MutexGuard lock(mMutex);
            data = SmartBufferPointer(mPointers[index]);
            mPointers[index] = mEmpty;
            mIsUsed[index] = false;
            mItemsInQueue--;
            res = true;
        }
        return res;
    }

    virtual uint16_t
    getNumberOfItems() override
    {
        outpost::rtos::MutexGuard lock(mMutex);
        return mItemsInQueue;
    }

private:
    SmartBufferPointer mEmpty;

    outpost::rtos::Mutex mMutex;

    uint16_t mItemsInQueue;

    size_t mLastIndex;

    SmartBufferPointer mPointers[N];
    bool mIsUsed[N];
};

}  // namespace utils
}  // namespace outpost

#endif /* MODULES_UTILS_SRC_MU_COMMON_UTILS_SMART_BUFFER_QUEUE_H_ */
