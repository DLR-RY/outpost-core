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

namespace outpost
{
namespace compression
{
template <class Sender>
DataAggregator<Sender>::DataAggregator(uint16_t parameterId,
                                       SamplingRate sr,
                                       Blocksize bs,
                                       outpost::utils::SharedBufferPoolBase& memoryPool,
                                       outpost::utils::ReferenceQueueBase<DataBlock>& outputQueue,
                                       outpost::time::Clock& clock) :
    DataAggregatorBase(parameterId, sr, bs),
    mMemoryPool(memoryPool),
    mOutputQueue(outputQueue),
    mClock(clock)
{
}

template <class Sender>
DataAggregator<Sender>::~DataAggregator()
{
}

template <class Sender>
bool
DataAggregator<Sender>::push(Fixpoint fp)
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
                res = Sender::send(mBlock, mOutputQueue);
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

}  // namespace compression
}  // namespace outpost
