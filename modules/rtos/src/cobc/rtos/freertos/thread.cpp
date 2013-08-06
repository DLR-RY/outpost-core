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
cobc::rtos::Thread::wrapper(void* object)
{
	Thread* thread = reinterpret_cast<Thread *>(object);
	thread->run();

	// Returning from a FreeRTOS thread is a fatal error, nothing more to
	// do here than call the fatal error handler.
	rtos::FailureHandler::fatal(rtos::FailureCode::returnFromThread());
}

// ----------------------------------------------------------------------------
cobc::rtos::Thread::Thread(uint8_t priority, size_t stack, const char * name)
{
	int status = xTaskCreate(
			&Thread::wrapper,
			(const signed char*) name,
			(stack / 4) + 1,
			this,
			priority,
			&handle);

	if (status != pdPASS) {
		rtos::FailureHandler::fatal(rtos::FailureCode::resourceAllocationFailed());
	}

	vTaskSuspend(handle);
}

cobc::rtos::Thread::~Thread()
{
	vTaskDelete(handle);
}

// ----------------------------------------------------------------------------
void
cobc::rtos::Thread::start()
{
	vTaskResume(handle);
}

// ----------------------------------------------------------------------------
void
cobc::rtos::Thread::setPriority(uint8_t priority)
{
	vTaskPrioritySet(handle, priority);
}

uint8_t
cobc::rtos::Thread::getPriority() const
{
	return uxTaskPriorityGet(handle);
}
