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

#include "periodic_task_manager.h"

using namespace outpost::rtos;

PeriodicTaskManager::PeriodicTaskManager() : mId()
{
    rtems_name name = rtems_build_name('P', 'E', 'R', 'D');
    rtems_status_code result = rtems_rate_monotonic_create(name, &mId);

    if (result != RTEMS_SUCCESSFUL)
    {
        FailureHandler::fatal(FailureCode::resourceAllocationFailed(Resource::periodicTask));
    }
}

PeriodicTaskManager::~PeriodicTaskManager()
{
    rtems_status_code result = rtems_rate_monotonic_delete(mId);
    if (result != RTEMS_SUCCESSFUL)
    {
        // TODO create different error code?
        FailureHandler::fatal(FailureCode::genericRuntimeError(Resource::periodicTask));
    }
}
