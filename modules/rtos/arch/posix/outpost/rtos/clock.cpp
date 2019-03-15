/*
 * Copyright (c) 2013-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2013-2017, Fabian Greif (DLR RY-AVS)
 */

#include "clock.h"

#include "internal/time.h"

#include <time.h>

namespace outpost
{
namespace rtos
{
static constexpr int64_t microsecondsPerSecond =
        time::Duration::microsecondsPerMillisecond * time::Duration::millisecondsPerSecond;

outpost::time::SpacecraftElapsedTime
SystemClock::now() const
{
    timespec time = getTime(CLOCK_MONOTONIC);

    // Convert to microseconds. Store seconds in a uint64_t to avoid an overflow when converting
    // to microseconds on 32-bit systems.
    uint64_t secondsInMicroseconds = time.tv_sec;
    secondsInMicroseconds *= microsecondsPerSecond;
    uint64_t nanosecondsInMicroseconds = time.tv_nsec;
    nanosecondsInMicroseconds /= time::Duration::nanosecondsPerMicrosecond;
    uint64_t totalMicroseconds = secondsInMicroseconds + nanosecondsInMicroseconds;

    return outpost::time::SpacecraftElapsedTime::afterEpoch(
            outpost::time::Microseconds(totalMicroseconds));
}

}  // namespace rtos
}  // namespace outpost
