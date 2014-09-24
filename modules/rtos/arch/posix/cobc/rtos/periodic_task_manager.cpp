/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include "periodic_task_manager.h"

#include <errno.h>

static const uint32_t nanosecondsPerSecond = 1000000000;
static const uint32_t nanosecondsPerMicrosecond = 1000;

using namespace cobc::rtos;

/**
 *
 * \param[inout] time
 * \param[in]    duration
 */
static void
addTime(timespec* time, cobc::time::Duration duration)
{
    uint64_t nanoseconds = duration.microseconds() * nanosecondsPerMicrosecond;
    nanoseconds += time->tv_nsec;

    // add seconds
    time->tv_sec += static_cast<time_t>(nanoseconds / nanosecondsPerSecond);

    // store remaining nanoseconds
    time->tv_nsec = static_cast<uint32_t>(nanoseconds % nanosecondsPerSecond);
}

/**
 * Compare two times.
 *
 * \param time
 * \param other
 *
 * \retval true time is bigger or equal than other
 * \retval false time is lower than other
 */
static bool
isBigger(timespec* time, timespec* other)
{
    if (time->tv_sec > other->tv_sec)
    {
        return true;
    }
    else if (time->tv_sec == other->tv_sec)
    {
        if (time->tv_nsec >= other->tv_nsec)
        {
            return true;
        }
    }

    return false;
}

static inline void
getTime(timespec* time)
{
    int result = clock_gettime(CLOCK_MONOTONIC, time);
    if (result != 0)
    {
        FailureHandler::fatal(FailureCode::genericRuntimeError());
    }
}

PeriodicTaskManager::PeriodicTaskManager() :
    mutex(),
    timerRunning(false),
    nextWakeTime()
{
}

PeriodicTaskManager::~PeriodicTaskManager()
{
}

PeriodicTaskManager::Status::Type
PeriodicTaskManager::nextPeriod(time::Duration period)
{
    MutexGuard lock(mutex);
    Status::Type status = Status::running;

    if (timerRunning)
    {
        struct timespec currentTime;
        getTime(&currentTime);

        // Check if the time is in the current period
        if (isBigger(&currentTime, &nextWakeTime))
        {
            status = Status::timeout;
        }

        int result;
        do
        {
            result = clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME,
                    &nextWakeTime, NULL);

            // EINTR is returned when the sleep is interrupted by a signal
            // handler. In this case the sleep can be restarted.
            //
            // Any other result unequal zero is an failure which can not be
            // resolved here and therefore triggers the fatal error handler.
            if (result != 0 && result != EINTR)
            {
                FailureHandler::fatal(FailureCode::genericRuntimeError());
            }
        }
        while (result != 0);
    }
    else
    {
        // period is started now, no need to wait
        clock_gettime(CLOCK_MONOTONIC, &nextWakeTime);
        timerRunning = true;
    }

    // calculate the next wake-up time
    addTime(&nextWakeTime, period);

    return status;
}

PeriodicTaskManager::Status::Type
PeriodicTaskManager::status()
{
    MutexGuard lock(mutex);

    if (!timerRunning)
    {
        return Status::idle;
    }
    else
    {
        struct timespec currentTime;
        getTime(&currentTime);

        // Check if the time is in the current period
        if (isBigger(&currentTime, &nextWakeTime))
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
    MutexGuard lock(mutex);
    timerRunning = false;
}
