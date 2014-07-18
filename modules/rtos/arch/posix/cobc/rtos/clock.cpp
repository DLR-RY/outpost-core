/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include "clock.h"

#include <time.h>

cobc::time::TimePoint
cobc::rtos::SystemClock::now() const
{
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);

    // convert to microseconds
    uint64_t microseconds = (time.tv_nsec / 1000) + (time.tv_sec * 1000000);

    return cobc::time::TimePoint(microseconds);
}
