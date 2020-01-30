/*
 * Copyright (c) 2020, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2020, Jan Malburg (DLR RY-AVS)
 */

#include "barrier.h"

outpost::rtos::Barrier::Barrier(uint32_t numberOfThreads) :
    mWaitSemaphore(::outpost::rtos::BinarySemaphore::State::acquired),
    mReleaseSemaphore(::outpost::rtos::BinarySemaphore::State::released),
    mMaxWaiting(numberOfThreads),
    mCurrentlyWaiting(0){};

void
outpost::rtos::Barrier::wait()
{
    // To ensure that on does not start waiting while the rest is not done releasing.
    mReleaseSemaphore.acquire();

    mCounterMutex.acquire();
    uint32_t myNumber = ++mCurrentlyWaiting;
    mCounterMutex.release();

    if (myNumber < mMaxWaiting)
    {
        mReleaseSemaphore.release();

        // wait till we start releasing
        mWaitSemaphore.acquire();

        // release the next one
        mWaitSemaphore.release();

        mCounterMutex.acquire();
        myNumber = --mCurrentlyWaiting;
        mCounterMutex.release();

        if (myNumber == 0)
        {
            // you are the last
            // get the Semaphore values to initial value
            mWaitSemaphore.acquire();
            mReleaseSemaphore.release();
        }

        return;
    }
    else
    {
        // release the next one
        mWaitSemaphore.release();

        mCounterMutex.acquire();
        myNumber = --mCurrentlyWaiting;
        mCounterMutex.release();

        // for the special case maxWaiting == 1
        if (myNumber == 0)
        {
            // you are the last
            // get the Semaphore values to initial value
            mWaitSemaphore.acquire();
            mReleaseSemaphore.release();
        }

        return;
    }
}
