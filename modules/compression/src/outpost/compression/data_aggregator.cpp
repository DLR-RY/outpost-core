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

#include "data_block_sender.h"

#include <outpost/base/fixpoint.h>
#include <outpost/time/clock.h>
#include <outpost/utils/container/shared_object_pool.h>

namespace outpost
{
namespace compression
{
DataAggregator* DataAggregator::listOfAllDataAggregators = nullptr;

DataAggregator::DataAggregator() :
    ImplicitList<DataAggregator>(DataAggregator::listOfAllDataAggregators, this),
    mParameterId(0),
    mSamplingRate(SamplingRate::disabled),
    mNextSamplingRate(SamplingRate::disabled),
    mBlocksize(Blocksize::disabled),
    mNextBlocksize(Blocksize::disabled),
    mBlock(),
    mEnabled(false),
    mDisableAfterCurrentBlock(false),
    mMemoryPool(nullptr),
    mSender(nullptr),
    mNumCompletedBlocks(0),
    mNumLostBlocks(0),
    mNumLostSamples(0),
    mNumOverallSamples(0)
{
}

DataAggregator::DataAggregator(uint16_t paramId,
                               outpost::utils::SharedBufferPoolBase& pool,
                               DataBlockSender& sender) :
    ImplicitList<DataAggregator>(DataAggregator::listOfAllDataAggregators, this),
    mParameterId(paramId),
    mSamplingRate(SamplingRate::disabled),
    mNextSamplingRate(SamplingRate::disabled),
    mBlocksize(Blocksize::disabled),
    mNextBlocksize(Blocksize::disabled),
    mBlock(),
    mEnabled(false),
    mDisableAfterCurrentBlock(false),
    mMemoryPool(&pool),
    mSender(&sender),
    mNumCompletedBlocks(0),
    mNumLostBlocks(0),
    mNumLostSamples(0),
    mNumOverallSamples(0)
{
}

bool
DataAggregator::initialize(uint16_t paramId,
                           outpost::utils::SharedBufferPoolBase& pool,
                           DataBlockSender& sender)
{
    if (findDataAggregator(paramId) == nullptr)
    {
        mParameterId = paramId;
        mMemoryPool = &pool;
        mSender = &sender;
        return true;
    }
    else
    {
        return false;
    }
}

DataAggregator::~DataAggregator()
{
    removeFromList(&DataAggregator::listOfAllDataAggregators, this);
}

DataAggregator*
DataAggregator::findDataAggregator(uint16_t paramId)
{
    DataAggregator* aggregator = nullptr;
    for (DataAggregator* it = DataAggregator::listOfAllDataAggregators;
         (nullptr != it) && (nullptr == aggregator);
         it = it->getNext())
    {
        if (it->getParameterId() == paramId)
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
DataAggregator::push(Fixpoint fp, const outpost::time::GpsTime& currentTime)
{
    bool res = false;
    if (isEnabled())
    {
        if (!mBlock.isValid())
        {
            outpost::utils::SharedBufferPointer p;
            if (mMemoryPool->allocate(p))
            {
                mSamplingRate = mNextSamplingRate;
                mBlocksize = mNextBlocksize;
                mBlock = {p, mParameterId, currentTime, mSamplingRate, mBlocksize};
            }
        }

        if (mBlock.push(fp))
        {
            res = true;
            mNumOverallSamples++;

            if (mBlock.isComplete())
            {
                mNumCompletedBlocks++;
                if (!mSender->send(mBlock))
                {
                    mNumLostBlocks++;
                }
                if (mDisableAfterCurrentBlock)
                {
                    disable();
                }

                mBlock = {};
            }
        }
        else
        {
            mNumLostSamples++;
        }
    }
    return res;
}

bool
DataAggregator::isAtStartOfNewBlock() const
{
    return mBlock.isValid();
}

bool
DataAggregator::enable(SamplingRate sr, Blocksize bs)
{
    if (mMemoryPool != nullptr && mSender != nullptr)
    {
        setNextBlocksize(bs);
        setNextSamplingRate(sr);
        mEnabled = true;
        mDisableAfterCurrentBlock = false;
        return true;
    }
    else
    {
        return false;
    }
}

bool
DataAggregator::enableForOneBlock(SamplingRate sr, Blocksize bs)
{
    if (mMemoryPool != nullptr && mSender != nullptr)
    {
        setNextBlocksize(bs);
        setNextSamplingRate(sr);
        mEnabled = true;
        mDisableAfterCurrentBlock = true;
        return true;
    }
    else
    {
        return false;
    }
}

void
DataAggregator::disable()
{
    mEnabled = false;
    mBlock = DataBlock{};
}

}  // namespace compression
}  // namespace outpost
