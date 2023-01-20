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

#ifndef OUTPOST_SWB_BUS_HANDLER_THREAD_IMP_H_
#define OUTPOST_SWB_BUS_HANDLER_THREAD_IMP_H_

#include "bus_distributor.h"
#include "bus_handler_thread.h"

#include <outpost/support/heartbeat.h>
#include <outpost/swb/bus_channel.h>
#include <outpost/utils/communicator.h>

namespace outpost
{
namespace swb
{
template <typename IDType>
constexpr outpost::time::Duration BusHandlerThread<IDType>::RECEIVE_TIMEOUT;

template <typename IDType>
BusHandlerThread<IDType>::BusHandlerThread(BusDistributor<IDType>& bus,
                                           outpost::utils::Receiver<Message<IDType>>& receiver,
                                           outpost::support::parameter::HeartbeatSource heartbeat,
                                           uint8_t priority,
                                           size_t stackSize) :
    outpost::rtos::Thread(priority, stackSize, "SWB"),
    mBus(bus),
    mReceiver(receiver),
    mHeartbeatSource(heartbeat),
    mNumberOfIncomingMessages(0U),
    mNumberOfForwardedMessages(0U),
    mNumberOfDefaultedMessages(0U)
{
}

template <typename IDType>
void
BusHandlerThread<IDType>::run()
{
    while (true)
    {
        outpost::support::Heartbeat::send(mHeartbeatSource, RECEIVE_TIMEOUT * 2U);
        step();
    }
}

template <typename IDType>
bool
BusHandlerThread<IDType>::step(outpost::time::Duration timeout)
{
    Message<IDType> message;
    if (mReceiver.receive(message, timeout))
    {
        mNumberOfIncomingMessages++;
        auto channelIterator = mBus.getChannels().begin();
        bool isForwarded = false;
        while (channelIterator != mBus.getChannels().end())
        {
            if (OperationResult::success == channelIterator->sendMessage(message))
            {
                isForwarded = true;
            }
            ++channelIterator;
        }
        if (isForwarded)
        {
            mNumberOfForwardedMessages++;
        }
        else
        {
            if (mBus.mDefaultChannel != nullptr
                && (OperationResult::success == mBus.mDefaultChannel->sendMessage(message)))
            {
                mNumberOfDefaultedMessages++;
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}

}  // namespace swb
}  // namespace outpost

#endif
