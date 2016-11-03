/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.5.
 *
 * It is distributed under the terms of the GNU General Public License with a
 * linking exception. See the file "LICENSE" for the full license governing
 * this code.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 */
// ----------------------------------------------------------------------------

#include "clock.h"

#include <rtems.h>

cobc::time::SpacecraftElapsedTime
cobc::rtos::SystemClock::now() const
{
    rtems_interval us_per_tick = rtems_configuration_get_microseconds_per_tick();
    rtems_interval ticks_since_boot = rtems_clock_get_ticks_since_boot();

    // convert to microseconds
    uint64_t us = static_cast<uint64_t>(ticks_since_boot) *
                  static_cast<uint64_t>(us_per_tick);

    return cobc::time::SpacecraftElapsedTime::afterEpoch(cobc::time::Microseconds(us));
}
