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

#include "data_processor_thread.h"

#include "legall_wavelet.h"

#include <outpost/base/fixpoint.h>

namespace outpost
{
namespace compression
{
DataProcessorThread::DataProcessorThread(
        uint8_t thread_priority,
        outpost::utils::SharedBufferPoolBase& pool,
        outpost::utils::ReferenceQueueBase<DataBlock>& inputQueue,
        outpost::utils::ReferenceQueueBase<DataBlock>& outputQueue) :
    outpost::rtos::Thread(thread_priority, 1024, "DPT"),
    mInputQueue(inputQueue),
    mOutputQueue(outputQueue),
    mPool(pool),
    mCheckpoint(outpost::rtos::Checkpoint::State::suspending),
    numIncomingBlocks(0),
    numProcessedBlocks(0),
    numForwardedBlocks(0),
    mEncodingSlice(mEncodingBuffer),
    mBitstream(mEncodingSlice)
{
}

DataProcessorThread::~DataProcessorThread()
{
}

void
DataProcessorThread::run()
{
    while (1)
    {
        mCheckpoint.pass();
        processSingleBlock();
    }
}

void
DataProcessorThread::enable()
{
    mCheckpoint.resume();
}

void
DataProcessorThread::disable()
{
    mCheckpoint.suspend();
}

bool
DataProcessorThread::isEnabled()
{
    return mCheckpoint.getState() == outpost::rtos::Checkpoint::State::running;
}

void
DataProcessorThread::processSingleBlock(outpost::time::Duration timeout)
{
    DataBlock b;
    if (mInputQueue.receive(b, timeout))
    {
        numIncomingBlocks++;
        if (compress(b))
        {
            numProcessedBlocks++;
            if (mOutputQueue.send(b))
            {
                numForwardedBlocks++;
            }
        }
    }
}

bool
DataProcessorThread::compress(DataBlock& b)
{
    b.applyWaveletTransform();
    if (b.getCoefficients().getNumberOfElements() > 0U)
    {
        outpost::utils::SharedBufferPointer p;
        if (mPool.allocate(p))
        {
            DataBlock outputBlock(
                    p, b.getParameterId(), b.getStartTime(), b.getSamplingRate(), b.getBlocksize());
            if (b.encode(outputBlock, mEncoder))
            {
                b = outputBlock;
                return true;
            }
        }
    }
    return false;
}

}  // namespace compression
}  // namespace outpost
