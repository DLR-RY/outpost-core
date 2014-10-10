/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.3-pre.
 *
 * It is distributed under the terms of the GNU General Public License with
 * a linking exception. See the file "LICENSE" for the full license governing
 * this code.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
// ----------------------------------------------------------------------------

#ifndef COBC_RTOS_POSIX_TIME_H
#define COBC_RTOS_POSIX_TIME_H

#include <time.h>

#include <cobc/time/duration.h>

namespace cobc
{
namespace rtos
{

static inline timespec
toRelativeTime(const time::Duration duration)
{
    uint64_t nanoseconds = duration.microseconds() * 1000;

    timespec relativeTime = {
        // seconds
        static_cast<time_t>(nanoseconds / 1000000000),

        // remaining nanoseconds
        static_cast<long int>(nanoseconds % 1000000000)
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

}
}

#endif // COBC_RTOS_POSIX_TIME_H
