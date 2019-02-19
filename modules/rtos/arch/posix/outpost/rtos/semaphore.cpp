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
Semaphore::Semaphore(uint32_t count) : sid()
{
    // shared semaphores are disabled
    if (sem_init(&sid, 0, count) != 0)
    {
        FailureHandler::fatal(FailureCode::resourceAllocationFailed(Resource::semaphore));
    }
}

Semaphore::~Semaphore()
{
    sem_destroy(&sid);
}

bool
Semaphore::acquire(time::Duration timeout)
{
    timespec t = toAbsoluteTime(timeout);
    return (sem_timedwait(&sid, &t) == 0);
}

// ----------------------------------------------------------------------------
BinarySemaphore::BinarySemaphore(State::Type initial) : value(initial)
{
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&signal, NULL);
}

BinarySemaphore::~BinarySemaphore()
{
    pthread_cond_destroy(&signal);
    pthread_mutex_destroy(&mutex);
}

bool
BinarySemaphore::acquire()
{
    pthread_mutex_lock(&mutex);
    while (value == State::acquired)
    {
        pthread_cond_wait(&signal, &mutex);
    }
    value = State::acquired;
    pthread_mutex_unlock(&mutex);

    return true;
}

bool
BinarySemaphore::acquire(time::Duration timeout)
{
    timespec time = toAbsoluteTime(timeout);

    pthread_mutex_lock(&mutex);
    while (value == State::acquired)
    {
        if (pthread_cond_timedwait(&signal, &mutex, &time) != 0)
        {
            // Timeout or other error has occurred
            // => semaphore can't be acquired
            pthread_mutex_unlock(&mutex);
            return false;
        }
    }
    value = State::acquired;
    pthread_mutex_unlock(&mutex);

    return true;
}

void
BinarySemaphore::release()
{
    pthread_mutex_lock(&mutex);
    value = State::released;
    pthread_cond_signal(&signal);
    pthread_mutex_unlock(&mutex);
}
