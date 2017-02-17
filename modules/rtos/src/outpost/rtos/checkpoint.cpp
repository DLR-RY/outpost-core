/*
 * Copyright (c) 2016-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2016-2017, Fabian Greif (DLR RY-AVS)
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
