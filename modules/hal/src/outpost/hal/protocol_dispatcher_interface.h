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

#ifndef OUTPOST_HAL_PROTOCOL_DISPATCHER_INTERFACE_H_
#define OUTPOST_HAL_PROTOCOL_DISPATCHER_INTERFACE_H_

#include <outpost/rtos.h>
#include <outpost/utils/container/shared_buffer_queue.h>
#include <outpost/utils/container/shared_object_pool.h>

#include <stdint.h>

namespace outpost
{
namespace hal
{
class ProtocolDispatcherInterfaceBase
{
public:
    ProtocolDispatcherInterfaceBase() = default;
    virtual ~ProtocolDispatcherInterfaceBase() = default;

    /**
     * Return the number of packages that were drop for a given queue.
     * If a queue is listening to different protocol the sum is returned
     *
     * @param queue the queue
     * @return	sum of all packages dropped for the given queue
     */
    virtual uint32_t
    getNumberOfDroppedPackages(outpost::utils::SharedBufferQueueBase* queue) = 0;

    /**
     * @return	Returns the number of packages the were totally dropped,
     * 			i.e. don't got any queue or all queues where full
     */
    virtual uint32_t
    getNumberOfDroppedPackages() = 0;

    /**
     * @return Return the number of received packages that were cut because they exceeded
     * maxPacketSize
     */
    virtual uint32_t
    getNumberOfPartialPackages() = 0;

    /**
     * Return the number of packages that were cut for a given queue.
     * If a queue is listening to different protocol the sum is returned
     *
     * @param queue the queue
     * @return	sum of all packages cut for the given queue
     */
    virtual uint32_t
    getNumberOfPartialPackages(outpost::utils::SharedBufferQueueBase* queue) = 0;

    /**
     * @return Return the number of bytes that were cut because packages exceeded maxPacketSize
     */
    virtual uint32_t
    getNumberOfOverflowedBytes() = 0;

    /**
     * Return the number of bytes that were cut for a given queue.
     * If a queue is listening to different protocol the sum is returned
     *
     * @param queue the queue
     * @return	sum of all bytes cut for the given queue
     */
    virtual uint32_t
    getNumberOfOverflowedBytes(outpost::utils::SharedBufferQueueBase* queue) = 0;

    /**
     * @return Returns number of packages that could not be match to any queue,
     *         note will not increase once setDefaultQueue was executed successfully
     */
    virtual uint32_t
    getNumberOfUnmatchedPackages() = 0;

    /**
     * Resets all error counters back to 0
     */
    virtual void
    resetErrorCounters() = 0;

    /**
     * Handles a package
     * @param package	The buffer containing the package
     * @param readBytes	The number of bytes in the packages may be larger then the buffer, in that
     * case the package has been cut
     */
    virtual void
    handlePackage(const outpost::Slice<const uint8_t>& package, uint32_t readBytes) = 0;
};

template <typename protocolType  // pod and must support operator=, operator==, and default
                                 // constructor
          >
class ProtocolDispatcherInterface : public ProtocolDispatcherInterfaceBase
{
public:
    ProtocolDispatcherInterface() = default;
    virtual ~ProtocolDispatcherInterface() = default;

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
     * 			false 	if nullptr or already set
     */
    virtual bool
    setDefaultQueue(outpost::utils::SharedBufferPoolBase* pool,
                    outpost::utils::SharedBufferQueueBase* queue,
                    bool dropPartial = false) = 0;

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
    virtual bool
    addQueue(protocolType id,
             outpost::utils::SharedBufferPoolBase* pool,
             outpost::utils::SharedBufferQueueBase* queue,
             bool dropPartial = false) = 0;
};

}  // namespace hal
}  // namespace outpost

#endif /* OUTPOST_HAL_PROTOCOL_DISPATCHER_INTERFACE_H_ */
