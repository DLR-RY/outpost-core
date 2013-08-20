/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include "thread.h"

#include "../failure_handler.h"

#include <stdio.h>

/// Minimum stack size configured through the FreeRTOS configuration.
static const std::size_t minimumStackSize = configMINIMAL_STACK_SIZE * sizeof(portSTACK_TYPE);

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
cobc::rtos::Thread::Thread(uint8_t priority, size_t stack, const char * name) :
	handle(0),
	priority(priority),
	stackSize(stack),
	name(name)
{
	if (stackSize < minimumStackSize) {
		stackSize = minimumStackSize;
	}
}

cobc::rtos::Thread::~Thread()
{
	if (handle != 0) {
		vTaskDelete(handle);
	}
}

// ----------------------------------------------------------------------------
void
cobc::rtos::Thread::start()
{
	if (handle == 0)
	{
		int status = xTaskCreate(
				&Thread::wrapper,
				(const signed char*) name,
				(stackSize / sizeof(portSTACK_TYPE)) + 1,
				this,
				static_cast<unsigned portBASE_TYPE>(priority),
				//3,
				&handle);

		if (status != pdPASS) {
			rtos::FailureHandler::fatal(rtos::FailureCode::resourceAllocationFailed());
		}
	}
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

// ----------------------------------------------------------------------------
void
cobc::rtos::Thread::sleep(::cobc::time::Duration duration)
{
	vTaskDelay((duration.milliseconds() * configTICK_RATE_HZ) / 1000.0);
}

// ----------------------------------------------------------------------------
void
cobc::rtos::Thread::startScheduler()
{
	//vTaskStartScheduler();

	rtos::FailureHandler::fatal(rtos::FailureCode::returnFromThread());
}
