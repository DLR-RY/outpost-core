/*
 * Copyright (c) 2013-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2013-2017, Fabian Greif (DLR RY-AVS)
 * - 2013, Norbert Toth (DLR RY-AVS)
 */

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
