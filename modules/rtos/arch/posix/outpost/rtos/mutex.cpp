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
 * - 2015, Jan Sommer (DLR SC-SRV)
 */

#include "mutex.h"

#include "internal/time.h"

#include <outpost/rtos/failure_handler.h>

#include <time.h>

using namespace outpost::rtos;

Mutex::Mutex()
{
    pthread_mutexattr_t attr;
    if (pthread_mutexattr_init(&attr) != 0)
    {
        FailureHandler::fatal(FailureCode::resourceAllocationFailed(Resource::mutex));
    }

    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&mMutex, &attr);

    if (pthread_mutexattr_destroy(&attr) != 0)
    {
        FailureHandler::fatal(FailureCode::resourceAllocationFailed(Resource::mutex));
    }
}

bool
Mutex::acquire(outpost::time::Duration timeout)
{
    bool success = false;
    if (timeout == time::Duration::infinity())
    {
        success = acquire();
    }
    else
    {
        timespec time = toAbsoluteTime(CLOCK_REALTIME, timeout);
        success = (pthread_mutex_timedlock(&mMutex, &time) == 0);
    }
    return success;
}
