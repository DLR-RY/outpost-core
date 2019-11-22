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

#ifndef OUTPOST_HAL_PROTOCOL_DISPATCHER_IMPL_H_
#define OUTPOST_HAL_PROTOCOL_DISPATCHER_IMPL_H_

#include "protocol_dispatcher.h"

#include <string.h>  // for memcpy

namespace outpost
{
namespace hal
{
template <typename protocolType, uint32_t numberOfQueues>
bool
ProtocolDispatcher<protocolType, numberOfQueues>::setDefaultQueue(
        outpost::utils::SharedBufferPoolBase* pool,
        outpost::utils::SharedBufferQueueBase* queue,
        bool dropPartial)
{
    if (queue == nullptr || pool == nullptr)
    {
        return false;
    }
    else if (mDefaultListener.mQueue != nullptr)
    {
        // only one default queue allowed
        return false;
    }
    else
    {
        mDefaultListener.mQueue = queue;
        mDefaultListener.mPool = pool;
        mDefaultListener.mDropPartial = dropPartial;
        mDefaultListener.mNumberOfDroppedPackages = 0;
        mDefaultListener.mNumberOfPartialPackages = 0;
        mDefaultListener.mNumberOfOverflowedBytes = 0;
        return true;
    }
}

template <typename protocolType, uint32_t numberOfQueues>
bool
ProtocolDispatcher<protocolType, numberOfQueues>::addQueue(
        protocolType id,
        outpost::utils::SharedBufferPoolBase* pool,
        outpost::utils::SharedBufferQueueBase* queue,
        bool dropPartial)
{
    outpost::rtos::MutexGuard lock(mMutex);
    if (queue == nullptr || pool == nullptr)
    {
        return false;
    }
    else if (mNumberOfListeners >= numberOfQueues)
    {
        // no free space
        return false;
    }
    else
    {
        mListeners[mNumberOfListeners].mQueue = queue;
        mListeners[mNumberOfListeners].mPool = pool;
        mListeners[mNumberOfListeners].mId = id;
        mListeners[mNumberOfListeners].mDropPartial = dropPartial;
        mNumberOfListeners++;
        return true;
    }
}

template <typename protocolType, uint32_t numberOfQueues>
inline uint32_t
ProtocolDispatcher<protocolType, numberOfQueues>::getNumberOfDroppedPackages(
        outpost::utils::SharedBufferQueueBase* queue)
{
    outpost::rtos::MutexGuard lock(mMutex);
    uint32_t ret = (mDefaultListener.mQueue == queue) ? mDefaultListener.mNumberOfDroppedPackages
                                                      : 0;
    for (uint32_t i = 0; i < mNumberOfListeners; i++)
    {
        if (mListeners[i].mQueue == queue)
        {
            ret += mListeners[i].mNumberOfDroppedPackages;
        }
    }
    return ret;
}

template <typename protocolType, uint32_t numberOfQueues>
inline uint32_t
ProtocolDispatcher<protocolType, numberOfQueues>::getNumberOfDroppedPackages()
{
    return mNumberOfDroppedPackages;
}

template <typename protocolType, uint32_t numberOfQueues>
inline uint32_t
ProtocolDispatcher<protocolType, numberOfQueues>::getNumberOfPartialPackages()
{
    return mNumberOfPartialPackages;
}

template <typename protocolType, uint32_t numberOfQueues>
inline uint32_t
ProtocolDispatcher<protocolType, numberOfQueues>::getNumberOfPartialPackages(
        outpost::utils::SharedBufferQueueBase* queue)
{
    outpost::rtos::MutexGuard lock(mMutex);
    uint32_t ret = (mDefaultListener.mQueue == queue) ? mDefaultListener.mNumberOfPartialPackages
                                                      : 0;
    for (uint32_t i = 0; i < mNumberOfListeners; i++)
    {
        if (mListeners[i].mQueue == queue)
        {
            ret += mListeners[i].mNumberOfPartialPackages;
        }
    }
    return ret;
}

template <typename protocolType, uint32_t numberOfQueues>
inline uint32_t
ProtocolDispatcher<protocolType, numberOfQueues>::getNumberOfOverflowedBytes()
{
    return mNumberOfOverflowedBytes;
}

template <typename protocolType, uint32_t numberOfQueues>
inline uint32_t
ProtocolDispatcher<protocolType, numberOfQueues>::getNumberOfOverflowedBytes(
        outpost::utils::SharedBufferQueueBase* queue)
{
    outpost::rtos::MutexGuard lock(mMutex);
    uint32_t ret = (mDefaultListener.mQueue == queue) ? mDefaultListener.mNumberOfOverflowedBytes
                                                      : 0;
    for (uint32_t i = 0; i < mNumberOfListeners; i++)
    {
        if (mListeners[i].mQueue == queue)
        {
            ret += mListeners[i].mNumberOfOverflowedBytes;
        }
    }
    return ret;
}

template <typename protocolType, uint32_t numberOfQueues>
inline uint32_t
ProtocolDispatcher<protocolType, numberOfQueues>::getNumberOfUnmatchedPackages()
{
    return mNumberOfUnmatchedPackages;
}

template <typename protocolType, uint32_t numberOfQueues>
inline void
ProtocolDispatcher<protocolType, numberOfQueues>::resetErrorCounters()
{
    outpost::rtos::MutexGuard lock(mMutex);
    mNumberOfDroppedPackages = 0;
    mNumberOfPartialPackages = 0;
    mNumberOfOverflowedBytes = 0;
    mNumberOfUnmatchedPackages = 0;

    for (unsigned int i = 0; i < numberOfQueues; i++)
    {
        mListeners[i].mNumberOfDroppedPackages = 0;
        mListeners[i].mNumberOfPartialPackages = 0;
        mListeners[i].mNumberOfOverflowedBytes = 0;
    }
    mDefaultListener.mNumberOfDroppedPackages = 0;
    mDefaultListener.mNumberOfPartialPackages = 0;
    mDefaultListener.mNumberOfOverflowedBytes = 0;
}

template <typename protocolType, uint32_t numberOfQueues>
void
ProtocolDispatcher<protocolType, numberOfQueues>::handlePackage()
{
    uint32_t readBytes = 0;
    outpost::Slice<uint8_t> tmp = mBuffer;  // for safety such the the length cannot be changed
    readBytes = mReceiver.receive(tmp, mWaitTime);
    if (readBytes > 0)  // 0 -> timeout
    {
        outpost::rtos::MutexGuard lock(mMutex);
        if (readBytes > mBuffer.getNumberOfElements())
        {
            uint32_t cut = readBytes - mBuffer.getNumberOfElements();
            mNumberOfPartialPackages++;
            mNumberOfOverflowedBytes += cut;
        }

        protocolType id;
        // we are conservative so we assume protocolType need an alignment but is not given
        // aligned in buffer
        memcpy(&id, &mBuffer[mOffset], sizeof(protocolType));

        bool found = false;
        bool dropped = true;

        for (unsigned int i = 0; i < mNumberOfListeners; i++)
        {
            if (mListeners[i].mId == id)
            {
                found = true;
                if (insertIntoQueue(mListeners[i], readBytes))
                {
                    dropped = false;
                }
            }
        }

        if (!found)
        {
            if (mDefaultListener.mQueue != nullptr)
            {
                if (insertIntoQueue(mDefaultListener, readBytes))
                {
                    dropped = false;
                }
            }
            else
            {
                mNumberOfUnmatchedPackages++;
            }
        }

        if (dropped)
        {
            mNumberOfDroppedPackages++;
        }
    }
}

template <typename protocolType, uint32_t numberOfQueues>
bool
ProtocolDispatcher<protocolType, numberOfQueues>::insertIntoQueue(
        ProtocolDispatcher<protocolType, numberOfQueues>::Listener& listener, uint32_t readBytes)
{
    bool inserted = false;

    outpost::utils::SharedBufferPointer sharedBuffer;
    if (listener.mPool->allocate(sharedBuffer))
    {
        // minimum of readBytes, buffer length and maxPacketSize
        uint32_t effectiveSize = (readBytes < sharedBuffer.getLength()) ? readBytes
                                                                        : sharedBuffer.getLength();
        effectiveSize = effectiveSize < mBuffer.getNumberOfElements()
                                ? effectiveSize
                                : mBuffer.getNumberOfElements();

        memcpy(&sharedBuffer->getPointer()[0], &mBuffer[0], effectiveSize);
        outpost::utils::SharedChildPointer child;
        sharedBuffer.getChild(child, 0, 0, effectiveSize);
        if (!listener.mDropPartial || effectiveSize >= readBytes)
        {
            inserted = listener.mQueue->send(child);
        }

        if (inserted)
        {
            if (effectiveSize < readBytes)
            {
                listener.mNumberOfOverflowedBytes += readBytes - effectiveSize;
                listener.mNumberOfPartialPackages++;
            }
        }
        else
        {
            listener.mNumberOfDroppedPackages++;
        }
    }
    else
    {
        listener.mNumberOfDroppedPackages++;
    }
    return inserted;
};

template <typename protocolType, uint32_t numberOfQueues>
void
ProtocolDispatcher<protocolType, numberOfQueues>::run()
{
    while (true)
    {
        outpost::support::Heartbeat::send(mHeartbeatSource, mWaitTime + mTolerance);
        handlePackage();
    }
}

}  // namespace hal
}  // namespace outpost

#endif /* OUTPOST_HAL_PROTOCOL_DISPATCHER_IMPL_H_ */
