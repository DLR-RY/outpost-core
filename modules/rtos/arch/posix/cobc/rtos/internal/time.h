/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
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
