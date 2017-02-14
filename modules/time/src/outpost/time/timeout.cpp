/*
 * Copyright (c) 2016, German Aerospace Center (DLR)
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

#include "timeout.h"

using outpost::time::Timeout;

Timeout::Timeout() :
    mEndtime(),
    mState(stopped)
{
}

Timeout::Timeout(const outpost::time::Clock& clock, outpost::time::Duration time) :
    mEndtime(clock.now() + time),
    mState(armed)
{
}

void
Timeout::restart(const outpost::time::Clock& clock, outpost::time::Duration time)
{
    mEndtime = clock.now() + time;
    mState = armed;
}

Timeout::State
Timeout::getState(const outpost::time::Clock& clock)
{
    if ((mState == armed) && (clock.now() >= mEndtime))
    {
        mState = expired;
    }
    return mState;
}
