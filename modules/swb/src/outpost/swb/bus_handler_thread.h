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

#ifndef OUTPOST_SWB_BUS_HANDLER_THREAD_H_
#define OUTPOST_SWB_BUS_HANDLER_THREAD_H_

#include <outpost/parameter/support.h>
#include <outpost/rtos/thread.h>

namespace outpost
{
namespace utils
{
class SharedBufferPointer;

template <typename T>
class Receiver;
}  // namespace utils

namespace swb
{
/**
 * \ingroup    swb
 */
template <typename IDType>
class BusDistributor;

template <typename IDType>
struct Message;

/**
 * The BusHandlerThread handles incoming Messages and distributes them to registered BusChannel.
 * \ingroup    swb
 */
template <typename IDType>
class BusHandlerThread : public outpost::rtos::Thread
{
public:
    friend class BusDistributor<IDType>;

    /**
     * Constructor.
     * \param bus BusDistributor for which the BusHandlerThread is responsible.
     * \param receiver Receiver that holds incoming Message
     * \param heartbeat HeartbeatSource for the software watchdog.
     * \param priority Priority of the Thread
     * \param stackSize Stack size of the Thread, defaults to Thread::defaultStackSize
     */
    BusHandlerThread(BusDistributor<IDType>& bus,
                     outpost::utils::Receiver<Message<IDType>>& receiver,
                     outpost::support::parameter::HeartbeatSource heartbeat,
                     uint8_t priority,
                     size_t stackSize = outpost::rtos::Thread::defaultStackSize);

    /**
     * Destructor.
     */
    virtual ~BusHandlerThread() = default;

    /**
     * Performs a single step of processing for a single Message.
     * \param timeout Timeout for a single step after which the software watchdog is fed.
     * \return true if a message was handled, false if timeout occurred
     */
    bool
    step(outpost::time::Duration timeout = RECEIVE_TIMEOUT);

    /**
     * Getter for the total number of incoming Message.
     * \return Number of incoming Message through the Receiver
     */
    inline uint32_t
    getNumberOfIncomingMessages() const
    {
        return mNumberOfIncomingMessages;
    }

    /**
     * Getter for the number of Message that have been forwarded to at least one regular BusChannel.
     * \return Number of forwarded Message
     */
    inline uint32_t
    getNumberOfForwardedMessages() const
    {
        return mNumberOfForwardedMessages;
    }

    /**
     * Getter for the number of Messages that have been send to the default channel.
     * \return Number of Message send to the default channel
     */
    inline uint32_t
    getNumberOfDefaulteddMessages() const
    {
        return mNumberOfDefaultedMessages;
    }

protected:
    /**
     * Thread implementation that is called by the superclass.
     */
    void
    run() override;

    static constexpr outpost::time::Duration RECEIVE_TIMEOUT = outpost::time::Seconds(5);

    BusDistributor<IDType>& mBus;
    outpost::utils::Receiver<Message<IDType>>& mReceiver;

    outpost::support::parameter::HeartbeatSource mHeartbeatSource;

    uint32_t mNumberOfIncomingMessages;
    uint32_t mNumberOfForwardedMessages;
    uint32_t mNumberOfDefaultedMessages;
};

}  // namespace swb
}  // namespace outpost

#include "bus_handler_thread_impl.h"

#endif /* OUTPOST_SWB_BUS_HANDLER_THREAD_H_ */
