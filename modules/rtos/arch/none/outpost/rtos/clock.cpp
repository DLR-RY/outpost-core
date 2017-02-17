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

outpost::time::SpacecraftElapsedTime
outpost::rtos::SystemClock::now() const
{
    // convert to microseconds
    uint64_t us = 0;

    return outpost::time::SpacecraftElapsedTime::afterEpoch(outpost::time::Microseconds(us));
}
