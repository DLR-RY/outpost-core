/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.4.
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

#include <cstring>
#include <cobc/rtos/failure_handler.h>

// ----------------------------------------------------------------------------
cobc::rtos::Timer::~Timer()
{
    timer_delete(mTid);
}

// ----------------------------------------------------------------------------
void
cobc::rtos::Timer::start(time::Duration duration)
{
    struct itimerspec time;
    uint64_t nanoseconds = duration.microseconds() * cobc::time::Duration::nanosecondsPerMicrosecond;

    // interval
    time.it_interval.tv_sec = 0;
    time.it_interval.tv_nsec = 0;

    // initial expiration
    // seconds
    time.it_value.tv_sec = static_cast<time_t>(nanoseconds / 1000000000);
    // nanoseconds
    time.it_value.tv_nsec = static_cast<uint32_t>(nanoseconds % 1000000000);

    memcpy(&mInterval, &time, sizeof(struct itimerspec));

    if (timer_settime(mTid, 0, &time, NULL) != 0)
    {
        // Could not set the timer valuell
        FailureHandler::fatal(FailureCode::genericRuntimeError(Resource::timer));
    }
}

void
cobc::rtos::Timer::reset()
{
    if (timer_settime(mTid, 0, &mInterval, NULL) != 0)
    {
        // Could not set the timer value
        FailureHandler::fatal(FailureCode::genericRuntimeError(Resource::timer));
    }
}

// Disarm timer by
void
cobc::rtos::Timer::cancel()
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
cobc::rtos::Timer::isRunning()
{
    struct itimerspec value;

    if (timer_gettime(mTid, &value) != 0)
    {
        FailureHandler::fatal(FailureCode::genericRuntimeError(Resource::timer));
    }

    bool running = (value.it_value.tv_sec != 0) || (value.it_value.tv_nsec != 0);
    return running;
}

// ----------------------------------------------------------------------------
void
cobc::rtos::Timer::startTimerDaemonThread(uint8_t /*priority*/,
                                          size_t /*stack*/)
{
	// do nothing here for POSIX, only used for RTEMS
}

// ----------------------------------------------------------------------------
void
cobc::rtos::Timer::createTimer(const char* /*name*/)
{
    struct sigevent event;

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

// ----------------------------------------------------------------------------
void
cobc::rtos::Timer::invokeTimer(union sigval parameter)
{
    Timer* timer = reinterpret_cast<Timer *>(parameter.sival_ptr);
    (timer->mObject->*(timer->mFunction))(timer);
}
