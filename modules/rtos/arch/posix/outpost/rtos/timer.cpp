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

#include "timer.h"

#include "internal/time.h"

#include <outpost/rtos/failure_handler.h>

#include <cstring>

using outpost::rtos::Timer;

Timer::~Timer()
{
    timer_delete(mTid);
}

void
Timer::start(time::Duration duration)
{
    // initial expiration
    timespec relative = toRelativeTime(duration);

    itimerspec time;
    time.it_interval.tv_sec = 0;
    time.it_interval.tv_nsec = 0;

    time.it_value.tv_sec = relative.tv_sec;
    time.it_value.tv_nsec = relative.tv_nsec;

    memcpy(&mInterval, &time, sizeof(itimerspec));

    if (timer_settime(mTid, 0, &time, NULL) != 0)
    {
        // Could not set the timer valuell
        FailureHandler::fatal(FailureCode::genericRuntimeError(Resource::timer));
    }
}

void
Timer::reset()
{
    if (timer_settime(mTid, 0, &mInterval, NULL) != 0)
    {
        // Could not set the timer value
        FailureHandler::fatal(FailureCode::genericRuntimeError(Resource::timer));
    }
}

void
Timer::cancel()
{
    struct itimerspec time;

    memset(&time, 0, sizeof(time));

    if (timer_settime(mTid, 0, &time, NULL) != 0)
    {
        // Could not set the timer value
        FailureHandler::fatal(FailureCode::genericRuntimeError(Resource::timer));
    }
}

bool
Timer::isRunning()
{
    itimerspec value;

    if (timer_gettime(mTid, &value) != 0)
    {
        FailureHandler::fatal(FailureCode::genericRuntimeError(Resource::timer));
    }

    bool running = (value.it_value.tv_sec != 0) || (value.it_value.tv_nsec != 0);
    return running;
}

void Timer::startTimerDaemonThread(uint8_t /*priority*/, size_t /*stack*/)
{
    // do nothing here for POSIX, only used for RTEMS
}

void
Timer::createTimer(const char* /*name*/)
{
    sigevent event;

    memset(&event, 0, sizeof(event));

    // Set the sigevent structure to cause the signal to be
    // delivered by creating a new thread.
    event.sigev_notify = SIGEV_THREAD;
    event.sigev_notify_function = Timer::invokeTimer;
    event.sigev_notify_attributes = NULL;
    event.sigev_value.sival_ptr = this;

    if (timer_create(CLOCK_MONOTONIC, &event, &mTid) != 0)
    {
        // Could not allocate a new timer
        FailureHandler::fatal(FailureCode::resourceAllocationFailed(Resource::timer));
    }

    // Disable timer for now
    this->cancel();
}

void
Timer::invokeTimer(union sigval parameter)
{
    Timer* timer = reinterpret_cast<Timer*>(parameter.sival_ptr);
    (timer->mObject->*(timer->mFunction))(timer);
}
