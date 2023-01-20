/*
 * Copyright (c) 2020-2021, German Aerospace Center (DLR)
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

#ifndef OUTPOST_SWB_BUS_CHANNEL_H_
#define OUTPOST_SWB_BUS_CHANNEL_H_

#include <outpost/base/slice.h>
#include <outpost/rtos/mutex.h>
#include <outpost/rtos/semaphore.h>
#include <outpost/swb/default_message_filter.h>
#include <outpost/swb/types.h>
#include <outpost/utils/container/deque.h>
#include <outpost/utils/container/implicit_list.h>
#include <outpost/utils/container/list.h>

#include <type_traits>

namespace outpost
{
namespace utils
{
class SharedBufferPointer;
}
namespace swb
{
/**
 * The BusChannel interface is used to receive messages from the SoftwareBus. A SoftwareBus will
 * distribute messages to registered BusChannels according to their filtering.
 * \ingroup swb
 * \param IDType type of the message ID
 */
template <typename IDType>
class BusChannel : public outpost::ListElement
{
public:
    /**
     * Default constructor.
     */
    BusChannel();

    /**
     * Default destructor
     */
    virtual ~BusChannel() = default;

    /**
     * This method is supposed to be used by a SoftwareBus instance only.
     * \param m Message to be sent to the channel.
     * \return OperationResult according to a successful send operation.
     */
    virtual OperationResult
    sendMessage(const Message<IDType>& m) = 0;

    /**
     * Retrieves a single Message from the channel within a given timeout.
     * \param m Reference of the received Message
     * \param timeout Maximum time to wait for a successful reception.
     * \return OperationResult according to successful reception.
     */
    virtual OperationResult
    receiveMessage(Message<IDType>& m, outpost::time::Duration timeout) = 0;

    /**
     * Checks whether a given Message matches the BusChannel's filter.
     * \param m Message to be checked
     * \return Returns true if the message matches the BusChannel, false otherwise.
     */
    virtual bool
    matches(const Message<IDType>& m) const = 0;

    /**
     * Getter for the number of incoming Message to the BusChannel. This includes Messages that
     * will be filtered out.
     * \return Number of incoming Messages.
     */
    inline uint32_t
    getNumberOfIncomingMessages() const
    {
        return mNumIncomingMessages;
    }

    /**
     * Getter for the number of Message that were successfully appended to the internal queue.
     * \return Number of accepted messages.
     */
    inline uint32_t
    getNumberOfAppendedMessages() const
    {
        return mNumAppendedMessages;
    }

    /**
     * Getter for the number of failed receptions by the BusChannel, indicating that there is not
     * enough space in the queue.
     * \return Number of Message that could not be appended to the
     * internal queue.
     */
    inline uint32_t
    getNumberOfFailedReceptions() const
    {
        return mNumFailedReceptions;
    }

    /**
     * Getter for the number of Message that were retrieved from the channel.
     * \return Number of retrieved Message.
     */
    inline uint32_t
    getNumberOfRetrievedMessages() const
    {
        return mNumRetrievedMessages;
    }

protected:
    uint32_t mNumIncomingMessages;
    uint32_t mNumAppendedMessages;
    uint32_t mNumFailedReceptions;

    uint32_t mNumRetrievedMessages;
};

template <typename IDType, typename Filter = FilterNone<IDType>>
class FilteredBusChannel : public virtual BusChannel<IDType>
{
public:
    FilteredBusChannel() = default;

    /**
     * Allows for setting the internal Filter through a writeable reference.
     * \return Returns a reference to the filter. Used to setup the filter, therefore writeable.
     */
    virtual Filter&
    getFilter() = 0;
};

/**
 * \ingroup    swb
 */
template <typename IDType>
class BufferedBusChannel : public virtual BusChannel<IDType>
{
public:
    explicit BufferedBusChannel(outpost::Slice<Message<IDType>> buffer);

    virtual ~BufferedBusChannel() = default;

    virtual OperationResult
    sendMessage(const Message<IDType>& m) override;

    virtual OperationResult
    receiveMessage(Message<IDType>& m,
                   outpost::time::Duration timeout = outpost::time::Duration::zero()) override;

    /**
     * Getter for the number of Message currently waiting for retrieval in the internal queue.
     * \return Current number of enqueued Message.
     */
    size_t
    getCurrentNumberOfMessages();

    using BusChannel<IDType>::matches;

protected:
    outpost::Deque<Message<IDType>> mBuffer;

    outpost::rtos::Mutex mMutex;
    outpost::rtos::Semaphore mMessageAvailable;
};

template <size_t N, typename IDType>
class BusChannelMemory
{
public:
    BusChannelMemory()
    {
        mMemory.fill(Message<IDType>{IDType(), utils::ConstSharedBufferPointer()});
    }

protected:
    std::array<Message<IDType>, N> mMemory;
};

/**
 * \ingroup swb
 * \param N number of messages this channel can buffer
 * \param IDType type of the message ID
 * \param Filter Filter to decide whether the channel wants a Message
 *               requires a function bool operator()(const Message<IDType>&) which returns true
 *               if the BusChannel accepts the Message. Also needs default constructor
 */
template <size_t N, typename IDType, typename Filter = FilterNone<IDType>>
class BufferedBusChannelWithMemory : private BusChannelMemory<N, IDType>,
                                     public BufferedBusChannel<IDType>,
                                     public FilteredBusChannel<IDType, Filter>
{
public:
    BufferedBusChannelWithMemory() :
        BusChannelMemory<N, IDType>(),
        BufferedBusChannel<IDType>(outpost::asSlice(BusChannelMemory<N, IDType>::mMemory)),
        FilteredBusChannel<IDType, Filter>()
    {
        static_assert(std::is_convertible<Filter*, outpost::swb::MessageFilter<IDType>*>::value,
                      "Filter must be a MessageFilter");
        static_assert(std::is_default_constructible<Filter>::value,
                      "Filter must be default constructible");
    }

    virtual ~BufferedBusChannelWithMemory() = default;

    bool
    matches(const Message<IDType>& m) const override
    {
        return mFilter(m);
    }

    Filter&
    getFilter() override
    {
        return mFilter;
    }

private:
    Filter mFilter;
};

}  // namespace swb
}  // namespace outpost

#include "bus_channel_impl.h"

#endif /* OUTPOST_SWB_BUS_CHANNEL_H_ */
