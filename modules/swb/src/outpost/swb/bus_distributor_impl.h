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

#ifndef OUTPOST_SWB_BUS_DISTRIBUTOR_IMPL_H_
#define OUTPOST_SWB_BUS_DISTRIBUTOR_IMPL_H_

#include "bus_distributor.h"

#include <outpost/rtos/mutex_guard.h>
#include <outpost/utils/communicator.h>

namespace outpost
{
namespace swb
{
template <typename IDType>
BusDistributor<IDType>::BusDistributor(
        outpost::utils::Receiver<Message<IDType>>& receiver,
        uint8_t priority,
        outpost::support::parameter::HeartbeatSource heartbeatSource) :
    mHandlerThread(*this, receiver, heartbeatSource, priority),
    mReceiver(receiver),
    mDefaultChannel(nullptr)
{
}

template <typename IDType>
void
BusDistributor<IDType>::start()
{
    mHandlerThread.start();
}

template <typename IDType>
OperationResult
BusDistributor<IDType>::registerChannel(BusChannel<IDType>& channel)
{
    outpost::rtos::MutexGuard lock(mChannelMutex);
    mChannels.append(&channel);
    return OperationResult::success;
}

template <typename IDType>
OperationResult
BusDistributor<IDType>::unregisterChannel(BusChannel<IDType>& channel)
{
    outpost::rtos::MutexGuard lock(mChannelMutex);
    if (mChannels.removeNode(&channel))
    {
        return OperationResult::success;
    }
    else
    {
        return OperationResult::notFound;
    }
}

template <typename IDType>
OperationResult
BusDistributor<IDType>::setDefaultChannel(BusChannel<IDType>& channel)
{
    outpost::rtos::MutexGuard lock(mChannelMutex);
    if (nullptr == mDefaultChannel)
    {
        mDefaultChannel = &channel;
        return OperationResult::success;
    }
    else
    {
        return OperationResult::maxChannelsReached;
    }
}

template <typename IDType>
void
BusDistributor<IDType>::handleAllMessages()
{
    while (mHandlerThread.step(outpost::time::Duration::zero()))
    {
        // all done in the condition
    }
}

template <typename IDType>
bool
BusDistributor<IDType>::handleSingleMessage()
{
    return mHandlerThread.step(outpost::time::Duration::zero());
}

}  // namespace swb
}  // namespace outpost

#endif
