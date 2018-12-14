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

#ifndef HEARTBEAT_LIMITER_H_
#define HEARTBEAT_LIMITER_H_

#include "heartbeat.h"

#include <outpost/parameter/support.h>
#include <outpost/time/clock.h>
#include <outpost/time/duration.h>
#include <outpost/time/time_epoch.h>

namespace outpost
{
namespace support
{
/**
 * Helper class to avoid sending heartbeats too often.
 *
 * Limits the number of heartbeat signals send to the topic to one
 * per heartbeat interval. If the execution timeout changes a heart beat might
 * be send earlier.
 */
class HeartbeatLimiter
{
public:
    inline HeartbeatLimiter(outpost::time::Clock& clock,
                            const outpost::time::Duration heartbeatInterval,
                            const outpost::support::parameter::HeartbeatSource source) :
        mClock(clock),
        mHeartbeatInterval(heartbeatInterval),
        mSource(source),
        mTimeout(time::SpacecraftElapsedTime::startOfEpoch())
    {
    }

    /**
     * Send a heartbeat signal (if required)
     *
     * \param   executionTimeout
     *      Maximum time allocated to finish the operation
     */
    void
    send(outpost::time::Duration executionTimeout);

private:
    outpost::time::Clock& mClock;

    /// A new heartbeat will only be generated after this duration
    const outpost::time::Duration mHeartbeatInterval;
    const outpost::support::parameter::HeartbeatSource mSource;

    /// Time at which the heartbeat will expire
    outpost::time::SpacecraftElapsedTime mTimeout;
};

}  // namespace support
}  // namespace outpost

#endif
