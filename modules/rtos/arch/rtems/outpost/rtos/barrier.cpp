/*
 * Copyright (c) 2020, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2020, Jan Malburg (DLR RY-AVS)
 */

#include "barrier.h"

#include <rtems.h>

#include <outpost/rtos/failure_handler.h>

// ----------------------------------------------------------------------------
outpost::rtos::Barrier::Barrier(uint32_t numberOfThreads)
{
    rtems_name name = rtems_build_name('B', 'A', 'R', '-');

    rtems_status_code status =
            rtems_barrier_create(name, RTEMS_BARRIER_AUTOMATIC_RELEASE, numberOfThreads, &mId);
    if (status != RTEMS_SUCCESSFUL)
    {
        FailureHandler::fatal(FailureCode::resourceAllocationFailed(Resource::barrier));
    }
}

outpost::rtos::Barrier::~Barrier()
{
    rtems_barrier_delete(mId);
}

void
outpost::rtos::Barrier::wait()
{
    rtems_barrier_wait(mId, RTEMS_NO_TIMEOUT);
}
