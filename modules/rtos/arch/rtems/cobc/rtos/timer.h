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

#ifndef COBC_RTOS_RTEMS_TIMER_H
#define COBC_RTOS_RTEMS_TIMER_H

#include <rtems.h>
#include <cobc/time/duration.h>

#include <cobc/utils/callable.h>

namespace cobc
{
namespace rtos
{

/**
 * Software timer.
 *
 * The timer callback functions are called in the context of the
 * timer server thread.
 *
 * The Timer Server thread is responsible for executing the timer
 * service routines associated with all task-based timers.
 *
 * The Timer Server is designed to remain blocked until a task-based
 * timer fires. This reduces the execution overhead of the Timer Server.
 *
 * \warning    You need to call Timer::startTimerDaemonThread() somewhere
 *             in your start-up code. Otherwise the timers won't work!
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
     *         be sub-class of cobc::rtos::Callable.
     * \param function
     *         Member function of \p object to call when the timer
     *         expires.
     * \param name
     *         Name of the timer. Maximum length is four characters. Longer
     *         names will be truncated.
     *
     * Example:
     * \code
     * class A : public cobc::rtos::Callable
     * {
     * public:
     *     void
     *     function(cobc::rtos::Timer*)
     *     {
     *     }
     * };
     *
     * A a;
     *
     * cobc::rtos::Timer timer(&a, &A::function);
     * \endcode
     *
     * Example 2:
     * \code
     * class B : public cobc::rtos::Callable
     * {
     * public:
     *     B() :
     *         timer(this, &B::function, "TIM2");
     *     {
     *     }
     *
     *     void
     *     function(cobc::rtos::Timer*)
     *     {
     *     }
     *
     * private:
     *     cobc::rtos::Timer timer;
     * };
     * \endcode
     *
     * \see    cobc::Callable
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
     * Start the timer daemon.
     *
     * This function initiates the Timer Server thread. This thread is
     * responsible for executing all timers.
     *
     * As it is a normal thread it was to be considered when
     * configuring CONFIGURE_MAXIMUM_TASKS.
     *
     * \param priority
     *         Thread priority. RTEMS supports priorities between 1..255.
     *         Lower values represent a higher priority. To ensure that
     *         the handler-function is called at the exact time it is
     *         a good idea to give the timer daemon thread a high
     *         priority (low value).
     * \param stack
     *         Stack size in bytes. If the stack is smaller than the
     *         default stack size it is replaced with the default size.
     */
    static void
    startTimerDaemonThread(uint8_t priority, size_t stack = 0);

private:
    void
    createTimer(const char* name);

    /**
     * Helper function to map the void-pointer to a
     * member function pointer and call the member function.
     */
    static void
    invokeTimer(rtems_id id, void* timer);

    /// Object and member function to call when the timer expires.
    Callable* const mObject;
    Function const mFunction;

    rtems_id mTid;
};

}
}

// ----------------------------------------------------------------------------
// Implementation of the template constructor
template <typename T>
cobc::rtos::Timer::Timer(T* object, void (T::*function)(Timer* timer), const char* name) :
    mObject(reinterpret_cast<Callable *>(object)),
    mFunction(reinterpret_cast<Function>(function)),
    mTid()
{
    this->createTimer(name);
}

#endif // COBC_RTOS_RTEMS_TIMER_H
