/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include "clock.h"

#include <rtems.h>

#ifdef UNITTEST
static cobc::time::TimePoint currentTime;
#endif

cobc::time::TimePoint
cobc::rtos::Clock::now()
{
#ifdef UNITTEST
    return currentTime;
#else
    rtems_interval ticks_since_boot;
    rtems_interval us_per_tick = rtems_configuration_get_microseconds_per_tick();

    rtems_clock_get(RTEMS_CLOCK_GET_TICKS_SINCE_BOOT, &ticks_since_boot);

    // convert to microseconds
    uint64_t us = ticks_since_boot * us_per_tick;

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
