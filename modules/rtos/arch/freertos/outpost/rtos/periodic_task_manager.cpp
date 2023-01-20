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

#include "periodic_task_manager.h"

#include "traits.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <outpost/rtos/mutex.h>
#include <outpost/rtos/mutex_guard.h>

using namespace outpost::rtos;

PeriodicTaskManager::PeriodicTaskManager() :
    mMutex(), mTimerRunning(false), mLastWakeTime(), mCurrentPeriod()
{
}

PeriodicTaskManager::~PeriodicTaskManager()
{
}

PeriodicTaskManager::Status::Type
PeriodicTaskManager::nextPeriod(time::Duration period)
{
    MutexGuard lock(mMutex);
    Status::Type currentStatus = Status::running;

    const TickType_t nextPeriodTicks =
            period.milliseconds() < 1000 * 1000
                    ?  // prevent overflows by ordering dependent of amount.
                    (period.milliseconds() * configTICK_RATE_HZ) / 1000
                    : (period.milliseconds() / 1000) * configTICK_RATE_HZ;
    if (mTimerRunning)
    {
        if (static_cast<Traits<TickType_t>::SignedType>(xTaskGetTickCount() - mLastWakeTime)
            > static_cast<Traits<TickType_t>::SignedType>(mCurrentPeriod))
        {
            currentStatus = Status::timeout;
        }

        vTaskDelayUntil(&mLastWakeTime, mCurrentPeriod);
    }
    else
    {
        // period is started now, no need to wait
        mLastWakeTime = xTaskGetTickCount();
        mTimerRunning = true;
    }

    mCurrentPeriod = nextPeriodTicks;
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
    else if (static_cast<Traits<TickType_t>::SignedType>(xTaskGetTickCount() - mLastWakeTime)
             > static_cast<Traits<TickType_t>::SignedType>(mCurrentPeriod))
    {
        return Status::timeout;
    }
    else
    {
        return Status::running;
    }
}

void
PeriodicTaskManager::cancel()
{
    MutexGuard lock(mMutex);
    mTimerRunning = false;
}
