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

#ifndef OUTPOST_COMPRESSION_DATA_BLOCK_SENDER_H_
#define OUTPOST_COMPRESSION_DATA_BLOCK_SENDER_H_

namespace outpost
{
namespace utils
{
template <typename T>
class ReferenceQueueBase;
}

namespace compression
{
class DataBlock;

/**
 * In order to anticipate different transmission mechanisms (queues, smpc, etc.) and policies (i.e.
 * optmizing for safety, delay, jitter, etc.) for DataBlocks, the abstract DataBlockSender allows
 * for implementing these through generalization. This, in terms, enables dynamic switching of
 * policies during runtime if desired.
 */
class DataBlockSender
{
public:
    /**
     * Default constructor
     * @param queue Output queue to send the DataBlocks to.
     */
    DataBlockSender() = default;

    /**
     * Default destructor
     */
    virtual ~DataBlockSender() = default;

    /**
     * Sends a DataBlock.
     * @param block The DataBlock to send
     * @return Returns true if successful, false otherwise.
     */
    virtual bool
    send(DataBlock& block) = 0;
};

/**
 * OneTimeSender tries to send the DataBlock to a ReferenceQueue once and returns the result.
 */
class OneTimeQueueSender : public DataBlockSender
{
public:
    /**
     * Constructor that is handed an outgoing queue.
     * @param queue ReferenceQueue to send DataBlocks to.
     */
    explicit OneTimeQueueSender(outpost::utils::ReferenceQueueBase<DataBlock>& queue);
    ~OneTimeQueueSender() = default;

    /**
     * @see DataBlockSender::send
     */
    bool
    send(DataBlock&) override;

protected:
    outpost::utils::ReferenceQueueBase<DataBlock>& mOutputQueue;
};

}  // namespace compression
}  // namespace outpost

#endif /* EXT_OUTPOST_CORE_MODULES_COMPRESSION_SRC_OUTPOST_COMPRESSION_DATA_BLOCK_SENDER_H_ */
