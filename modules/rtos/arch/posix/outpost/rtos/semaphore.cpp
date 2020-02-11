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

#include "semaphore.h"

#include "internal/time.h"

#include <outpost/rtos/failure_handler.h>

#include <time.h>

using outpost::rtos::BinarySemaphore;
using outpost::rtos::Semaphore;

// ----------------------------------------------------------------------------
Semaphore::Semaphore(uint32_t count) : mSid()
{
    // shared semaphores are disabled
    if (sem_init(&mSid, 0, count) != 0)
    {
        FailureHandler::fatal(FailureCode::resourceAllocationFailed(Resource::semaphore));
    }
}

Semaphore::~Semaphore()
{
    sem_destroy(&mSid);
}

bool
Semaphore::acquire(time::Duration timeout)
{
    bool success = false;
    if (timeout == time::Duration::infinity())
    {
        success = acquire();
    }
    else
    {
        timespec t = toAbsoluteTime(CLOCK_REALTIME, timeout);
        success = (sem_timedwait(&mSid, &t) == 0);
    }
    return success;
}

// ----------------------------------------------------------------------------
BinarySemaphore::BinarySemaphore() : mValue(BinarySemaphore::State::released)
{
    pthread_mutex_init(&mMutex, NULL);
    pthread_cond_init(&mSignal, NULL);
}

BinarySemaphore::BinarySemaphore(State::Type initial) : mValue(initial)
{
    pthread_mutex_init(&mMutex, NULL);
    pthread_cond_init(&mSignal, NULL);
}

BinarySemaphore::~BinarySemaphore()
{
    pthread_cond_destroy(&mSignal);
    pthread_mutex_destroy(&mMutex);
}

bool
BinarySemaphore::acquire()
{
    pthread_mutex_lock(&mMutex);
    while (mValue == State::acquired)
    {
        pthread_cond_wait(&mSignal, &mMutex);
    }
    mValue = State::acquired;
    pthread_mutex_unlock(&mMutex);

    return true;
}

bool
BinarySemaphore::acquire(time::Duration timeout)
{
    bool success = false;
    if (timeout == time::Duration::infinity())
    {
        success = acquire();
    }
    else
    {
        timespec time = toAbsoluteTime(CLOCK_REALTIME, timeout);
        pthread_mutex_lock(&mMutex);
        while (mValue == State::acquired)
        {
            if (pthread_cond_timedwait(&mSignal, &mMutex, &time) != 0)
            {
                // Timeout or other error has occurred
                // => semaphore can't be acquired
                pthread_mutex_unlock(&mMutex);
                return false;
            }
        }
        mValue = State::acquired;
        pthread_mutex_unlock(&mMutex);

        success = true;
    }
    return success;
}

void
BinarySemaphore::release()
{
    pthread_mutex_lock(&mMutex);
    mValue = State::released;
    pthread_cond_signal(&mSignal);
    pthread_mutex_unlock(&mMutex);
}
