/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include "semaphore.h"

#include <rtems.h>
#include "../failure_handler.h"

// ----------------------------------------------------------------------------
cobc::rtos::Semaphore::Semaphore(uint32_t count)
{
	rtems_name name = rtems_build_name('C', 'S', 'E', 'M');

	if (rtems_semaphore_create(name, count,
			RTEMS_PRIORITY |
			RTEMS_COUNTING_SEMAPHORE, 1, &id) != RTEMS_SUCCESSFUL) {
		rtos::FailureHandler::fatal(rtos::FailureCode::resourceAllocationFailed());
	}
}

cobc::rtos::Semaphore::~Semaphore()
{
	rtems_semaphore_delete(id);
}

// ----------------------------------------------------------------------------
bool
cobc::rtos::Semaphore::acquire()
{
	// wait forever
	rtems_interval timeout = RTEMS_NO_TIMEOUT;

	return (rtems_semaphore_obtain(id, RTEMS_WAIT, timeout) == RTEMS_SUCCESSFUL);
}

bool
cobc::rtos::Semaphore::acquire(time::Duration timeout)
{
	return (rtems_semaphore_obtain(id, RTEMS_WAIT, timeout.milliseconds()) == RTEMS_SUCCESSFUL);
}

// ----------------------------------------------------------------------------
cobc::rtos::BinarySemaphore::BinarySemaphore(State initial)
{
	rtems_name name = rtems_build_name('B', 'S', 'E', 'M');

	if (rtems_semaphore_create(name, (initial == acquired) ? 0 : 1,
			RTEMS_PRIORITY |
			RTEMS_BINARY_SEMAPHORE, 1, &id) != RTEMS_SUCCESSFUL) {
		rtos::FailureHandler::fatal(rtos::FailureCode::resourceAllocationFailed());
	}
}

cobc::rtos::BinarySemaphore::~BinarySemaphore()
{
	rtems_semaphore_delete(id);
}

// ----------------------------------------------------------------------------
bool
cobc::rtos::BinarySemaphore::acquire()
{
	// wait forever
	rtems_interval timeout = RTEMS_NO_TIMEOUT;
	return (rtems_semaphore_obtain(id, RTEMS_WAIT, timeout) == RTEMS_SUCCESSFUL);
}

bool
cobc::rtos::BinarySemaphore::acquire(time::Duration timeout)
{
	return (rtems_semaphore_obtain(id, RTEMS_WAIT, timeout.milliseconds()) == RTEMS_SUCCESSFUL);
}
