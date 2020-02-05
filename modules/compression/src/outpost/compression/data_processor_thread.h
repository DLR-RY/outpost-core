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

#ifndef OUTPOST_COMPRESSION_DATA_PROCESSOR_THREAD_H_
#define OUTPOST_COMPRESSION_DATA_PROCESSOR_THREAD_H_

#include "data_block.h"
#include "nls_encoder.h"

#include <outpost/rtos/checkpoint.h>
#include <outpost/rtos/thread.h>
#include <outpost/utils/container/reference_queue.h>
#include <outpost/utils/container/shared_object_pool.h>

namespace outpost
{
namespace compression
{
class DataProcessorThread : public outpost::rtos::Thread
{
public:
    DataProcessorThread(uint8_t,
                        outpost::utils::SharedBufferPoolBase&,
                        outpost::utils::ReferenceQueueBase<DataBlock>&,
                        outpost::utils::ReferenceQueueBase<DataBlock>&);

    virtual ~DataProcessorThread();

    void
    run();

    void
    enable();
    void
    disable();

    inline uint32_t
    getNumberOfIncomingBlocks()
    {
        return numIncomingBlocks;
    }

    inline uint32_t
    getNumberOfProcessedBlocks()
    {
        return numProcessedBlocks;
    }

    inline uint32_t
    getNumberOfForwardedBlocks()
    {
        return numForwardedBlocks;
    }

    inline bool
    isEnabled()
    {
        return mCheckpoint.getState() == outpost::rtos::Checkpoint::State::running;
    }

    inline void
    resetCounters()
    {
        numIncomingBlocks = 0;
        numProcessedBlocks = 0;
        numForwardedBlocks = 0;
    }

    void processSingleBlock(outpost::time::Duration = outpost::time::Duration::infinity());

private:
    bool
    compress(DataBlock& b);

    outpost::utils::ReferenceQueueBase<DataBlock>& mInputQueue;
    outpost::utils::ReferenceQueueBase<DataBlock>& mOutputQueue;

    outpost::utils::SharedBufferPoolBase& mPool;

    outpost::rtos::Checkpoint mCheckpoint;

    uint32_t numIncomingBlocks;
    uint32_t numProcessedBlocks;
    uint32_t numForwardedBlocks;

    NLSEncoder mEncoder;

    static constexpr uint16_t maximumEncodingBufferLength = 16400;
    uint8_t mEncodingBuffer[maximumEncodingBufferLength];
    outpost::Slice<uint8_t> mEncodingSlice;
    outpost::Bitstream mBitstream;
};

}  // namespace compression
}  // namespace outpost

#endif /* OUTPOST_COMPRESSION_DATA_PROCESSOR_THREAD_H_ */
