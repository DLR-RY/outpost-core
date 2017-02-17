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
 */
// ----------------------------------------------------------------------------

#include "clock.h"

#include <time.h>

outpost::time::SpacecraftElapsedTime
outpost::rtos::SystemClock::now() const
{
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);

    // convert to microseconds
    uint64_t microseconds = (time.tv_nsec / 1000) + (time.tv_sec * 1000000);

    return outpost::time::SpacecraftElapsedTime::afterEpoch(outpost::time::Microseconds(microseconds));
}
