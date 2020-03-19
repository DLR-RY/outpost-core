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
/**
 * The DataProcessorThread is responsible for taking over the workload of transforming and encoding
 * DataBlocks one at a time.
 */
class DataProcessorThread : public outpost::rtos::Thread
{
public:
    /** Constructor
     * @param thread_priority Priority in the OS' scheduler
     * @param pool SharedBufferPool for allocation of new DataBlocks
     * @param inputQueue Queue to listen to for incoming raw DataBlocks
     * @param outputQueue Queue to send encoded DataBlocks to for long-term storage or transmission
     * to ground
     */
    DataProcessorThread(uint8_t thread_priority,
                        outpost::utils::SharedBufferPoolBase& pool,
                        outpost::utils::ReferenceQueueBase<DataBlock>& inputQueue,
                        outpost::utils::ReferenceQueueBase<DataBlock>& outputQueue);

    virtual ~DataProcessorThread();

    /**
     * The method is called by the OS' scheduler. It awaits DataBlocks on the incoming queue,
     * transforms them using the wavelet transform and then encodes the data to a newly allocated
     * DataBlock.
     */
    void
    run();

    /**
     * Enables the processing of DataBlocks
     */
    void
    enable();

    /**
     * Disables the processing of DataBlocks
     */
    void
    disable();

    /**
     * Getter for the number of DataBlocks that have been received from the input queue.
     * @return Returns the number of incoming blocks.
     */
    inline uint32_t
    getNumberOfIncomingBlocks()
    {
        return numIncomingBlocks;
    }

    /**
     * Getter for the number of DataBlocks that have been processed.
     * @return Returns the number of processed blocks.
     */
    inline uint32_t
    getNumberOfProcessedBlocks()
    {
        return numProcessedBlocks;
    }

    /**
     * Getter for the number of DataBlocks that haven been forwarded to the output queue.
     * @return Returns the number of forwarded blocks.
     */
    inline uint32_t
    getNumberOfForwardedBlocks()
    {
        return numForwardedBlocks;
    }

    /**
     * Getter for the thread's state.
     * @return Returns true if processing is currently enabled, false otherwise.
     */
    bool
    isEnabled();

    /**
     * Resets the counters for incoming, processed and forwarded blocks.
     */
    inline void
    resetCounters()
    {
        numIncomingBlocks = 0;
        numProcessedBlocks = 0;
        numForwardedBlocks = 0;
    }

    /**
     * Goes through the entire processing sequence for a single block,
     * from reception from the input queue through compression to forwarding to the output queue.
     * @param timeout Timeout for reception of a DataBlock on the input queue.
     */
    void
    processSingleBlock(outpost::time::Duration timeout = outpost::time::Duration::infinity());

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
