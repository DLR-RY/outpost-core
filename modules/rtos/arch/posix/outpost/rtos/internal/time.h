/*
 * Copyright (c) 2014-2017, 2019, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2014-2017, 2019, Fabian Greif (DLR RY-AVS)
 */

#ifndef OUTPOST_RTOS_POSIX_TIME_H
#define OUTPOST_RTOS_POSIX_TIME_H

#include <outpost/time/duration.h>

#include <time.h>

namespace outpost
{
namespace rtos
{
/**
 * Convert a duration to a C `timespec` type.
 *
 * A 64 bit for `timespec.tv_sec` is assumed.
 *
 * \warning
 *      For negative durations, `tv_sec` and `tv_nsec`
 *      will be negative.
 */
static inline timespec
toRelativeTime(const time::Duration duration)
{
    // Split into seconds and sub-seconds to avoid an overflow
    // when converting the sub-seconds to nanoseconds.
    uint64_t seconds = duration.seconds();
    auto subSeconds = duration - time::Seconds(seconds);

    // Convert to sub-seconds nanoseconds, this will never exceed a
    // value of 1,000,000,000
    int32_t nanoseconds = subSeconds.microseconds() * time::Duration::nanosecondsPerMicrosecond;

    timespec relativeTime = {
            // seconds
            static_cast<time_t>(seconds),

            // remaining nanoseconds
            static_cast<long int>(nanoseconds),
    };

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
    static constexpr int32_t maximumNanosecondsPerSecond =
            time::Duration::nanosecondsPerMicrosecond * time::Duration::microsecondsPerMillisecond
            * time::Duration::millisecondsPerSecond;

    // get current time
    timespec absoluteTime;
    clock_gettime(CLOCK_REALTIME, &absoluteTime);

    timespec relative = toRelativeTime(duration);

    absoluteTime.tv_nsec += relative.tv_nsec;
    if (absoluteTime.tv_nsec >= maximumNanosecondsPerSecond)
    {
        absoluteTime.tv_sec += 1;
        absoluteTime.tv_nsec -= maximumNanosecondsPerSecond;
    }
    else if (absoluteTime.tv_nsec < 0)
    {
        absoluteTime.tv_sec -= 1;
        absoluteTime.tv_nsec += maximumNanosecondsPerSecond;
    }
    absoluteTime.tv_sec += relative.tv_sec;

    return absoluteTime;
}

}  // namespace rtos
}  // namespace outpost

#endif
