/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include "clock.h"

cobc::time::TimePoint
cobc::rtos::SystemClock::now()
{
    // convert to microseconds
    uint64_t us = 0;

    return cobc::time::TimePoint(us);
}
