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
 * - 2015-2016, Jan Sommer (DLR SC-SRV)
 */
// ----------------------------------------------------------------------------

#ifndef OUTPOST_RTOS_RTEMS_THREAD_H
#define OUTPOST_RTOS_RTEMS_THREAD_H

#include <cstddef>
#include <stdint.h>

#include <rtems.h>
#include "rtems/interval.h"

namespace outpost
{
namespace rtos
{

/**
 * Wrapper class for the Thread function of the Operating System.
 *
 * Encapsulates the RTEMS C thread functions and provides C++-style
 * access to them.
 *
 * The run()-method of a derived class is invoked in the newly created
 * thread context. The derived class can also hold data members
 * associated with the specific thread.
 *
 * This class uses the standard RTEMS tasks (not the POSIX threads).
 * Make sure to adapt CONFIGURE_MAXIMUM_TASKS to the number of
 * thread created. If more threads than available are created the
 * FailureHandler::fatal() handler is called.
 *
 * \author    Fabian Greif
 * \ingroup    rtos
 */
class Thread
{
public:
    /// Unique identifier to identify a thread.
    typedef uint32_t Identifier;

    enum FloatingPointSupport
    {
        noFloatingPoint,
        floatingPoint
    };

    /**
     * Use the default value for the stack size.
     *
     * The default value is depending on the project settings.
     */
    static const size_t defaultStackSize = 0;

    /**
     * Create a new thread.
     *
     * Make sure to set CONFIGURE_MAXIMUM_TASKS to the maximum number
     * of executed tasks. Otherwise this constructor will fail to
     * allocate the necessary thread id and call the fatal error
     * handler.
     *
     * \param priority
     *         Thread priority. A higher value represent a higher priority.
     * \param stack
     *         Stack size in bytes. If the stack is smaller than the
     *         default stack size it is replaced with the default size.
     * \param name
     *         Name of the thread. Length is limited to four characters
     *         due to RTEMS internal handling of the string.
     * \param floatingPointSupport
     *      Enable usage of floating point operations for this thread. If
     *      hardware supported floating point is used, each thread which
     *      uses floating point must enable this!
     *
     * \see    rtos::FailureHandler::fatal()
     */
    Thread(uint8_t priority,
           size_t stack = defaultStackSize,
           const char* name = 0,
           FloatingPointSupport floatingPointSupport = floatingPoint);

    /**
     * Destructor.
     *
     * Frees the resources used by this thread to be used by another
     * thread.
     */
    virtual
    ~Thread();

    /**
     * Start the execution of the thread.
     *
     * This may preempt the current thread if the thread to be executed
     * has a higher priority.
     */
    void
    start();

    /**
     * Get a unique identifier for this thread.
     *
     * \return  Unique identifier.
     */
    Identifier
    getIdentifier() const;

    /**
     * Get the unique identifier for the currently executed thread.
     *
     * \return  Unique identifier.
     */
    static Identifier
    getCurrentThreadIdentifier();

    /**
     * Set a new priority for the thread.
     *
     * The calling thread may be preempted if it lowers its own
     * priority or raises another thread's priority.
     *
     * From the RTEMS user manual:
     *
     *     If the task specified by id is currently holding any binary
     *     semaphores which use the priority inheritance algorithm,
     *     then the task’s priority cannot be lowered immediately. If
     *     the task’s priority were lowered immediately, then priority
     *     inversion results. The requested lowering of the task’s
     *     priority will occur when the task has released all priority
     *     inheritance binary semaphores. The task’s priority can be
     *     increased regardless of the task’s use of priority
     *     inheritance binary semaphores.
     *
     * \param priority
     *         Thread priority. Higher values represent a higher priority.
     */
    void
    setPriority(uint8_t priority);

    /**
     * Read the priority.
     *
     * \return    Priority of the thread
     */
    uint8_t
    getPriority() const;

    /**
     * Give up the processor but remain in ready state.
     */
    static inline void
    yield()
    {
        rtems_task_wake_after(RTEMS_YIELD_PROCESSOR);
    }

    /**
     * Suspend the current thread for the given time.
     *
     * \param timeout
     *         Time to sleep.
     */
    static inline void
    sleep(::outpost::time::Duration duration)
    {
        rtems_task_wake_after(rtems::getInterval(duration));
    }

protected:
    /**
     * Working method of the thread.
     *
     * This method is called after the thread is started. It may never
     * return (endless loop). On a return the fatal error
     * handler will be called.
     */
    virtual void
    run() = 0;

private:
    // disable copy constructor
    Thread(const Thread& other);

    // disable assignment operator
    Thread&
    operator=(const Thread& other);

    static void
    wrapper(rtems_task_argument object);

    rtems_id mTid;
};

}
}

#endif
