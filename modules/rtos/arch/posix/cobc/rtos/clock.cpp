/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.6.
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

#include <time.h>

cobc::time::SpacecraftElapsedTime
cobc::rtos::SystemClock::now() const
{
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);

    // convert to microseconds
    uint64_t microseconds = (time.tv_nsec / 1000) + (time.tv_sec * 1000000);

    return cobc::time::SpacecraftElapsedTime::afterEpoch(cobc::time::Microseconds(microseconds));
}
