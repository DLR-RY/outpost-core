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

#include "nls_encoder.h"

#include <outpost/parameter/support.h>
#include <outpost/rtos/checkpoint.h>
#include <outpost/rtos/thread.h>
#include <outpost/utils/storage/bitstream.h>

namespace outpost
{
namespace utils
{
template <typename T>
class ReferenceQueueBase;

class SharedBufferPoolBase;
}  // namespace utils

namespace compression
{
class DataBlock;

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
                        outpost::support::parameter::HeartbeatSource heartbeatSource,
                        outpost::utils::SharedBufferPoolBase& pool,
                        outpost::utils::ReferenceQueueBase<DataBlock>& inputQueue,
                        outpost::utils::ReferenceQueueBase<DataBlock>& outputQueue,
                        uint8_t numOutputRetries = 5U,
                        outpost::time::Duration retryTimeout = outpost::time::Milliseconds(500));

    virtual ~DataProcessorThread();

    /**
     * The method is called by the OS' scheduler. It awaits DataBlocks on the incoming queue,
     * transforms them using the wavelet transform and then encodes the data to a newly allocated
     * DataBlock.
     */
    void
    run() override;

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
    getNumberOfReceivedBlocks() const
    {
        return mNumIncomingBlocks;
    }

    /**
     * Getter for the number of DataBlocks that have been processed.
     * @return Returns the number of processed blocks.
     */
    inline uint32_t
    getNumberOfProcessedBlocks() const
    {
        return mNumProcessedBlocks;
    }

    /**
     * Getter for the number of DataBlocks that haven been forwarded to the output queue.
     * @return Returns the number of forwarded blocks.
     */
    inline uint32_t
    getNumberOfForwardedBlocks() const
    {
        return mNumForwardedBlocks;
    }

    /**
     * Getter for the number of DataBlocks that have been lost because they could not be sent to
     * the output queue.
     * @return Returns the number of forwarded blocks.
     */
    inline uint32_t
    getNumberOfLostBlocks() const
    {
        return mNumLostBlocks;
    }

    /**
     * Getter for the thread's state.
     * @return Returns true if processing is currently enabled, false otherwise.
     */
    bool
    isEnabled() const;

    /**
     * Resets the counters for incoming, processed and forwarded blocks.
     */
    inline void
    resetCounters()
    {
        mNumIncomingBlocks = 0;
        mNumProcessedBlocks = 0;
        mNumForwardedBlocks = 0;
        mNumLostBlocks = 0;
    }

    /**
     * Goes through the entire processing sequence for a single block,
     * from reception from the input queue through compression to forwarding to the output queue.
     * @param timeout Timeout for reception of a DataBlock on the input queue.
     */
    void
    processSingleBlock(outpost::time::Duration timeout = waitForBlockTimeout);

    static constexpr uint16_t maximumEncodingBufferLength = 16500;

private:
    bool
    compress(DataBlock& b);

    outpost::support::parameter::HeartbeatSource mHeartbeatSource;

    outpost::utils::ReferenceQueueBase<DataBlock>& mInputQueue;
    outpost::utils::ReferenceQueueBase<DataBlock>& mOutputQueue;

    outpost::utils::SharedBufferPoolBase& mPool;

    outpost::rtos::Checkpoint mCheckpoint;

    uint32_t mNumIncomingBlocks;
    uint32_t mNumProcessedBlocks;
    uint32_t mNumForwardedBlocks;
    uint32_t mNumLostBlocks;

    NLSEncoder mEncoder;

    static constexpr outpost::time::Duration waitForBlockTimeout = outpost::time::Seconds(5);
    static constexpr outpost::time::Duration processingTimeout = outpost::time::Seconds(1);

    outpost::time::Duration mRetrySendTimeout;
    uint8_t mMaxSendRetries;
};

}  // namespace compression
}  // namespace outpost

#endif /* OUTPOST_COMPRESSION_DATA_PROCESSOR_THREAD_H_ */
