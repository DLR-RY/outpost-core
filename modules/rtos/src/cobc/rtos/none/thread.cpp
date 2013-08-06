/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include "thread.h"

#include "../failure_handler.h"
#include <stdio.h>

// ----------------------------------------------------------------------------
void
cobc::rtos::Thread::wrapper(rtems_task_argument object)
{
	Thread* thread = reinterpret_cast<Thread *>(object);
	thread->run();

	// Returning from a RTEMS thread is a fatal error, nothing more to
	// do here than call the fatal error handler.
	rtos::FailureHandler::fatal(rtos::FailureCode::returnFromThread());
}

// ----------------------------------------------------------------------------
cobc::rtos::Thread::Thread(uint8_t priority, size_t stack,
		const char * name)
{
	rtems_name taskName = 0;
	if (name == 0)
	{
		// taskName = 0 is not allowed.
		taskName = rtems_build_name('T', 'H', 'D', '-');
	}
	else {
		for (uint_fast8_t i = 0; i < 4; ++i) {
			if (name != 0) {
				taskName |= *name++;
			}
			taskName <<= 8;
		}
	}

	rtems_status_code status = rtems_task_create(taskName, priority, stack,
			RTEMS_DEFAULT_MODES, RTEMS_DEFAULT_ATTRIBUTES, &tid);

	if (status != RTEMS_SUCCESSFUL) {
		rtos::FailureHandler::fatal(rtos::FailureCode::resourceAllocationFailed());
	}
}

cobc::rtos::Thread::~Thread()
{
	rtems_task_delete(tid);
}

// ----------------------------------------------------------------------------
void
cobc::rtos::Thread::start()
{
	rtems_task_start(tid, wrapper, reinterpret_cast<rtems_task_argument>(this));
}

// ----------------------------------------------------------------------------
void
cobc::rtos::Thread::setPriority(uint8_t priority)
{
	rtems_task_priority old;
	rtems_task_set_priority(tid, priority, &old);
}

uint8_t
cobc::rtos::Thread::getPriority() const
{
	rtems_task_priority priority;
	rtems_task_set_priority(tid, RTEMS_CURRENT_PRIORITY, &priority);

	return priority;
}
