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

#ifndef OUTPOST_COMPRESSION_DATA_AGGREGATOR_H_
#define OUTPOST_COMPRESSION_DATA_AGGREGATOR_H_

#include <outpost/base/fixpoint.h>
#include <outpost/compression/data_block.h>
#include <outpost/time/clock.h>
#include <outpost/utils/container/implicit_list.h>
#include <outpost/utils/container/reference_queue.h>
#include <outpost/utils/container/shared_object_pool.h>

namespace outpost
{
namespace compression
{
class DataAggregatorBase : public ImplicitList<DataAggregatorBase>
{
public:
    DataAggregatorBase(uint16_t parameterId, SamplingRate sr, Blocksize bs);
    virtual ~DataAggregatorBase();

    virtual bool
    push(Fixpoint fp) = 0;

    inline uint16_t
    getParameterId()
    {
        return mParameterId;
    }

    bool
    isBlockEmpty()
    {
        return !mBlock.isValid();
    }

    inline void
    setSamplingRate(SamplingRate sr)
    {
        mNextSamplingRate = sr;
    }

    inline bool
    hasSamplingRateChanged()
    {
        return mSamplingRate != mNextSamplingRate;
    }

    inline SamplingRate
    getSamplingRate()
    {
        return mSamplingRate;
    }

    inline SamplingRate
    getNextSamplingRate()
    {
        return mNextSamplingRate;
    }

    inline void
    setBlocksize(Blocksize bs)
    {
        mNextBlocksize = bs;
    }

    inline bool
    hasBlocksizeChanged()
    {
        return mBlocksize != mNextBlocksize;
    }

    inline Blocksize
    getBlocksize()
    {
        return mBlocksize;
    }

    inline Blocksize
    getNextBlocksize()
    {
        return mNextBlocksize;
    }

    inline bool
    isEnabled()
    {
        return mEnabled;
    }

    inline void
    enable()
    {
        mEnabled = true;
        mDisableAfterCurrentBlock = false;
    }

    inline void
    enableForOneBlock()
    {
        mEnabled = true;
        mDisableAfterCurrentBlock = true;
    }

    inline void
    disable()
    {
        mEnabled = false;
    }

    inline void
    disableAfterCurrentBlock()
    {
        mDisableAfterCurrentBlock = true;
    }

    static DataAggregatorBase*
    findDataAggregator(uint16_t paramId);

    static uint16_t
    numberOfAggregators();

protected:
    static DataAggregatorBase* listOfAllDataAggregators;

    uint16_t mParameterId;
    SamplingRate mSamplingRate;
    SamplingRate mNextSamplingRate;

    Blocksize mBlocksize;
    Blocksize mNextBlocksize;

    DataBlock mBlock;

    bool mEnabled;
    bool mDisableAfterCurrentBlock;
};

template <class Sender>
class DataAggregator : public DataAggregatorBase
{
public:
    DataAggregator(uint16_t parameterId,
                   SamplingRate sr,
                   Blocksize bs,
                   outpost::utils::SharedBufferPoolBase& memoryPool,
                   outpost::utils::ReferenceQueueBase<DataBlock>& outputQueue,
                   outpost::time::Clock& clock);
    virtual ~DataAggregator();

    virtual bool
    push(Fixpoint fp) override;

private:
    outpost::utils::SharedBufferPoolBase& mMemoryPool;
    outpost::utils::ReferenceQueueBase<DataBlock>& mOutputQueue;
    outpost::time::Clock& mClock;
};

}  // namespace compression
}  // namespace outpost

#include "data_aggregator_impl.h"

#endif /* OUTPOST_COMPRESSION_DATA_AGGREGATOR_H_ */
