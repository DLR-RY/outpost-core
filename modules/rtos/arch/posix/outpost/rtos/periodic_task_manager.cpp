/*
 * Copyright (c) 2014-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2014-2017, Fabian Greif (DLR RY-AVS)
 */

#include "periodic_task_manager.h"

#include "internal/time.h"

#include <outpost/rtos/failure_handler.h>
#include <outpost/rtos/mutex_guard.h>

#include <errno.h>

using namespace outpost::rtos;

PeriodicTaskManager::PeriodicTaskManager() : mMutex(), mTimerRunning(false), mNextWakeTime()
{
}

PeriodicTaskManager::Status::Type
PeriodicTaskManager::nextPeriod(time::Duration period)
{
    MutexGuard lock(mMutex);
    Status::Type currentStatus = Status::running;

    if (mTimerRunning)
    {
        timespec currentTime = getTime();

        // Check if the time is in the current period
        if (isBigger(currentTime, mNextWakeTime))
        {
            currentStatus = Status::timeout;
        }

        int result;
        do
        {
            result = clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &mNextWakeTime, NULL);

            // EINTR is returned when the sleep is interrupted by a signal
            // handler. In this case the sleep can be restarted.
            //
            // Any other result unequal zero is an failure which can not be
            // resolved here and therefore triggers the fatal error handler.
            if (result != 0 && result != EINTR)
            {
                FailureHandler::fatal(FailureCode::genericRuntimeError());
            }
        } while (result != 0);
    }
    else
    {
        // period is started now, no need to wait
        mNextWakeTime = getTime();
        mTimerRunning = true;
    }

    // calculate the next wake-up time
    timespec relative = toRelativeTime(period);
    addTime(mNextWakeTime, relative);

    return currentStatus;
}

PeriodicTaskManager::Status::Type
PeriodicTaskManager::status()
{
    MutexGuard lock(mMutex);
    if (!mTimerRunning)
    {
        return Status::idle;
    }
    else
    {
        timespec currentTime = getTime();

        // Check if the time is in the current period
        if (isBigger(currentTime, mNextWakeTime))
        {
            return Status::timeout;
        }
        else
        {
            return Status::running;
        }
    }
}

void
PeriodicTaskManager::cancel()
{
    MutexGuard lock(mMutex);
    mTimerRunning = false;
}
