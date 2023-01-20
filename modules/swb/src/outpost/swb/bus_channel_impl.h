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

#ifndef OUTPOST_SWB_BUS_CHANNEL_IMPL_H_
#define OUTPOST_SWB_BUS_CHANNEL_IMPL_H_

#include "bus_channel.h"

#include <outpost/utils/container/shared_buffer.h>

namespace outpost
{
namespace swb
{
template <typename IDType>
BusChannel<IDType>::BusChannel() :
    mNumIncomingMessages(0),
    mNumAppendedMessages(0),
    mNumFailedReceptions(0),
    mNumRetrievedMessages(0)
{
}

template <typename IDType>
BufferedBusChannel<IDType>::BufferedBusChannel(outpost::Slice<Message<IDType>> buffer) :
    mBuffer(buffer), mMessageAvailable(0U)
{
}

template <typename IDType>
OperationResult
BufferedBusChannel<IDType>::sendMessage(const Message<IDType>& m)
{
    OperationResult res = OperationResult::sendFailed;
    BusChannel<IDType>::mNumIncomingMessages++;

    if (matches(m))
    {
        outpost::rtos::MutexGuard lock(mMutex);
        if (mBuffer.append(m))
        {
            BusChannel<IDType>::mNumAppendedMessages++;
            mMessageAvailable.release();
            res = OperationResult::success;
        }
        else
        {
            BusChannel<IDType>::mNumFailedReceptions++;
            res = OperationResult::noBufferAvailable;
        }
    }
    return res;
}

template <typename IDType>
OperationResult
BufferedBusChannel<IDType>::receiveMessage(Message<IDType>& m, outpost::time::Duration timeout)
{
    OperationResult res = OperationResult::success;
    if (mMessageAvailable.acquire(timeout))
    {
        outpost::rtos::MutexGuard lock(mMutex);
        if (!mBuffer.isEmpty())
        {
            m = mBuffer.getFront();
            // required to release the underlying buffer because outpost::Deque<T> will not do so
            // automatically for the Message type.
            mBuffer.getFront() = Message<IDType>();
            mBuffer.removeFront();
            BusChannel<IDType>::mNumRetrievedMessages++;
        }
        else
        {
            res = OperationResult::invalidState;
        }
    }
    else
    {
        res = OperationResult::noMessageAvailable;
    }
    return res;
}

template <typename IDType>
size_t
BufferedBusChannel<IDType>::getCurrentNumberOfMessages()
{
    outpost::rtos::MutexGuard lock(mMutex);
    return mBuffer.getSize();
}

}  // namespace swb
}  // namespace outpost

#endif
