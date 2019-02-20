/*
 * Copyright (c) 2019, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2019, Fabian Greif (DLR RY-AVS)
 */

#include "time.h"

#include <outpost/rtos/failure_handler.h>

namespace outpost
{
namespace rtos
{
static constexpr int32_t nanosecondsPerSecond = time::Duration::nanosecondsPerMicrosecond
                                                * time::Duration::microsecondsPerMillisecond
                                                * time::Duration::millisecondsPerSecond;

timespec
getTime()
{
    timespec time;
    int result = clock_gettime(CLOCK_MONOTONIC, &time);
    if (result != 0)
    {
        FailureHandler::fatal(FailureCode::genericRuntimeError(Resource::clock));
    }
    return time;
}

timespec
toRelativeTime(time::Duration duration)
{
    // Split into seconds and sub-seconds to avoid an overflow
    // when converting the sub-seconds to nanoseconds.
    int64_t seconds = duration.seconds();
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

timespec
toAbsoluteTime(time::Duration duration)
{
    timespec absoluteTime = getTime();
    timespec relative = toRelativeTime(duration);

    addTime(absoluteTime, relative);

    return absoluteTime;
}

void
addTime(timespec& result, const timespec& increment)
{
    result.tv_nsec += increment.tv_nsec;
    if (result.tv_nsec >= nanosecondsPerSecond)
    {
        result.tv_sec += 1;
        result.tv_nsec -= nanosecondsPerSecond;
    }
    else if (result.tv_nsec < 0)
    {
        result.tv_sec -= 1;
        result.tv_nsec += nanosecondsPerSecond;
    }
    result.tv_sec += increment.tv_sec;
}

}  // namespace rtos
}  // namespace outpost
