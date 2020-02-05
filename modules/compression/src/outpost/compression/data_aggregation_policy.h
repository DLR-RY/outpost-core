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

#include <outpost/compression/data_aggregator.h>
#include <outpost/compression/data_block.h>

namespace outpost
{
namespace compression
{
namespace data_aggregation_policy
{
class TryOnce
{
    friend class outpost::compression::DataAggregator<TryOnce>;
    static bool
    send(DataBlock& b, outpost::utils::ReferenceQueueBase<DataBlock>& q)
    {
        return q.send(b);
    }
};

template <uint8_t Tries, uint16_t MS>
class RetryTimeout
{
    friend class outpost::compression::DataAggregator<RetryTimeout>;
    static bool
    send(DataBlock&, outpost::utils::ReferenceQueueBase<DataBlock>&)
    {
        return false;
    }
};

template <uint16_t MS>
class RetryForever
{
    friend class outpost::compression::DataAggregator<RetryForever>;
    static bool
    send(DataBlock&, outpost::utils::ReferenceQueueBase<DataBlock>&)
    {
        return false;
    }
};

}  // namespace data_aggregation_policy
}  // namespace compression
}  // namespace outpost
