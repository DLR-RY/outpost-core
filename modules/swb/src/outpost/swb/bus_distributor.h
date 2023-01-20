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
 * - 2021, Jan Malburg (DLR RY-AVS)
 */

#ifndef OUTPOST_SWB_BUS_DISTRIBUTOR_H_
#define OUTPOST_SWB_BUS_DISTRIBUTOR_H_

#include "bus_channel.h"
#include "bus_handler_thread.h"
#include "message_handler.h"
#include "types.h"

#include <outpost/rtos/mutex.h>
#include <outpost/utils/container/shared_buffer.h>

#include <stdint.h>

namespace outpost
{
namespace utils
{
template <typename T>
class Receiver;
}  // namespace utils

namespace swb
{
/**
 * The BusDistributor holds the underlying thread that handles passing around of messages as well as
 * a list if registered channels. It is not meant to be used directly but through the SoftwareBus by
 * inheritance.
 * \param IDType type of the message ID
 * \ingroup swb
 */
template <typename IDType>
class BusDistributor : public MessageHandler
{
    friend class BusHandlerThread<IDType>;

public:
    /**
     * Constructor.
     * \param queue Receiver that is passed to the internal BusHandlerThread. All Message will be
     *        passed to this queue for processing.
     * \param priority Priority of the internal
     *        BusHandlerThread
     * \param heartbeatSource HeartbeatSource for the internal BusHandlerThread to
     *        feed OUTPOST's the software watchdog.
     */
    BusDistributor(outpost::utils::Receiver<Message<IDType>>& queue,
                   uint8_t priority,
                   outpost::support::parameter::HeartbeatSource heartbeatSource);

    /**
     * Default destructor.
     */
    virtual ~BusDistributor() = default;

    /**
     * Starts the internal BusHandlerThread
     */
    void
    start();

    /**
     * Registers a BusChannel that will receive Message.
     * \param channel The BusChannel to be registered.
     */
    OperationResult
    registerChannel(BusChannel<IDType>& channel);

    /**
     * Removes a bus channel
     * Warning: Function is racey. Function for unit-testing only, don't use in production code.
     *          Some unit-tests require this function for correct tear down.
     *
     * \param channel The BusChannel to be unregistered.
     */
    OperationResult
    unregisterChannel(BusChannel<IDType>& channel);

    /**
     * Sets the default channel, i.e. the channel that gets all
     * messages not matched by any other channel. Can only set
     * one default channel, function fails if already one is set
     *
     * \param channel the default channel
     * \return success if everything is correct
     *         maxChannelsReached: if already on default channel is set
     */
    OperationResult
    setDefaultChannel(BusChannel<IDType>& channel);

    /**
     * Getter for the current number of BusChannel.
     * \return Number of BusChannel.
     */
    inline size_t
    getNumberOfChannels() const
    {
        return mChannels.size();
    }

    /**
     * Getter for the number of handled Message.
     * \return Number of Message handled by the internal BusHandlerThread.
     */
    inline uint32_t
    getNumberOfHandledMessages() const
    {
        return mHandlerThread.getNumberOfIncomingMessages();
    }

    /**
     * Getter for the number of Messages forwarded to regular bus channels (not the default bus
     * channel). *Note:* This does not match the sum of Message received by all registered
     * BusChannel, because every Message is counted only once. \return Number of Message forwarded
     * to at least one BusChannel
     */
    inline uint32_t
    getNumberOfForwardedMessages() const
    {
        return mHandlerThread.getNumberOfForwardedMessages();
    }

    /**
     * Getter for the number of Message send to the default channel.
     *
     * \return Number of Message forwarded to the default
     *         BusChannel
     */
    inline uint32_t
    getNumberOfDefaultedMessages() const
    {
        return mHandlerThread.getNumberOfDefaulteddMessages();
    }

protected:
    /**
     * Getter for the internal List of BusChannel.
     * \return List of BusChannel.
     */
    inline outpost::List<BusChannel<IDType>>&
    getChannels()
    {
        return mChannels;
    }

    void
    handleAllMessages() override;

    bool
    handleSingleMessage() override;

private:
    BusHandlerThread<IDType> mHandlerThread;

    outpost::utils::Receiver<Message<IDType>>& mReceiver;
    outpost::List<BusChannel<IDType>> mChannels;
    BusChannel<IDType>* mDefaultChannel;
    outpost::rtos::Mutex mChannelMutex;
};

}  // namespace swb
}  // namespace outpost

#include "bus_distributor_impl.h"

#endif
