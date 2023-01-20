/*
 * Copyright (c) 2021, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2021, Jan Malburg (DLR RY-AVS)
 */

#ifndef UNITTEST_HAL_WATCHDOG_STUB_H_
#define UNITTEST_HAL_WATCHDOG_STUB_H_

#include <outpost/hal/watchdog.h>
#include <outpost/time/clock.h>

#include <stdlib.h>

namespace unittest
{
namespace hal
{
template <size_t intervalInSeconds>
class WatchdogStub : public outpost::hal::Watchdog
{
public:
    static constexpr outpost::time::Duration interval = outpost::time::Seconds(intervalInSeconds);

    explicit WatchdogStub(outpost::time::Clock& clock) :
        mClock(clock),
        mLastReset(clock.now()),
        mFailed(false)

                {};

    bool
    reset() override
    {
        if (mClock.now() >= mLastReset + interval)
        {
            mFailed = true;
        }
        mLastReset = mClock.now();
        return true;
    }

    outpost::time::Duration
    getInterval() override
    {
        return interval;
    }

    bool
    hasFailed() const
    {
        return mFailed || mClock.now() >= mLastReset + interval;
    }

private:
    outpost::time::Clock& mClock;
    outpost::time::SpacecraftElapsedTime mLastReset;
    bool mFailed;
};

template <size_t intervalInSeconds>
constexpr outpost::time::Duration WatchdogStub<intervalInSeconds>::interval;

}  // namespace hal
}  // namespace unittest

#endif
