/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include "thread.h"

#include <stdio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "../failure_handler.h"

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
/*
 * FreeRTOS supports priorities between 0 and (configMAX_PRIORITIES - 1).
 * Lower values represent a lower priority, 0 is the priority of the idle task
 * and the overall lowest priority.
 */
static const uint8_t stepWidth = 256 / configMAX_PRIORITIES;
static const uint8_t offset = (256 - configMAX_PRIORITIES  * stepWidth + stepWidth) / 2;

static uint8_t
toFreeRtosPriority(uint8_t priority)
{
	return (priority * stepWidth + offset);
}

static uint8_t
fromFreeRtosPriority(uint8_t freeRtosPriority)
{
	return ((freeRtosPriority - offset) / stepWidth);
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
				static_cast<unsigned portBASE_TYPE>(toFreeRtosPriority(priority)),
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
	vTaskPrioritySet(handle, toFreeRtosPriority(priority));
}

uint8_t
cobc::rtos::Thread::getPriority() const
{
	return fromFreeRtosPriority(uxTaskPriorityGet(handle));
}

// ----------------------------------------------------------------------------
void
cobc::rtos::Thread::yield()
{
	taskYIELD();
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
#if !defined(GOMSPACE)
	vTaskStartScheduler();
#endif
	rtos::FailureHandler::fatal(rtos::FailureCode::returnFromThread());
}
