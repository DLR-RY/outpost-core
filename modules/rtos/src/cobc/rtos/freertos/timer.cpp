/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include "timer.h"

#include <cobc/rtos/failure_handler.h>

#include <freertos/task.h>

// ----------------------------------------------------------------------------
cobc::rtos::Timer::~Timer()
{
	if (xTimerDelete(handle, portMAX_DELAY) != pdPASS) {
		// TODO failureHandler
	}
}

// ----------------------------------------------------------------------------
void
cobc::rtos::Timer::start(time::Duration duration)
{
	if ((xTimerChangePeriod(handle,
	                        duration.milliseconds() * configTICK_RATE_HZ / 1000,
	                        portMAX_DELAY) != pdPASS) ||
		(xTimerStart(handle, portMAX_DELAY) != pdPASS))
	{
		// TODO failureHandler
	}
}

void
cobc::rtos::Timer::reset()
{
	if (xTimerReset(handle, portMAX_DELAY) != pdPASS) {
		// TODO failureHandler
	}
}

void
cobc::rtos::Timer::cancel()
{
	if (xTimerStop(handle, portMAX_DELAY) != pdPASS) {
		// TODO failureHandler
	}
}

// ----------------------------------------------------------------------------
void
cobc::rtos::Timer::startTimerDaemonThread(uint8_t priority, size_t stack)
{
	(void) priority;
	(void) stack;

	// Timer daemon is automatically started by including freertos/timers.c and
	// adjusting the following defines:
	//
	// - configUSE_TIMERS = 1
	// - configTIMER_TASK_PRIORITY
	// - configTIMER_QUEUE_LENGTH
	// - configTIMER_TASK_STACK_DEPTH
	//
	// see also:
	// http://www.freertos.org/Configuring-a-real-time-RTOS-application-to-use-software-timers.html
}

// ----------------------------------------------------------------------------
void
cobc::rtos::Timer::createTimer(const char* name)
{
	handle = xTimerCreate(reinterpret_cast<const signed char *>(name),
	                      0,
	                      pdFALSE,	// no auto-reload
	                      (void *) this,
	                      &Timer::invokeTimer);

	if (handle == 0) {
		rtos::FailureHandler::fatal(rtos::FailureCode::resourceAllocationFailed());
	}
}

// ----------------------------------------------------------------------------
void
cobc::rtos::Timer::invokeTimer(xTimerHandle handle)
{
	Timer * timer = reinterpret_cast<Timer *>(pvTimerGetTimerID(handle));
	(timer->object->*(timer->function))(timer);
}

