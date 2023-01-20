/*
 * Copyright (c) 2016-2022, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2016-2017, Fabian Greif (DLR RY-AVS)
 * - 2022, Felix Passenberg (DLR RY-AVS)
 */

#ifndef OUTPOST_TIME_TIMEOUT_H
#define OUTPOST_TIME_TIMEOUT_H

#include <outpost/time/clock.h>
#include <outpost/time/duration.h>

namespace outpost
{
namespace time
{
/**
 * Polling based timeout class.
 *
 * Allows to track that a certain amount of time has passed.
 *
 * \author  Fabian Greif
 */
class Timeout
{
public:
    enum State
    {
        stopped,
        armed,
        expired
    };

    /**
     * Create a stopped timeout.
     *
     * The timeout can later be started by calling the `restart` method.
     */
    Timeout();

    /**
     * Create a new timeout and start it immediately.
     */
    Timeout(const outpost::time::Clock& clock, outpost::time::Duration time);

    /**
     * Starts the time with a new EndTime.
     */
    void
    restart(const outpost::time::Clock& clock, outpost::time::Duration time);

    /**
     * Updates a running Timeouts EndTime.
     *
     * Checks if a Timeout expired before updating the EndTime.
     * A stopped timer is not restarted.
     */
    void
    changeDuration(const outpost::time::Clock& clock, outpost::time::Duration time);

    /**
     * Stop the timeout.
     *
     * A stopped timeout will never expire.
     */
    inline void
    stop() const
    {
        mState = stopped;
    }

    State
    getState(const outpost::time::Clock& clock) const;

    inline bool constexpr isStopped() const
    {
        return (mState == stopped);
    }

    inline bool
    isExpired(const outpost::time::Clock& clock) const
    {
        return (getState(clock) == expired);
    }

    bool
    isArmed(const outpost::time::Clock& clock) const
    {
        return (getState(clock) == armed);
    }

private:
    outpost::time::SpacecraftElapsedTime mStartTime;
    outpost::time::SpacecraftElapsedTime mEndTime;
    mutable State mState;
};

}  // namespace time
}  // namespace outpost

#endif
