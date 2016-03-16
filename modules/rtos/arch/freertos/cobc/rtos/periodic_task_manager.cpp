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

#include "periodic_task_manager.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <cobc/rtos/mutex.h>
#include <cobc/rtos/mutex_guard.h>

#include "traits.h"

using namespace cobc::rtos;

PeriodicTaskManager::PeriodicTaskManager() :
    mMutex(),
    mTimerRunning(false),
    mLastWakeTime(),
    mCurrentPeriod()
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

    const portTickType nextPeriodTicks = (period.milliseconds() * configTICK_RATE_HZ) / 1000;
    if (mTimerRunning) {
        if (static_cast<Traits<portTickType>::SignedType>(xTaskGetTickCount() - mLastWakeTime) >
                static_cast<Traits<portTickType>::SignedType>(mCurrentPeriod)) {
            currentStatus = Status::timeout;
        }

        vTaskDelayUntil(&mLastWakeTime, mCurrentPeriod);
    }
    else {
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

    if (!mTimerRunning) {
        return Status::idle;
    }
    else if (static_cast<Traits<portTickType>::SignedType>(xTaskGetTickCount() - mLastWakeTime) >
                static_cast<Traits<portTickType>::SignedType>(mCurrentPeriod)) {
        return Status::timeout;
    }
    else {
        return Status::running;
    }
}

void
PeriodicTaskManager::cancel()
{
    MutexGuard lock(mMutex);
    mTimerRunning = false;
}
