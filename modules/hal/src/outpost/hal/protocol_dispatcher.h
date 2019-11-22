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

#include "protocol_dispatcher_interface.h"
#include "receiver_interface.h"

#include <outpost/base/slice.h>
#include <outpost/rtos.h>
#include <outpost/support/heartbeat.h>
#include <outpost/time/duration.h>
#include <outpost/utils/container/shared_buffer_queue.h>
#include <outpost/utils/container/shared_object_pool.h>

#include <stdint.h>

#include <array>

namespace outpost
{
namespace hal
{
template <typename protocolType,   // pod and must support operator=, operator==, and default
                                   // constructor
          uint32_t numberOfQueues  // how many queues can be included
          >
class ProtocolDispatcher : public outpost::rtos::Thread,
                           public virtual ProtocolDispatcherInterface<protocolType>
{
    static constexpr outpost::time::Duration waitTime = outpost::time::Seconds(10);
    static constexpr outpost::time::Duration tolerance = outpost::time::Seconds(1);

    friend class ProtocolDispatcherTest;  // that test can call handle package

public:
    /**
     * @param receiver        the object used to receive packages
     * @param buffer	      buffer for the received packages, should be equal or larger than the
     * largest package that can be received, or data will be dropped, memory shall not used by any
     * other
     * @param offset	      number of bytes before the protocol identifier
     * @param priority        see outpost::rtos::Thread
     * @param stackSize       see outpost::rtos::Thread
     * @param threadName      see outpost::rtos::Thread
     * @param heartbeatSource heartbeat id for the worker thread
     * @param waitTime		  Time to wait on a receive (i.e. roughly max the time between
     * heartbeats)
     * @param tolerance		  Small time addition to insert data into the queues, must be larger than
     * zero)
     */
    ProtocolDispatcher(ReceiverInterface& receiver,
                       outpost::Slice<uint8_t> buffer,
                       uint32_t offSet,
                       uint8_t priority,
                       size_t stackSize,
                       char* threadName,
                       outpost::support::parameter::HeartbeatSource heartbeatSource,
                       outpost::time::Duration waitTime = outpost::time::Seconds(10),
                       outpost::time::Duration tolerance = outpost::time::Seconds(1)) :
        outpost::rtos::Thread(priority, stackSize, threadName),
        mReceiver(receiver),
        mNumberOfListeners(0),
        mNumberOfDroppedPackages(0),
        mNumberOfUnmatchedPackages(0),
        mNumberOfPartialPackages(0),
        mNumberOfOverflowedBytes(0),
        mBuffer(buffer),
        mOffset(offSet),
        mHeartbeatSource(heartbeatSource),
        mWaitTime(waitTime),
        mTolerance(tolerance)
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
     * @param[in] pool the pool to get the storage from, the provided memories shall be large enough
     * to fit a package
     * @param[in] dropPartial   if true only complete packages will be put into the queue
     *
     * @return	True 	if successfull
     * 			false 	if nullptr oder already set
     */
    bool
    setDefaultQueue(outpost::utils::SharedBufferPoolBase* pool,
                    outpost::utils::SharedBufferQueueBase* queue,
                    bool dropPartial = false) override;

    /**
     * Adds a queue for a specific protocol id.
     * This call can only succeed numberOfQueues times per dispatcher
     *
     * @param[in] id	The id value to listen to
     * @param[in] pool	The pool to allocate memory from, the provided memories shall be large
     * enough to fit a package of the specific protocol
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
             bool dropPartial = false) override;

    /**
     * Return the number of packages that were dropped for a given queue.
     * If a queue is listening to different protocol the sum is returned
     *
     * @param queue the queue
     * @return	sum of all packages dropped for the given queue
     */
    inline uint32_t
    getNumberOfDroppedPackages(outpost::utils::SharedBufferQueueBase* queue) override;

    /**
     * @return	Returns the number of packages the were totally dropped,
     * 			i.e. don't got any queue or all queues where full
     */
    inline uint32_t
    getNumberOfDroppedPackages() override;

    /**
     * @return Return the number of received packages that were cut because they exceeded
     * maxPacketSize
     */
    inline uint32_t
    getNumberOfPartialPackages() override;

    /**
     * Return the number of packages that were cut for a given queue.
     * If a queue is listening to different protocol the sum is returned
     *
     * @param queue the queue
     * @return	sum of all packages cut for the given queue
     */
    inline uint32_t
    getNumberOfPartialPackages(outpost::utils::SharedBufferQueueBase* queue) override;

    /**
     * @return Return the number of bytes that were cut because packages exceeded maxPacketSize
     */
    inline uint32_t
    getNumberOfOverflowedBytes() override;

    /**
     * Return the number of bytes that were cut for a given queue.
     * If a queue is listening to different protocol the sum is returned
     *
     * @param queue the queue
     * @return	sum of all bytes cut for the given queue
     */
    inline uint32_t
    getNumberOfOverflowedBytes(outpost::utils::SharedBufferQueueBase* queue) override;

    /**
     * @return Returns number of packages that could not be match to any queue,
     *         note will not increase once setRemainingQueue was executed successfully
     */
    inline uint32_t
    getNumberOfUnmatchedPackages() override;

    /**
     * Resets all error counters back to 0
     */
    inline void
    resetErrorCounters() override;

protected:
    void
    run();

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
    handlePackage();

    bool
    insertIntoQueue(Listener& listener, uint32_t readBytes);

    ReceiverInterface& mReceiver;

    // one additional for the match rest one
    std::array<Listener, numberOfQueues> mListeners;
    Listener mDefaultListener;
    uint32_t mNumberOfListeners;
    uint32_t mNumberOfDroppedPackages;
    uint32_t mNumberOfUnmatchedPackages;
    uint32_t mNumberOfPartialPackages;
    uint32_t mNumberOfOverflowedBytes;
    outpost::rtos::Mutex mMutex;

    outpost::Slice<uint8_t> mBuffer;
    const uint32_t mOffset;

    const outpost::support::parameter::HeartbeatSource mHeartbeatSource;

    const outpost::time::Duration mWaitTime;
    const outpost::time::Duration mTolerance;
};

}  // namespace hal
}  // namespace outpost

#include "protocol_dispatcher_impl.h"

#endif /* OUTPOST_HAL_PROTOCOL_DISPATCHER_H_ */
