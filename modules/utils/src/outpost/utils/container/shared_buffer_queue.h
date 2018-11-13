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

#include <outpost/rtos/queue.h>
#include "shared_buffer.h"

namespace outpost
{
namespace utils
{
/**
 * \ingroup SharedBuffer
 * \brief Base class of the SharedBufferQueue for passing by reference.
 *
 * The standard RTOS/POSIX queues are not capable of handling SharedBufferPointers,
 * since they use pointers or standard constructors instead of references.
 * Hence, SharedBufferQueueBase inherits from outpost::rtos::Queue and adds a wrapper that keeps the references.
 */
class SharedBufferQueueBase : public outpost::rtos::Queue<size_t>
{
public:
    /**
     * Default destructor.
     */
    virtual ~SharedBufferQueueBase() = default;

    /**
     * \brief Send a SharedBufferPointer to the queue.
     * \param data SharedBufferPointer to be sent.
     * \return Returns true if data could be sent, false otherwise.
     */
    virtual bool
    send(SharedBufferPointer& data) = 0;

    /**
     *	\brief Receive a SharedBufferPointer from the queue.
     *
     *	Can be either blocking (timeout > 0) or non-blocking (timeout = 0).
     *	\param data Reference for the SharedBufferPointer to be received.
     *	\param timeout Duration for which the caller is willing to wait for an incoming SharedBufferPointer
     *	\return Returns true if a SharedBufferPointer was received, false otherwise (e.g. a timeout occured)
     */
    virtual bool
    receive(SharedBufferPointer& data,
            outpost::time::Duration timeout = outpost::time::Duration::infinity()) = 0;

    /**
     *	\brief Getter function for the number of items currently stored in the queue.
     *	\return Returns the number of items in the queue that are ready for receiving.
     */
    virtual uint16_t
    getNumberOfItems() = 0;

    /**
     *	\brief Checks whether the queue is currently empty.
     *	\return Returns true if there are no elements in the queue waiting to be received, false otherwise.
     */
    virtual bool
    isEmpty() = 0;

    /**
     *	\brief Checks whether there is a free slot in the queue.
     *	\return Returns true if data can be sent to the queue, false otherwise.
     */
    virtual bool
    isFull() = 0;

protected:
    /**
     * \brief Constructor for a SharedBufferQueueBase. May only be called by its derivatives (i.e. SharedBufferQueue)
     * \param N Maximum number of elements in the queue
     */
    inline SharedBufferQueueBase(size_t N) : outpost::rtos::Queue<size_t>(N)
    {
    }
};

/**
 * \ingroup SharedBuffer
 * \brief Implementation of a outpost::rtos::Queue that stores all additional information needed for passing SharedBufferPointer instances.
 */
template <size_t N>
class SharedBufferQueue : public SharedBufferQueueBase
{
public:
    /**
     *	\brief Standard constructor.
     */
    SharedBufferQueue() : SharedBufferQueueBase(N), mItemsInQueue(0), mLastIndex(0)
    {
        for (size_t i = 0; i < N; i++)
        {
            mIsUsed[i] = false;
        }
    }

    /**
     *	\brief Checks whether the queue is currently empty.
     *	\return Returns true if there are no elements in the queue waiting to be received, false otherwise.
     */
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

    /**
     *	\brief Checks whether there is a free slot in the queue.
     *	\return Returns true if data can be sent to the queue, false otherwise.
     */
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

    /**
     * \brief Send a SharedBufferPointer to the queue.
     * \see SharedBufferQueueBase::send(SharedBufferPointer&)
     * \param data SharedBufferPointer to be sent.
     * \return Returns true if data could be sent, false otherwise.
     */
    virtual bool
    send(SharedBufferPointer& data) override
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

    /**
     *	\brief Receive a SharedBufferPointer from the queue.
     *	\see SharedBufferQueueBase::receive(SharedBufferPointer&, outpost::time::Duration)
     *
     *	Can be either blocking (timeout > 0) or non-blocking (timeout = 0).
     *	\param data Reference for the SharedBufferPointer to be received.
     *	\param timeout Duration for which the caller is willing to wait for an incoming SharedBufferPointer
     *	\return Returns true if a SharedBufferPointer was received, false otherwise (e.g. a timeout occured)
     */
    virtual bool
    receive(SharedBufferPointer& data,
            outpost::time::Duration timeout = outpost::time::Duration::infinity()) override
    {
        bool res = false;
        size_t index;
        if (outpost::rtos::Queue<size_t>::receive(index, timeout))
        {
            outpost::rtos::MutexGuard lock(mMutex);
            data = SharedBufferPointer(mPointers[index]);
            mPointers[index] = mEmpty;
            mIsUsed[index] = false;
            mItemsInQueue--;
            res = true;
        }
        return res;
    }

    /**
     *	\brief Getter function for the number of items currently stored in the queue.
     *	\return Returns the number of items in the queue that are ready for receiving.
     */
    virtual uint16_t
    getNumberOfItems() override
    {
        outpost::rtos::MutexGuard lock(mMutex);
        return mItemsInQueue;
    }

private:
    SharedBufferPointer mEmpty;

    outpost::rtos::Mutex mMutex;

    uint16_t mItemsInQueue;

    size_t mLastIndex;

    SharedBufferPointer mPointers[N];
    bool mIsUsed[N];
};

}  // namespace utils
}  // namespace outpost

#endif /* OUTPOST_UTILS_SMART_BUFFER_QUEUE_H_ */
