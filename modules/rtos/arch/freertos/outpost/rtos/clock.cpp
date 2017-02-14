/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 *
 * This file is part of outpost 0.6.
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

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

outpost::time::SpacecraftElapsedTime
outpost::rtos::SystemClock::now() const
{
    // TODO Check when this will overflow
    uint64_t ticks_since_boot = static_cast<uint64_t>(xTaskGetTickCount());

    // convert to microseconds
    uint64_t us = (static_cast<uint64_t>(ticks_since_boot) * 1000000) / configTICK_RATE_HZ;

    return outpost::time::SpacecraftElapsedTime::afterEpoch(outpost::time::Microseconds(us));
}
