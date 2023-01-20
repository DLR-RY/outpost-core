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

#ifndef OUTPOST_SWB_SOFTWARE_BUS_IMPL_H
#define OUTPOST_SWB_SOFTWARE_BUS_IMPL_H

#include "software_bus.h"

#include <outpost/utils/container/reference_queue.h>
#include <outpost/utils/container/shared_object_pool.h>

namespace outpost
{
namespace swb
{
template <typename IDType>
SoftwareBus<IDType>::SoftwareBus(outpost::utils::SharedBufferPoolBase& pool,
                                 outpost::utils::ReferenceQueueBase<Message<IDType>>& queue,
                                 uint8_t priority,
                                 outpost::support::parameter::HeartbeatSource heartbeatSource) :
    BusDistributor<IDType>(queue, priority, heartbeatSource),
    mPool(pool),
    mQueue(queue),
    mNumberOfInvalidMessages(0U),
    mNumberOfFailedCopyOperations(0U),
    mNumberOfFailedSendOperations(0U),
    mNumberOfAcceptedMessages(0U)
{
}

template <typename IDType>
OperationResult
SoftwareBus<IDType>::sendMessage(const IDType id, const outpost::Slice<const uint8_t>& data)
{
    outpost::rtos::MutexGuard lock(mMutex);
    if (!valid(id, data))
    {
        mNumberOfInvalidMessages++;
        return OperationResult::invalidMessage;
    }
    outpost::utils::ConstSharedBufferPointer buffer;
    OperationResult res = copySliceToBuffer(data, buffer);
    if (OperationResult::success == res)
    {
        return sendMessage(id, buffer, CopyMode::zero_copy);
    }
    else
    {
        mNumberOfFailedCopyOperations++;
    }
    return res;
}

template <typename IDType>
OperationResult
SoftwareBus<IDType>::sendMessage(const IDType id,
                                 const outpost::utils::ConstSharedBufferPointer& buffer,
                                 CopyMode mode)
{
    outpost::rtos::MutexGuard lock(mMutex);
    if (!valid(id, buffer.asSlice()))
    {
        mNumberOfInvalidMessages++;
        return OperationResult::invalidMessage;
    }

    outpost::utils::ConstSharedBufferPointer tmpBuffer;
    if (CopyMode::copy_once == mode)
    {
        OperationResult res = copySliceToBuffer(buffer.asSlice(), tmpBuffer);
        if (OperationResult::success != res)
        {
            mNumberOfFailedCopyOperations++;
            return res;
        }
    }
    else
    {
        tmpBuffer = buffer;
    }

    Message<IDType> msg = {id, tmpBuffer};
    return sendMessage(msg);
}

template <typename IDType>
OperationResult
SoftwareBus<IDType>::sendMessage(Message<IDType>& msg)
{
    outpost::rtos::MutexGuard lock(mMutex);
    if (!valid(msg.id, msg.buffer.asSlice()))
    {
        mNumberOfInvalidMessages++;
        return OperationResult::invalidMessage;
    }

    if (mQueue.send(msg))
    {
        mNumberOfAcceptedMessages++;
        return OperationResult::success;
    }
    else
    {
        mNumberOfFailedSendOperations++;
        return OperationResult::sendFailed;
    }
}

template <typename IDType>
OperationResult
SoftwareBus<IDType>::copySliceToBuffer(const outpost::Slice<const uint8_t>& slice,
                                       outpost::utils::ConstSharedBufferPointer& buffer)
{
    if (slice.getNumberOfElements() == 0)
    {
        outpost::utils::SharedBufferPointer tmp;
        buffer = tmp;
        return OperationResult::success;
    }
    else if (slice.getNumberOfElements() <= mPool.getElementSize())
    {
        outpost::utils::SharedBufferPointer tmp;
        if (mPool.allocate(tmp))
        {
            tmp.asSlice().copyFrom(slice);
            outpost::utils::SharedChildPointer c;
            tmp.getChild(c, 0, 0, slice.getNumberOfElements());
            buffer = c;
            return OperationResult::success;
        }
        return OperationResult::noBufferAvailable;
    }
    return OperationResult::messageTooLong;
}

}  // namespace swb
}  // namespace outpost

#endif
