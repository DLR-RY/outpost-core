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

#include "data_aggregator.h"

namespace outpost
{
namespace compression
{
DataAggregator* DataAggregator::listOfAllDataAggregators = nullptr;

DataAggregator::DataAggregator(uint16_t parameterId,
                               outpost::time::Clock& clock,
                               outpost::utils::SharedBufferPoolBase& pool,
                               DataBlockSender& sender) :
    ImplicitList<DataAggregator>(DataAggregator::listOfAllDataAggregators, this),
    mParameterId(parameterId),
    mSamplingRate(SamplingRate::disabled),
    mNextSamplingRate(SamplingRate::disabled),
    mBlocksize(Blocksize::disabled),
    mNextBlocksize(Blocksize::disabled),
    mBlock(),
    mEnabled(false),
    mDisableAfterCurrentBlock(false),
    mClock(clock),
    mMemoryPool(pool),
    mSender(sender)
{
}

DataAggregator::~DataAggregator()
{
    removeFromList(&DataAggregator::listOfAllDataAggregators, this);
}

DataAggregator*
DataAggregator::findDataAggregator(uint16_t parameterId)
{
    DataAggregator* aggregator = nullptr;
    for (DataAggregator* it = DataAggregator::listOfAllDataAggregators;
         (it != nullptr) && (aggregator == nullptr);
         it = it->getNext())
    {
        if (it->getParameterId() == parameterId)
        {
            aggregator = it;
        }
    }
    return aggregator;
}

uint16_t
DataAggregator::numberOfAggregators()
{
    uint16_t n = 0;
    for (DataAggregator* it = DataAggregator::listOfAllDataAggregators; it != nullptr;
         it = it->getNext())
    {
        n++;
    }
    return n;
}

bool
DataAggregator::push(Fixpoint fp)
{
    bool res = false;
    if (isEnabled())
    {
        if (!mBlock.isValid())
        {
            outpost::utils::SharedBufferPointer p;
            if (mMemoryPool.allocate(p))
            {
                mSamplingRate = mNextSamplingRate;
                mBlocksize = mNextBlocksize;
                mBlock = {p,
                          mParameterId,
                          outpost::time::TimeEpochConverter<
                                  outpost::time::SpacecraftElapsedTimeEpoch,
                                  outpost::time::GpsEpoch>::convert(mClock.now()),
                          mSamplingRate,
                          mBlocksize};
            }
        }

        if (mBlock.isValid())
        {
            res = mBlock.push(fp);

            if (mBlock.isComplete())
            {
                mSender.send(mBlock);
                mBlock = {};
                if (mDisableAfterCurrentBlock)
                {
                    disable();
                }
            }
        }
    }
    return res;
}

bool
DataAggregator::isBlockValid() const
{
    return mBlock.isValid();
}

void
DataAggregator::enable(SamplingRate sr, Blocksize bs)
{
    setNextBlocksize(bs);
    setNextSamplingRate(sr);
    mEnabled = true;
    mDisableAfterCurrentBlock = false;
}

void
DataAggregator::enableForOneBlock(SamplingRate sr, Blocksize bs)
{
    setNextBlocksize(bs);
    setNextSamplingRate(sr);
    mEnabled = true;
    mDisableAfterCurrentBlock = true;
}

void
DataAggregator::disable()
{
    mEnabled = false;
    mBlock = DataBlock{};
}

void
DataAggregator::registerOutputQueue(outpost::utils::ReferenceQueueBase<DataBlock>* queue)
{
    mSender.registerOutputQueue(queue);
}

}  // namespace compression
}  // namespace outpost
