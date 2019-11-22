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

#ifndef OUTPOST_HAL_PROTOCOL_DISPATCHER_H_
#define OUTPOST_HAL_PROTOCOL_DISPATCHER_H_

#include <outpost/base/slice.h>
#include <outpost/rtos.h>
#include <outpost/rtos/queue.h>
#include <outpost/support/heartbeat.h>
#include <outpost/time/duration.h>
#include <outpost/utils/container/shared_buffer_queue.h>
#include <outpost/utils/container/shared_object_pool.h>

#include <stdint.h>
#include <string.h>  // for memcpy

#include <array>

namespace outpost
{
namespace hal
{
class Receiver
{
public:
    virtual ~Receiver() = default;

    /**
     * receives a data package
     *
     * @param buffer 	buffer to write the data to
     * @param timeout	max timeout to wait for data
     *
     * @return 0	If no package received in the time or failure in underlying receiver
     * 		   >0   Number of bytes received in package, if larger then
     * buffer.getNumberOfElements(), data has been lost
     */
    virtual uint32_t
    receive(outpost::Slice<uint8_t>& buffer, outpost::time::Duration timeout) = 0;
};

template <typename protocolType,    // pod and must support operator=, operator==, and default
                                    // constructor
          uint32_t numberOfQueues  // how many queues can be included
          >
class ProtocolDispatcher : public outpost::rtos::Thread
{
    static constexpr outpost::time::Duration waitTime = outpost::time::Seconds(10);
    static constexpr outpost::time::Duration tolerance = outpost::time::Seconds(1);

    friend class ProtocolDispatcherTest;  // that test can call handle package

public:
    /**
     * @param receiver        the object used to receive packages
     * @param buffer	      buffer for the received packages, should be equal or larger than the largest package that can be received, or data will be dropped, memory shall not used by any other
     * @param offset	      number of bytes before the protocol identifier
     * @param priority        see outpost::rtos::Thread
     * @param stackSize       see outpost::rtos::Thread
     * @param threadName      see outpost::rtos::Thread
     * @param heartbeatSource heartbeat id for the worker thread
     */
    ProtocolDispatcher(Receiver& receiver,
    				   outpost::Slice<uint8_t> buffer,
					   uint32_t offSet,
                       uint8_t priority,
                       size_t stackSize,
                       char* threadName,
                       outpost::support::parameter::HeartbeatSource heartbeatSource) :
        outpost::rtos::Thread(priority, stackSize, threadName),
        mReceiver(receiver),
        mNumberOfListener(0),
        mNumberOfDroppedPackages(0),
        mNumberOfUnmatchedPackages(0),
        mNumberOfPartialPackages(0),
        mNumberOfOverflowedBytes(0),
		mBuffer(buffer),
		mOffset(offSet),
        mHeartbeatSource(heartbeatSource)
    {
    }

    virtual ~ProtocolDispatcher()
    {
    }

    /**
     * This function is more for future debugging purposes,
     * sets a queue that will get all that no-one else matched
     *
     * @param[in] queue	The queue that will get all packages that are not matched by any regular
     * queue
     * @param[in] pool the pool to get the storage from
     * @param[in] dropPartial   if true only complete packages will be put into the queue
     *
     * @return	True 	if successfull
     * 			false 	if nullptr oder already set
     */
    bool
    setDefaultQueue(outpost::utils::SharedBufferPoolBase* pool,
                    outpost::utils::SharedBufferQueueBase* queue,
                    bool dropPartial = false)
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

    /**
     * Adds a queue for a specific protocol id.
     * This call can only succeed numberOfQueues times per dispatcher
     *
     * @param[in] id	The id value to listen to
     * @param[in] pool	The pool to allocate memory from
     * @param[in] queue	The queue to write the values to
     * @param[in] dropPartial   if true only complete packages will be put into the queue
     *
     * @return	true if successful
     * 			false	if nullpointer or all queue places filled up
     */
    bool
    addQueue(protocolType id,
             outpost::utils::SharedBufferPoolBase* pool,
             outpost::utils::SharedBufferQueueBase* queue,
             bool dropPartial = false)
    {
        outpost::rtos::MutexGuard lock(mMutex);
        if (queue == nullptr || pool == nullptr)
        {
            return false;
        }
        else if (mNumberOfListener == numberOfQueues)
        {
            // no free space
            return true;
        }
        else
        {
            mListener[mNumberOfListener].mQueue = queue;
            mListener[mNumberOfListener].mPool = pool;
            mListener[mNumberOfListener].mId = id;
            mListener[mNumberOfListener].mDropPartial = dropPartial;
            mNumberOfListener++;
            return true;
        }
    }

    /**
     * Return the number of packages that were drop for a given queue.
     * If a queue is listening to different protocol the sum is returned
     *
     * @param queue the queue
     * @return	sum of all packages dropped for the given queue
     */
    inline uint32_t
    getNumberOfDroppedPackages(outpost::utils::SharedBufferQueueBase* queue)
    {
        outpost::rtos::MutexGuard lock(mMutex);
        uint32_t ret =
                (mDefaultListener.mQueue == queue) ? mDefaultListener.mNumberOfDroppedPackages : 0;
        for (uint32_t i = 0; i < mNumberOfListener; i++)
        {
            if (mListener[i].mQueue == queue)
            {
                ret += mListener[i].mNumberOfDroppedPackages;
            }
        }
        return ret;
    }

    /**
     * @return	Returns the number of packages the were totally dropped,
     * 			i.e. don't got any queue or all queues where full
     */
    inline uint32_t
    getNumberOfDroppedPackages()
    {
        return mNumberOfDroppedPackages;
    }

    /**
     * @return Return the number of received packages that were cut because they exceeded
     * maxPacketSize
     */
    inline uint32_t
    getNumberOfPartialPackages()
    {
        return mNumberOfPartialPackages;
    }

    /**
     * Return the number of packages that were cut for a given queue.
     * If a queue is listening to different protocol the sum is returned
     *
     * @param queue the queue
     * @return	sum of all packages cut for the given queue
     */
    inline uint32_t
    getNumberOfPartialPackages(outpost::utils::SharedBufferQueueBase* queue)
    {
        outpost::rtos::MutexGuard lock(mMutex);
        uint32_t ret =
                (mDefaultListener.mQueue == queue) ? mDefaultListener.mNumberOfPartialPackages : 0;
        for (uint32_t i = 0; i < mNumberOfListener; i++)
        {
            if (mListener[i].mQueue == queue)
            {
                ret += mListener[i].mNumberOfPartialPackages;
            }
        }
        return ret;
    }

    /**
     * @return Return the number of bytes that were cut because packages exceeded maxPacketSize
     */
    inline uint32_t
    getNumberOfOverflowedBytes()
    {
        return mNumberOfOverflowedBytes;
    }

    /**
     * Return the number of bytes that were cut for a given queue.
     * If a queue is listening to different protocol the sum is returned
     *
     * @param queue the queue
     * @return	sum of all bytes cut for the given queue
     */
    inline uint32_t
    getNumberOfOverflowedBytes(outpost::utils::SharedBufferQueueBase* queue)
    {
        outpost::rtos::MutexGuard lock(mMutex);
        uint32_t ret =
                (mDefaultListener.mQueue == queue) ? mDefaultListener.mNumberOfOverflowedBytes : 0;
        for (uint32_t i = 0; i < mNumberOfListener; i++)
        {
            if (mListener[i].mQueue == queue)
            {
                ret += mListener[i].mNumberOfOverflowedBytes;
            }
        }
        return ret;
    }

    /**
     * @return Returns number of packages that could not be match to any queue,
     *         note will not increase once setRemainingQueue was executed successfully
     */
    inline uint32_t
    getNumberOfUnmatchedPackages()
    {
        return mNumberOfUnmatchedPackages;
    }

    /**
     * Resets all error counters back to 0
     */
    inline void
    resetErrorCounters()
    {
        outpost::rtos::MutexGuard lock(mMutex);
        mNumberOfDroppedPackages = 0;
        mNumberOfPartialPackages = 0;
        mNumberOfOverflowedBytes = 0;
        mNumberOfUnmatchedPackages = 0;

        for (unsigned int i = 0; i < numberOfQueues; i++)
        {
            mListener[i].mNumberOfDroppedPackages = 0;
            mListener[i].mNumberOfPartialPackages = 0;
            mListener[i].mNumberOfOverflowedBytes = 0;
        }
        mDefaultListener.mNumberOfDroppedPackages = 0;
        mDefaultListener.mNumberOfPartialPackages = 0;
        mDefaultListener.mNumberOfOverflowedBytes = 0;
    }

protected:
    void
    run()
    {
        while (true)
        {
            outpost::support::Heartbeat::send(mHeartbeatSource, waitTime + tolerance);
            handlePackage();
        }
    }

private:
    struct Listener
    {
        Listener() :
            mQueue(nullptr),
            mPool(0),
            mId(0),
            mNumberOfDroppedPackages(0),
            mNumberOfPartialPackages(0),
            mNumberOfOverflowedBytes(0),
            mDropPartial(false){};
        outpost::utils::SharedBufferQueueBase* mQueue;
        outpost::utils::SharedBufferPoolBase* mPool;
        protocolType mId;
        uint32_t mNumberOfDroppedPackages;
        uint32_t mNumberOfPartialPackages;
        uint32_t mNumberOfOverflowedBytes;
        bool mDropPartial;
    };

    void
    handlePackage()
    {
        uint32_t readBytes = 0;
        outpost::Slice<uint8_t> tmp = mBuffer; // for safety such the the length cannot be changed
        readBytes = mReceiver.receive(tmp, waitTime);
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

            for (unsigned int i = 0; i < mNumberOfListener; i++)
            {
                if (mListener[i].mId == id)
                {
                    found = true;
                    if (insertIntoQueue(mListener[i], readBytes))
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
            }

            if (dropped)
            {
                mNumberOfDroppedPackages++;
            }
        }
    }

    bool
    insertIntoQueue(Listener& listener, uint32_t readBytes)
    {
        bool inserted = false;

        outpost::utils::SharedBufferPointer sharedBuffer;
        if (listener.mPool->allocate(sharedBuffer))
        {
            // minimum of readBytes, buffer length and maxPacketSize
            uint32_t effectiveSize =
                    (readBytes < sharedBuffer.getLength()) ? readBytes : sharedBuffer.getLength();
            effectiveSize = effectiveSize < mBuffer.getNumberOfElements() ? effectiveSize : mBuffer.getNumberOfElements();

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

    Receiver& mReceiver;

    // one additional for the match rest one
    std::array<Listener, numberOfQueues> mListener;
    Listener mDefaultListener;
    uint32_t mNumberOfListener;
    uint32_t mNumberOfDroppedPackages;
    uint32_t mNumberOfUnmatchedPackages;
    uint32_t mNumberOfPartialPackages;
    uint32_t mNumberOfOverflowedBytes;
    outpost::rtos::Mutex mMutex;

    outpost::Slice<uint8_t> mBuffer;
    const uint32_t mOffset;

    const outpost::support::parameter::HeartbeatSource mHeartbeatSource;
};

}  // namespace hal
}  // namespace outpost

#endif /* MODULES_HAL_SRC_OUTPOST_HAL_PROTOCOL_DISPATCHER_H_ */
