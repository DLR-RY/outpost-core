/*
 * Copyright (c) 2015, 2017-2018 German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2015, 2017-2018, Fabian Greif (DLR RY-AVS)
 */

#ifndef OUTPOST_TIME_QUOTA_IMPL_H
#define OUTPOST_TIME_QUOTA_IMPL_H

#include "quota.h"

namespace outpost
{
namespace time
{

template <size_t Resources>
ContinuousIntervalQuota<Resources>::ContinuousIntervalQuota(outpost::time::Duration interval) :
    mInterval(interval),
    mCurrentIndex(0)
{
    reset();
}

template <size_t Resources>
void
ContinuousIntervalQuota<Resources>::setTimeInterval(outpost::time::Duration interval)
{
    mInterval = interval;
}

template <size_t Resources>
bool
ContinuousIntervalQuota<Resources>::access(outpost::time::SpacecraftElapsedTime now)
{
    size_t next = ++mCurrentIndex;
    if (next >= Resources)
    {
        next = 0;
    }

    bool accessGranted = false;
    outpost::time::SpacecraftElapsedTime t = mResources[next];
    if (now >= t + mInterval)
    {
        mResources[next] = now;
        mCurrentIndex = next;
        accessGranted = true;
    }

    return accessGranted;
}

template <size_t Resources>
void
ContinuousIntervalQuota<Resources>::reset()
{
    mCurrentIndex = 0;
    for (size_t i = 0; i < Resources; ++i)
    {
        mResources[i] = SpacecraftElapsedTime::startOfEpoch() - mInterval;
    }
}

}
}

#endif
