/*
 * Copyright (c) 2016-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2016-2017, Fabian Greif (DLR RY-AVS)
 */

#ifndef OUTPOST_RTOS_RTEMS_INTERVAL_H
#define OUTPOST_RTOS_RTEMS_INTERVAL_H

#include <rtems.h>

#include <outpost/time/duration.h>

namespace outpost
{
namespace rtos
{
namespace rtems
{
/**
 * Converts a duration into a RTEMS interval.
 *
 * An RTEMS interval needs to be at least 1 tick long, otherwise it would
 * specify an infinite interval.
 */
static inline rtems_interval
getInterval(outpost::time::Duration duration)
{
    rtems_interval interval;
    if (duration >= outpost::time::Duration::myriad())
    {
        interval = 0;
    }
    else if (duration < outpost::time::Duration::zero())
    {
        // Negative intervals are invalid and are rounded up to the
        // smallest positive interval.
        interval = 1;
    }
    else
    {
        interval = duration.microseconds() / rtems_configuration_get_microseconds_per_tick();
        if (interval < 1)
        {
            interval = 1;
        }
    }
    return interval;
}

}  // namespace rtems
}  // namespace rtos
}  // namespace outpost

#endif
