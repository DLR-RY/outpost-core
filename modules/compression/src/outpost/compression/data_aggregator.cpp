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
DataAggregatorBase* DataAggregatorBase::listOfAllDataAggregators = nullptr;

DataAggregatorBase::DataAggregatorBase(uint16_t parameterId, SamplingRate sr, Blocksize bs) :
    ImplicitList<DataAggregatorBase>(DataAggregatorBase::listOfAllDataAggregators, this),
    mParameterId(parameterId),
    mSamplingRate(sr),
    mNextSamplingRate(sr),
    mBlocksize(bs),
    mNextBlocksize(bs),
    mBlock(),
    mEnabled(false),
    mDisableAfterCurrentBlock(false)
{
}

DataAggregatorBase::~DataAggregatorBase()
{
    removeFromList(&DataAggregatorBase::listOfAllDataAggregators, this);
}

DataAggregatorBase*
DataAggregatorBase::findDataAggregator(uint16_t parameterId)
{
    DataAggregatorBase* aggregator = nullptr;
    for (DataAggregatorBase* it = DataAggregatorBase::listOfAllDataAggregators;
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
DataAggregatorBase::numberOfAggregators()
{
    uint16_t n = 0;
    for (DataAggregatorBase* it = DataAggregatorBase::listOfAllDataAggregators; it != nullptr;
         it = it->getNext())
    {
        n++;
    }
    return n;
}

}  // namespace compression
}  // namespace outpost
