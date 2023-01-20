/*
 * Copyright (c) 2020, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2020, Jan-Gerd Mess (DLR RY-AVS)
 * - 2021, Jan Malburg (DLR RY-AVS)
 */

#ifndef OUTPOST_SWB_SOFTWARE_BUS_H
#define OUTPOST_SWB_SOFTWARE_BUS_H

#include "bus_channel.h"
#include "bus_distributor.h"
#include "bus_handler_thread.h"
#include "types.h"

#include <outpost/utils/container/shared_buffer.h>

#include <stdint.h>

#include <type_traits>

namespace outpost
{
namespace utils
{
class SharedBufferPoolBase;
class ConstSharedBufferPointer;

template <typename T>
class ReferenceQueueBase;
}  // namespace utils

namespace swb
{
template <typename IDType>
class BusChannel;

template <typename IDType>
class BusHandlerThread;

/**
 * Declares the copy mode
 * \ingroup swb
 */
enum class CopyMode : uint8_t
{
    /** Copy once during send() such that the caller can keep using its buffer */
    copy_once,
    /** Do not copy, the buffer will be freed once it has been distributed to all recipients. */
    zero_copy
};

/**
 * The SoftwareBus allows for asynchronous exchange of Message between recipients and senders.
 * Senders can sent Message from their own memory pool or use the bus' pool. Receiver can register
 * channels that filter for certain types of Message using their id and/ or data. Houskeeping is
 * also provided through a number of counters.
 * \ingroup swb
 * \param IDType type of the message ID
 */
template <typename IDType>
class SoftwareBus : public BusDistributor<IDType>
{
public:
    /**
     * Constructor.
     * \param pool SharedBufferPoolBase from which the SoftwwareBus shall allocate its memory
     * \param queue ReferenceQueueBase to which Message will be sent. For internal use only.
     * \param priority Priority of the internal BusHandlerThread
     * \param heartbeatSource HeartbeatSource for the software watchdog.
     */
    SoftwareBus(outpost::utils::SharedBufferPoolBase& pool,
                outpost::utils::ReferenceQueueBase<Message<IDType>>& queue,
                uint8_t priority,
                outpost::support::parameter::HeartbeatSource heartbeatSource);

    /**
     * Destructor
     */
    virtual ~SoftwareBus() = default;

    /**
     * Send a Slice to the SoftwareBus identified by an ID.
     * Note: The message's data is copied to a buffer in the SoftwareBus' pool.
     * \param id Identifier for the message
     * \param data Data to be sent.
     */
    OperationResult
    sendMessage(const IDType id, const outpost::Slice<const uint8_t>& data);

    /**
     * Send a ConstSharedBufferPointer to the SoftwareBus, identified by an ID.
     * \param id Identifier for the message
     * \param buffer Buffer holding the message's data.
     * \param mode CopyMode to be used for the sendMessage operation.
     */
    OperationResult
    sendMessage(const IDType id,
                const outpost::utils::ConstSharedBufferPointer& buffer,
                CopyMode mode = CopyMode::zero_copy);

    /**
     * Sends a Message to the SoftwareBus. CopyMode::zero_copy is used for the operation.
     * \param msg Message to be sent.
     */
    OperationResult
    sendMessage(Message<IDType>& msg);

    /**
     * Getter for the number of Message that were declined by a potential filter.
     * \return Returns the number of declined Message.
     */
    inline uint32_t
    getNumberOfDeclinedMessages() const
    {
        return mNumberOfInvalidMessages;
    }

    /**
     * Getter for the number of failed copy operations when using CopyMode::copy_once.
     * \return Returns the number of failed copy operations due to overlong messages or an
     *         insufficient number of buffers.
     */
    inline uint32_t
    getNumberOfFailedCopyOperations() const
    {
        return mNumberOfFailedCopyOperations;
    }

    /**
     * Getter for the number of failed send operations caused by the internal queue.
     * \return Returns the number of failed send operations.
     */
    inline uint32_t
    getNumberOfFailedSendOperations() const
    {
        return mNumberOfFailedSendOperations;
    }

    /**
     * Getter for the number of accepted messages.
     * \return Returns the number of accepted messages.
     */
    inline uint32_t
    getNumberOfAcceptedMessages() const
    {
        return mNumberOfAcceptedMessages;
    }

protected:
    /**
     * Checks an incoming message for validity.
     * \return Returns true if the message is valid for sending, false otherwise.
     */
    virtual bool
    valid(IDType, const outpost::Slice<const uint8_t>&)
    {
        return true;
    }

private:
    OperationResult
    copySliceToBuffer(const outpost::Slice<const uint8_t>& slice,
                      outpost::utils::ConstSharedBufferPointer& buffer);

    outpost::rtos::Mutex mMutex;

    outpost::utils::SharedBufferPoolBase& mPool;
    outpost::utils::ReferenceQueueBase<Message<IDType>>& mQueue;

    uint32_t mNumberOfInvalidMessages;
    uint32_t mNumberOfFailedCopyOperations;
    uint32_t mNumberOfFailedSendOperations;
    uint32_t mNumberOfAcceptedMessages;
};

/**
 * Variant of the SoftwareBus that uses in incoming filter to limit the type of Message that is
 * accepted for distribution.
 * \ingroup swb
 * \param IDType type of the message id
 * \param Filter the type of the filter, must be default constructable and contains bool
 *               operator()(const IDType&, const outpost::Slice<const uint8_t>&) that returns true
 *               if the Bus shall be able to send the message
 */
template <typename IDType, typename Filter>
class SoftwareBusFiltered : public SoftwareBus<IDType>
{
public:
    SoftwareBusFiltered(outpost::utils::SharedBufferPoolBase& pool,
                        outpost::utils::ReferenceQueueBase<Message<IDType>>& queue,
                        uint8_t priority,
                        outpost::support::parameter::HeartbeatSource heartbeatSource) :
        SoftwareBus<IDType>(pool, queue, priority, heartbeatSource)
    {
        static_assert(std::is_convertible<Filter*, outpost::swb::MessageFilter<IDType>*>::value,
                      "Filter must be a MessageFilter");
        static_assert(std::is_default_constructible<Filter>::value,
                      "Filter must be default constructible");
    }

    virtual ~SoftwareBusFiltered() = default;

    /**
     * Getter for a writeable reference to the filter. Allows for setting the specific filter after
     * constructing the class.
     * \return Returns a writeable reference to the internal Filter.
     */
    Filter&
    getFilter()
    {
        return mFilter;
    }

protected:
    bool
    valid(IDType id, const outpost::Slice<const uint8_t>& buffer) override
    {
        return mFilter(id, buffer);
    }

private:
    Filter mFilter;
};

}  // namespace swb
}  // namespace outpost

#include "software_bus_impl.h"

#endif /* OUTPOST_SWB_SOFTWARE_BUS_H */
