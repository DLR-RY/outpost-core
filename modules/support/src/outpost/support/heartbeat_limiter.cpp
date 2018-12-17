/*
 * Copyright (c) 2018, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2018, Fabian Greif (DLR RY-AVS)
 */

#include "heartbeat_limiter.h"

#include <outpost/parameter/support.h>

namespace outpost
{
namespace support
{
void
HeartbeatLimiter::send(outpost::time::Duration processingTimeout)
{
    outpost::time::SpacecraftElapsedTime currentTime = mClock.now();
    outpost::time::Duration timeout =
            mHeartbeatInterval + processingTimeout + parameter::heartbeatTolerance;

    // Send a new heartbeat if either the heartbeat interval has been reached
    // or if the execution timeout has been significantly reduced.
    if ((mTimeout <= (currentTime + processingTimeout + parameter::heartbeatTolerance))
        || (mTimeout > (currentTime + timeout)))
    {
        mTimeout = currentTime + timeout;
        Heartbeat::send(mSource, timeout);
    }
}

}  // namespace support
}  // namespace outpost
