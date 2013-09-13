/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include "periodic_task_manager.h"

using namespace cobc::rtos;

PeriodicTaskManager::PeriodicTaskManager() :
	id()
{
	rtems_name name = rtems_build_name('P', 'E', 'R', 'D');
	rtems_status_code status = rtems_rate_monotonic_create(name, &id);

	if (status != RTEMS_SUCCESSFUL) {
		rtos::FailureHandler::fatal(rtos::FailureCode::resourceAllocationFailed());
	}
}

PeriodicTaskManager::~PeriodicTaskManager()
{
	rtems_status_code status = rtems_rate_monotonic_delete(id);
	if (status != RTEMS_SUCCESSFUL)	{
		// TODO create different error code?
		rtos::FailureHandler::fatal(rtos::FailureCode::resourceAllocationFailed());
	}
}
