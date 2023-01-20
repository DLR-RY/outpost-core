/*
 * Copyright (c) 2013-2022, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2013-2017, Fabian Greif (DLR RY-AVS)
 * - 2022, Felix Passenberg (DLR RY-AVS)
 */

#include "timeout.h"

using outpost::time::Timeout;

Timeout::Timeout() : mStartTime(), mEndTime(), mState(stopped)
{
}

Timeout::Timeout(const outpost::time::Clock& clock, outpost::time::Duration time) :
    mStartTime(clock.now()), mEndTime(mStartTime + time), mState(armed)
{
}

void
Timeout::restart(const outpost::time::Clock& clock, outpost::time::Duration time)
{
    mStartTime = clock.now();
    mEndTime = mStartTime + time;
    mState = armed;
}

void
Timeout::changeDuration(const outpost::time::Clock& clock, outpost::time::Duration time)
{
    getState(clock);
    mEndTime = mStartTime + time;
}

Timeout::State
Timeout::getState(const outpost::time::Clock& clock) const
{
    if ((mState == armed) && (clock.now() >= mEndTime))
    {
        mState = expired;
    }
    return mState;
}
