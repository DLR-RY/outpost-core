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
// ----------------------------------------------------------------------------

#include "timer.h"

#include <outpost/rtos/failure_handler.h>
#include <outpost/rtos/mutex_guard.h>

// ----------------------------------------------------------------------------
outpost::rtos::Timer::~Timer()
{
    rtems_timer_delete(mTid);
}

// ----------------------------------------------------------------------------
void
outpost::rtos::Timer::start(time::Duration duration)
{
    MutexGuard lock(mMutex);
    mRunning = true;
    rtems_timer_server_fire_after(mTid,
                                  duration.microseconds() / rtems_configuration_get_microseconds_per_tick(),
                                  &Timer::invokeTimer,
                                  (void *) this);
}

void
outpost::rtos::Timer::reset()
{
    MutexGuard lock(mMutex);
    rtems_status_code result = rtems_timer_reset(mTid);
    mRunning = (result == RTEMS_SUCCESSFUL);
}

void
outpost::rtos::Timer::cancel()
{
    MutexGuard lock(mMutex);
    mRunning = false;
    rtems_timer_cancel(mTid);
}

bool
outpost::rtos::Timer::isRunning()
{
    MutexGuard lock(mMutex);
    return mRunning;
}

// ----------------------------------------------------------------------------
void
outpost::rtos::Timer::startTimerDaemonThread(uint8_t priority, size_t stack)
{
    rtems_status_code result = rtems_timer_initiate_server(priority, stack, RTEMS_DEFAULT_ATTRIBUTES);
    if (result != RTEMS_SUCCESSFUL)
    {
        FailureHandler::fatal(FailureCode::resourceAllocationFailed(Resource::timer));
    }
}

// ----------------------------------------------------------------------------
void
outpost::rtos::Timer::createTimer(const char* name)
{
    rtems_name taskName = 0;
    if (name == 0)
    {
        // taskName = 0 is not allowed.
        taskName = rtems_build_name('T', 'I', 'M', '-');;
    }
    else
    {
        for (uint_fast8_t i = 0; i < 4; ++i)
        {
            if (name != 0)
            {
                taskName |= *name++;
            }
            taskName <<= 8;
        }
    }

    rtems_status_code result = rtems_timer_create(taskName, &mTid);
    if (result != RTEMS_SUCCESSFUL)
    {
        FailureHandler::fatal(FailureCode::resourceAllocationFailed(Resource::timer));
    }
}

// ----------------------------------------------------------------------------
void
outpost::rtos::Timer::invokeTimer(rtems_id id, void* parameter)
{
    (void) id;    // not used here

    Timer* timer = reinterpret_cast<Timer *>(parameter);
    {
        MutexGuard lock(timer->mMutex);
        timer->mRunning = false;
    }
    (timer->mObject->*(timer->mFunction))(timer);
}
