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

#include "mutex.h"

#include "rtems/interval.h"

#include <rtems.h>

#include <outpost/rtos/failure_handler.h>

// ----------------------------------------------------------------------------
outpost::rtos::Mutex::Mutex()
{
    rtems_name name = rtems_build_name('M', 'T', 'X', '-');

    rtems_status_code status = rtems_semaphore_create(
            name, 1, RTEMS_PRIORITY | RTEMS_BINARY_SEMAPHORE | RTEMS_INHERIT_PRIORITY, 1, &mId);
    if (status != RTEMS_SUCCESSFUL)
    {
        FailureHandler::fatal(FailureCode::resourceAllocationFailed(Resource::mutex));
    }
}

outpost::rtos::Mutex::~Mutex()
{
    rtems_semaphore_delete(mId);
}

// ----------------------------------------------------------------------------
bool
outpost::rtos::Mutex::acquire()
{
    // wait forever
    rtems_interval timeout = RTEMS_NO_TIMEOUT;

    return (rtems_semaphore_obtain(mId, RTEMS_WAIT, timeout) == RTEMS_SUCCESSFUL);
}

bool
outpost::rtos::Mutex::acquire(time::Duration timeout)
{
    rtems_interval time = rtems::getInterval(timeout);
    return (rtems_semaphore_obtain(mId, RTEMS_WAIT, time) == RTEMS_SUCCESSFUL);
}
