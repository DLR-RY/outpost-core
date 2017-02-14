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

#include "checkpoint.h"

using outpost::rtos::Checkpoint;

Checkpoint::Checkpoint(State initialState) :
    mMutex(),
    mSemaphore((initialState == running) ? BinarySemaphore::State::released
                                         : BinarySemaphore::State::acquired),
    mState((initialState == running) ? running : suspending)
{
}

Checkpoint::~Checkpoint()
{
}

void
Checkpoint::resume()
{
    mMutex.acquire();
    if (mState != running)
    {
        mState = running;
        mSemaphore.release();
    }
    mMutex.release();
}

void
Checkpoint::suspend()
{
    mMutex.acquire();
    if (mState == running)
    {
        mState = suspending;
        mSemaphore.acquire();
    }
    mMutex.release();
}

Checkpoint::State
Checkpoint::getState() const
{
    MutexGuard lock(mMutex);
    return mState;
}

bool
Checkpoint::shouldSuspend() const
{
    MutexGuard lock(mMutex);
    return (mState == suspending);
}

void
Checkpoint::pass()
{
    bool waiting = true;
    do
    {
        mMutex.acquire();
        if (mState == running)
        {
            mMutex.release();
            waiting = false;
        }
        else
        {
            mState = suspended;
            mMutex.release();

            mSemaphore.acquire();
            mSemaphore.release();
        }
    } while (waiting);
}
