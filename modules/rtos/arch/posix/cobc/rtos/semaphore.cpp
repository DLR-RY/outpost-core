/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
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

#include "semaphore.h"

#include <time.h>
#include <cobc/rtos/failure_handler.h>

#include "internal/time.h"

// ----------------------------------------------------------------------------
cobc::rtos::Semaphore::Semaphore(uint32_t count) : sid()
{
    // shared semaphores are disabled
    if (sem_init(&sid, 0, count) != 0)
    {
        FailureHandler::fatal(FailureCode::resourceAllocationFailed(Resource::semaphore));
    }
}

cobc::rtos::Semaphore::~Semaphore()
{
    sem_destroy(&sid);
}

bool
cobc::rtos::Semaphore::acquire(time::Duration timeout)
{
    timespec t = toAbsoluteTime(timeout);
    return (sem_timedwait(&sid, &t) == 0);
}

// ----------------------------------------------------------------------------
cobc::rtos::BinarySemaphore::BinarySemaphore(State::Type initial) :
    value(initial)
{
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&signal, NULL);
}

cobc::rtos::BinarySemaphore::~BinarySemaphore()
{
    pthread_cond_destroy(&signal);
    pthread_mutex_destroy(&mutex);
}

bool
cobc::rtos::BinarySemaphore::acquire()
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
cobc::rtos::BinarySemaphore::acquire(time::Duration timeout)
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
cobc::rtos::BinarySemaphore::release()
{
    pthread_mutex_lock(&mutex);
    value = State::released;
    pthread_cond_signal(&signal);
    pthread_mutex_unlock(&mutex);
}
