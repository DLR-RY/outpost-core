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
 * - 2013, Norbert Toth (DLR RY-AVS)
 */

#include "timer.h"

#include <outpost/rtos/failure_handler.h>

// ----------------------------------------------------------------------------
outpost::rtos::Timer::~Timer()
{
    if (xTimerDelete(mHandle, portMAX_DELAY) != pdPASS)
    {
        FailureHandler::fatal(FailureCode::resourceAllocationFailed(Resource::timer));
    }
}

// ----------------------------------------------------------------------------
void
outpost::rtos::Timer::start(time::Duration duration)
{
    if ((xTimerChangePeriod(mHandle,
                            duration.milliseconds() > 1000 * 1000
                                    ?  // prevent overflows for large delays, and underflows for
                                       // short delays
                                    (duration.milliseconds() * configTICK_RATE_HZ) / 1000
                                    : (duration.milliseconds() / 1000) * configTICK_RATE_HZ,
                            portMAX_DELAY)
         != pdPASS)
        || (xTimerStart(mHandle, portMAX_DELAY) != pdPASS))
    {
        FailureHandler::fatal(FailureCode::genericRuntimeError(Resource::timer));
    }
}

void
outpost::rtos::Timer::reset()
{
    if (xTimerReset(mHandle, portMAX_DELAY) != pdPASS)
    {
        FailureHandler::fatal(FailureCode::genericRuntimeError(Resource::timer));
    }
}

void
outpost::rtos::Timer::cancel()
{
    if (xTimerStop(mHandle, portMAX_DELAY) != pdPASS)
    {
        FailureHandler::fatal(FailureCode::genericRuntimeError(Resource::timer));
    }
}

bool
outpost::rtos::Timer::isRunning()
{
    bool active = false;
    if (xTimerIsTimerActive(mHandle) != pdFALSE)
    {
        active = true;
    }

    return active;
}

// ----------------------------------------------------------------------------
void
outpost::rtos::Timer::startTimerDaemonThread(uint8_t priority, size_t stack)
{
    (void) priority;
    (void) stack;
}

// ----------------------------------------------------------------------------
void
outpost::rtos::Timer::createTimer(const char* name)
{
    mHandle = xTimerCreate(
            name,
            1,        // dummy value (must be >= 0 but will be changed when starting the timer)
            pdFALSE,  // no auto-reload
            (void*) this,
            &Timer::invokeTimer);

    if (mHandle == 0)
    {
        FailureHandler::fatal(FailureCode::resourceAllocationFailed(Resource::timer));
    }
}

// ----------------------------------------------------------------------------
void
outpost::rtos::Timer::invokeTimer(TimerHandle_t handle)
{
    Timer* timer = reinterpret_cast<Timer*>(pvTimerGetTimerID(handle));
    (timer->mObject->*(timer->mFunction))(timer);
}
