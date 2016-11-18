/*
 * Copyright (c) 2016, German Aerospace Center (DLR)
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

#ifndef COBC_TIME_TIMEOUT_H
#define COBC_TIME_TIMEOUT_H

#include <cobc/time/clock.h>
#include <cobc/time/duration.h>

namespace cobc
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
    Timeout(const cobc::time::Clock& clock,
            cobc::time::Duration time);

    void
    restart(const cobc::time::Clock& clock,
            cobc::time::Duration time);

    /**
     * Stop the timeout.
     *
     * A stopped timeout will never expire.
     */
    inline void
    stop()
    {
        mState = stopped;
    }

    State
    getState(const cobc::time::Clock& clock);

    inline bool
    isStopped() const
    {
        return (mState == stopped);
    }

    inline bool
    isExpired(const cobc::time::Clock& clock)
    {
        return (getState(clock) == expired);
    }

    bool
    isArmed(const cobc::time::Clock& clock)
    {
        return (getState(clock) == armed);
    }

private:
    cobc::time::SpacecraftElapsedTime mEndtime;
    State mState;
};

}
}

#endif
