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

#include "periodic_task_manager.h"

using namespace cobc::rtos;

PeriodicTaskManager::PeriodicTaskManager() :
    mId()
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
