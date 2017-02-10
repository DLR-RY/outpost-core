/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.6.
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

#include "mutex.h"

#include <rtems.h>
#include <cobc/rtos/failure_handler.h>

#include "rtems/interval.h"

// ----------------------------------------------------------------------------
cobc::rtos::Mutex::Mutex()
{
    rtems_name name = rtems_build_name('M', 'T', 'X', '-');

    rtems_status_code status = rtems_semaphore_create(name, 1,
            RTEMS_PRIORITY |
            RTEMS_BINARY_SEMAPHORE |
            RTEMS_INHERIT_PRIORITY, 1, &mId);
    if (status != RTEMS_SUCCESSFUL)
    {
        FailureHandler::fatal(FailureCode::resourceAllocationFailed(Resource::mutex));
    }
}

cobc::rtos::Mutex::~Mutex()
{
    rtems_semaphore_delete(mId);
}

// ----------------------------------------------------------------------------
bool
cobc::rtos::Mutex::acquire()
{
    // wait forever
    rtems_interval timeout = RTEMS_NO_TIMEOUT;

    return (rtems_semaphore_obtain(mId, RTEMS_WAIT, timeout) == RTEMS_SUCCESSFUL);
}

bool
cobc::rtos::Mutex::acquire(time::Duration timeout)
{
    rtems_interval time = rtems::getInterval(timeout);
    return (rtems_semaphore_obtain(mId, RTEMS_WAIT, time) == RTEMS_SUCCESSFUL);
}
