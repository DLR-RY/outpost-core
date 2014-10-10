/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.3-pre.
 *
 * It is distributed under the terms of the GNU General Public License with
 * a linking exception. See the file "LICENSE" for the full license governing
 * this code.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "mutex.h"

#include <rtems.h>
#include <cobc/rtos/failure_handler.h>

// ----------------------------------------------------------------------------
cobc::rtos::Mutex::Mutex()
{
    rtems_name name = rtems_build_name('M', 'T', 'X', '-');

    if (rtems_semaphore_create(name, 1,
            RTEMS_PRIORITY |
            RTEMS_BINARY_SEMAPHORE |
            RTEMS_INHERIT_PRIORITY, 1, &id) != RTEMS_SUCCESSFUL) {
        rtos::FailureHandler::fatal(rtos::FailureCode::resourceAllocationFailed());
    }
}

cobc::rtos::Mutex::~Mutex()
{
    rtems_semaphore_delete(id);
}

// ----------------------------------------------------------------------------
bool
cobc::rtos::Mutex::acquire()
{
    // wait forever
    rtems_interval timeout = RTEMS_NO_TIMEOUT;

    return (rtems_semaphore_obtain(id, RTEMS_WAIT, timeout) == RTEMS_SUCCESSFUL);
}

bool
cobc::rtos::Mutex::acquire(time::Duration timeout)
{
    rtems_interval time = timeout.milliseconds();
    return (rtems_semaphore_obtain(id, RTEMS_WAIT, time) == RTEMS_SUCCESSFUL);
}
