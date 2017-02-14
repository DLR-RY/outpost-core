/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 *
 * This file is part of outpost 0.6.
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

#include "timer.h"

#include <outpost/rtos/failure_handler.h>

#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>
#include <freertos/task.h>

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
                            (duration.milliseconds() * configTICK_RATE_HZ) / 1000,
                            portMAX_DELAY) != pdPASS) ||
        (xTimerStart(mHandle, portMAX_DELAY) != pdPASS))
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
    mHandle = xTimerCreate(reinterpret_cast<const signed char *>(name),
                          1,        // dummy value (must be >= 0 but will be changed when starting the timer)
                          pdFALSE,    // no auto-reload
                          (void *) this,
                          &Timer::invokeTimer);

    if (mHandle == 0)
    {
        FailureHandler::fatal(FailureCode::resourceAllocationFailed(Resource::timer));
    }
}

// ----------------------------------------------------------------------------
void
outpost::rtos::Timer::invokeTimer(void* handle)
{
    Timer* timer = reinterpret_cast<Timer *>(pvTimerGetTimerID(handle));
    (timer->mObject->*(timer->mFunction))(timer);
}
