/*
 * Copyright (c) 2014-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2014-2017, Fabian Greif (DLR RY-AVS)
 */

#ifndef OUTPOST_RTOS_POSIX_TIME_H
#define OUTPOST_RTOS_POSIX_TIME_H

#include <outpost/time/duration.h>

#include <time.h>

namespace outpost
{
namespace rtos
{
static inline timespec
toRelativeTime(const time::Duration duration)
{
    uint64_t nanoseconds = duration.microseconds() * 1000;

    timespec relativeTime = {// seconds
                             static_cast<time_t>(nanoseconds / 1000000000),

                             // remaining nanoseconds
                             static_cast<long int>(nanoseconds % 1000000000)};

    return relativeTime;
}

/**
 * Convert a duration to an absolute time point.
 *
 * Uses the current time as a reference point for the given duration.
 */
static inline timespec
toAbsoluteTime(const time::Duration duration)
{
    uint64_t nanoseconds = duration.microseconds() * 1000;

    // get current time
    timespec absoluteTime;
    clock_gettime(CLOCK_REALTIME, &absoluteTime);

    absoluteTime.tv_nsec += static_cast<time_t>(nanoseconds % 1000000000);
    if (absoluteTime.tv_nsec >= 1000000000)
    {
        absoluteTime.tv_sec += 1;
        absoluteTime.tv_nsec = absoluteTime.tv_nsec - 1000000000;
    }
    absoluteTime.tv_sec += static_cast<long int>(nanoseconds / 1000000000);

    return absoluteTime;
}

}  // namespace rtos
}  // namespace outpost

#endif
