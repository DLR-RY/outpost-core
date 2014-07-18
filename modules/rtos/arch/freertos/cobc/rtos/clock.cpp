/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include "clock.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

cobc::time::TimePoint
cobc::rtos::SystemClock::now()
{
    // TODO Check when this will overflow
    uint64_t ticks_since_boot = static_cast<uint64_t>(xTaskGetTickCount());

    // convert to microseconds
    uint64_t us = (static_cast<uint64_t>(ticks_since_boot) * 1000000) / configTICK_RATE_HZ;

    return cobc::time::TimePoint(us);
}
