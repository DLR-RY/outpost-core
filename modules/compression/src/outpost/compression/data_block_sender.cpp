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
 * 2020, Jan-Gerd Mess (DLR RY-AVS)
 */

#include "data_block_sender.h"

#include <outpost/utils/container/reference_queue.h>

namespace outpost
{
namespace compression
{
DataBlockSender::DataBlockSender(outpost::utils::ReferenceQueueBase<DataBlock>* queue) :
    mOutputQueue(queue)
{
}

void
DataBlockSender::registerOutputQueue(utils::ReferenceQueueBase<DataBlock>* queue)
{
    mOutputQueue = queue;
}

OneTimeSender::OneTimeSender(utils::ReferenceQueueBase<DataBlock>* queue) : DataBlockSender(queue)
{
}

bool
OneTimeSender::send(DataBlock& b)
{
    if (mOutputQueue != nullptr)
    {
        return mOutputQueue->send(b);
    }
    return false;
}

}  // namespace compression
}  // namespace outpost
