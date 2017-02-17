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
// ----------------------------------------------------------------------------

#ifndef COBC_RTOS_FREERTOS_TIMER_H
#define COBC_RTOS_FREERTOS_TIMER_H

#include <cstddef>

#include <outpost/time/duration.h>

#include <outpost/utils/callable.h>

namespace outpost
{
namespace rtos
{

/**
 * Software timer.
 *
 * The timer callback functions are called in the context of the
 * timer daemon thread.
 *
 * The timer daemon thread is responsible for executing the timer
 * service routines associated with all task-based timers.
 *
 * \warning You need to enable the timer daemon thread in your FreeRTOS
 *          configuration. Otherwise the timers won't work! See
 *          startTimerDaemonThread().
 *
 * \author    Fabian Greif
 * \ingroup    rtos
 */
class Timer
{
public:
    /**
     * Type of the timer handler function.
     *
     * \param timer
     *         Timer which caused the function to be called. Can be used
     *         to restart the timer.
     */
    typedef void (Callable::*Function)(Timer* timer);

    /**
     * Create a timer.
     *
     * \param object
     *         Instance to with the function to be called belongs. Must
     *         be sub-class of outpost::rtos::Callable.
     * \param function
     *         Member function of \p object to call when the timer
     *         expires.
     * \param name
     *         Name of the timer. Maximum length is four characters. Longer
     *         names will be truncated.
     *
     * \see    outpost::Callable
     */
    template <typename T>
    Timer(T* object, void (T::*function)(Timer* timer), const char* name = "TIM-");

    /**
     * Delete the timer.
     *
     * If the timer is running, it is automatically canceled. All it's
     * allocated resources are reclaimed and can be used for another
     * timer.
     */
    ~Timer();

    /**
     * Start the timer.
     *
     * If the timer is running it is automatically reset before being
     * initiated.
     *
     * \param duration
     *         Runtime duration.
     */
    void
    start(time::Duration duration);

    /**
     * Reset the timer interval to it's original value when it is
     * currently running.
     *
     * If the timer is not running this has the same effect as calling
     * start().
     */
    void
    reset();

    /**
     * Abort operation.
     *
     * The timer will not fire until the next invocation of reset() or
     * start().
     */
    void
    cancel();

    /**
     * Check whether the timer is currently running.
     *
     * \retval  true    Timer is running
     * \retval  false   Timer has not been started or was stopped.
     */
    bool
    isRunning();

    /**
     * Start timer daemon.
     *
     * Unused for FreeRTOS. The timer daemon is automatically started
     * by including \c freertos/timers.c and adjusting the following
     * defines in \c freertos/FreeRTOSConfig.h:
     *
     * - configUSE_TIMERS = 1
     * - configTIMER_TASK_PRIORITY
     * - configTIMER_QUEUE_LENGTH
     * - configTIMER_TASK_STACK_DEPTH
     *
     * \see http://www.freertos.org/Configuring-a-real-time-RTOS-application-to-use-software-timers.html
     */
    static void
    startTimerDaemonThread(uint8_t priority, size_t stack = 0);

private:
    void
    createTimer(const char* name);

    static void
    invokeTimer(void* handle);

    /// Object and member function to call when the timer expires.
    Callable* const mObject;
    Function const mFunction;

    /// FreeRTOS timer handle
    void* mHandle;
};

}
}

// ----------------------------------------------------------------------------
// Implementation of the template constructor
template <typename T>
outpost::rtos::Timer::Timer(T* object, void (T::*function)(Timer* timer), const char* name) :
    mObject(reinterpret_cast<Callable *>(object)),
    mFunction(reinterpret_cast<Function>(function)),
    mHandle()
{
    this->createTimer(name);
}

#endif // COBC_RTOS_RTEMS_TIMER_H
