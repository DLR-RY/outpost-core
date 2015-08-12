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

#include "semaphore.h"

#include <rtems.h>
#include <cobc/rtos/failure_handler.h>

// ----------------------------------------------------------------------------
cobc::rtos::Semaphore::Semaphore(uint32_t count)
{
    rtems_name name = rtems_build_name('C', 'S', 'E', 'M');

    if (rtems_semaphore_create(name, count,
            RTEMS_PRIORITY |
            RTEMS_COUNTING_SEMAPHORE, 1, &mId) != RTEMS_SUCCESSFUL)
    {
        rtos::FailureHandler::fatal(rtos::FailureCode::resourceAllocationFailed());
    }
}

cobc::rtos::Semaphore::~Semaphore()
{
    rtems_semaphore_delete(mId);
}

// ----------------------------------------------------------------------------
bool
cobc::rtos::Semaphore::acquire()
{
    // wait forever
    rtems_interval timeout = RTEMS_NO_TIMEOUT;

    return (rtems_semaphore_obtain(mId, RTEMS_WAIT, timeout) == RTEMS_SUCCESSFUL);
}

bool
cobc::rtos::Semaphore::acquire(time::Duration timeout)
{
    return (rtems_semaphore_obtain(mId, RTEMS_WAIT, timeout.milliseconds()) == RTEMS_SUCCESSFUL);
}

// ----------------------------------------------------------------------------
cobc::rtos::BinarySemaphore::BinarySemaphore(State::Type initial)
{
    rtems_name name = rtems_build_name('B', 'S', 'E', 'M');

    if (rtems_semaphore_create(name, (initial == State::acquired) ? 0 : 1,
            RTEMS_PRIORITY |
            RTEMS_SIMPLE_BINARY_SEMAPHORE, 1, &mId) != RTEMS_SUCCESSFUL)
    {
        rtos::FailureHandler::fatal(rtos::FailureCode::resourceAllocationFailed());
    }
}

cobc::rtos::BinarySemaphore::~BinarySemaphore()
{
    rtems_semaphore_delete(mId);
}

// ----------------------------------------------------------------------------
bool
cobc::rtos::BinarySemaphore::acquire()
{
    // wait forever
    rtems_interval timeout = RTEMS_NO_TIMEOUT;
    return (rtems_semaphore_obtain(mId, RTEMS_WAIT, timeout) == RTEMS_SUCCESSFUL);
}

bool
cobc::rtos::BinarySemaphore::acquire(time::Duration timeout)
{
    return (rtems_semaphore_obtain(mId, RTEMS_WAIT, timeout.milliseconds()) == RTEMS_SUCCESSFUL);
}
