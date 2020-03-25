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
     */
    DataBlockSender(outpost::utils::ReferenceQueueBase<DataBlock>* queue = nullptr);

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

    void
    registerOutputQueue(outpost::utils::ReferenceQueueBase<DataBlock>* queue);

protected:
    outpost::utils::ReferenceQueueBase<DataBlock>* mOutputQueue;
};

/**
 * OneTimeSender tries to send the DataBlock to a ReferenceQueue once and returns the result.
 */
class OneTimeSender : public DataBlockSender
{
public:
    /**
     * Constructor that is handed an outgoing queue.
     * @param queue ReferenceQueue to send DataBlocks to.
     */
    OneTimeSender(outpost::utils::ReferenceQueueBase<DataBlock>* queue = nullptr);
    ~OneTimeSender() = default;

    /**
     * @see DataBlockSender::send
     */
    bool
    send(DataBlock&) override;
};

}  // namespace compression
}  // namespace outpost

#endif /* EXT_OUTPOST_CORE_MODULES_COMPRESSION_SRC_OUTPOST_COMPRESSION_DATA_BLOCK_SENDER_H_ */
