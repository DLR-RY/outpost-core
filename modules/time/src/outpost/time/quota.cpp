/*
 * Copyright (c) 2018 German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2018, Fabian Greif (DLR RY-AVS)
 */

#include "quota.h"

namespace outpost
{
namespace time
{

UnlimitedQuota DefaultQuota;

NonDeterministicIntervalQuota::NonDeterministicIntervalQuota(
        outpost::time::Duration interval,
        size_t numberOfResources) :
    mInterval(interval),
    mIntervalEnd(outpost::time::SpacecraftElapsedTime::startOfEpoch()),
    mNumberOfResources(numberOfResources)
{
}

void
NonDeterministicIntervalQuota::setTimeInterval(outpost::time::Duration interval)
{
    mInterval = interval;
}

bool
NonDeterministicIntervalQuota::access(outpost::time::SpacecraftElapsedTime now)
{
    if (mIntervalEnd < now)
    {
        mUsedResources = 0;
        mIntervalEnd = now + mInterval;
    }

    bool granted = false;
    if (mUsedResources < mNumberOfResources)
    {
        mUsedResources++;
        granted = true;
    }
    return granted;
}

void
NonDeterministicIntervalQuota::reset()
{
    mIntervalEnd = outpost::time::SpacecraftElapsedTime::startOfEpoch();
}

}
}
