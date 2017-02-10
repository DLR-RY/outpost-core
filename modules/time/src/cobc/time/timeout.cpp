/*
 * Copyright (c) 2016, German Aerospace Center (DLR)
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

#include "timeout.h"

using cobc::time::Timeout;

Timeout::Timeout() :
    mEndtime(),
    mState(stopped)
{
}

Timeout::Timeout(const cobc::time::Clock& clock, cobc::time::Duration time) :
    mEndtime(clock.now() + time),
    mState(armed)
{
}

void
Timeout::restart(const cobc::time::Clock& clock, cobc::time::Duration time)
{
    mEndtime = clock.now() + time;
    mState = armed;
}

Timeout::State
Timeout::getState(const cobc::time::Clock& clock)
{
    if ((mState == armed) && (clock.now() >= mEndtime))
    {
        mState = expired;
    }
    return mState;
}
