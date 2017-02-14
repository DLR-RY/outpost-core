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

#include "semaphore.h"

#include <rtems.h>
#include <outpost/rtos/failure_handler.h>

#include "rtems/interval.h"

// ----------------------------------------------------------------------------
outpost::rtos::Semaphore::Semaphore(uint32_t count)
{
    rtems_name name = rtems_build_name('C', 'S', 'E', 'M');

    if (rtems_semaphore_create(name, count,
            RTEMS_PRIORITY |
            RTEMS_COUNTING_SEMAPHORE, 1, &mId) != RTEMS_SUCCESSFUL)
    {
        FailureHandler::fatal(FailureCode::resourceAllocationFailed(Resource::semaphore));
    }
}

outpost::rtos::Semaphore::~Semaphore()
{
    rtems_semaphore_delete(mId);
}

// ----------------------------------------------------------------------------
bool
outpost::rtos::Semaphore::acquire()
{
    // wait forever
    rtems_interval timeout = RTEMS_NO_TIMEOUT;

    return (rtems_semaphore_obtain(mId, RTEMS_WAIT, timeout) == RTEMS_SUCCESSFUL);
}

bool
outpost::rtos::Semaphore::acquire(time::Duration timeout)
{
    rtems_option waitOption = RTEMS_WAIT;
    if (timeout == time::Duration::zero())
    {
        waitOption = RTEMS_NO_WAIT;
    } // else use default value RTEMS_WAIT
    return (rtems_semaphore_obtain(mId, waitOption, rtems::getInterval(timeout)) == RTEMS_SUCCESSFUL);
}

// ----------------------------------------------------------------------------
outpost::rtos::BinarySemaphore::BinarySemaphore(State::Type initial)
{
    rtems_name name = rtems_build_name('B', 'S', 'E', 'M');

    if (rtems_semaphore_create(name, (initial == State::acquired) ? 0 : 1,
            RTEMS_PRIORITY |
            RTEMS_SIMPLE_BINARY_SEMAPHORE, 1, &mId) != RTEMS_SUCCESSFUL)
    {
        FailureHandler::fatal(FailureCode::resourceAllocationFailed(Resource::semaphore));
    }
}

outpost::rtos::BinarySemaphore::~BinarySemaphore()
{
    rtems_semaphore_delete(mId);
}

// ----------------------------------------------------------------------------
bool
outpost::rtos::BinarySemaphore::acquire()
{
    // wait forever
    rtems_interval timeout = RTEMS_NO_TIMEOUT;
    return (rtems_semaphore_obtain(mId, RTEMS_WAIT, timeout) == RTEMS_SUCCESSFUL);
}

bool
outpost::rtos::BinarySemaphore::acquire(time::Duration timeout)
{
    rtems_option waitOption = RTEMS_WAIT;
    if (timeout == time::Duration::zero())
    {
        waitOption = RTEMS_NO_WAIT;
    } // else use default value RTEMS_WAIT
    return (rtems_semaphore_obtain(mId, waitOption, rtems::getInterval(timeout)) == RTEMS_SUCCESSFUL);
}
