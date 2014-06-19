/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include "clock.h"

#ifdef UNITTEST
static cobc::time::TimePoint currentTime;
#endif

cobc::time::TimePoint
cobc::rtos::Clock::now()
{
#ifdef UNITTEST
    return currentTime;
#else
    // convert to microseconds
    uint64_t us = 0;

    return cobc::time::TimePoint(us);
#endif
}

#ifdef UNITTEST
void
cobc::rtos::TestingClock::setTime(time::TimePoint time)
{
    currentTime = time;
}
#endif
