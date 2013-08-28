/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include "timer.h"

#include <cobc/rtos/failure_handler.h>

#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>
#include <freertos/task.h>

// ----------------------------------------------------------------------------
cobc::rtos::Timer::~Timer()
{
	if (xTimerDelete(handle, portMAX_DELAY) != pdPASS) {
		rtos::FailureHandler::fatal(rtos::FailureCode::resourceAllocationFailed());
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
		rtos::FailureHandler::fatal(rtos::FailureCode::genericRuntimeError());
	}
}

void
cobc::rtos::Timer::reset()
{
	if (xTimerReset(handle, portMAX_DELAY) != pdPASS) {
		rtos::FailureHandler::fatal(rtos::FailureCode::genericRuntimeError());
	}
}

void
cobc::rtos::Timer::cancel()
{
	if (xTimerStop(handle, portMAX_DELAY) != pdPASS) {
		rtos::FailureHandler::fatal(rtos::FailureCode::genericRuntimeError());
	}
}

// ----------------------------------------------------------------------------
void
cobc::rtos::Timer::startTimerDaemonThread(uint8_t priority, size_t stack)
{
	(void) priority;
	(void) stack;
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
cobc::rtos::Timer::invokeTimer(void* handle)
{
	Timer * timer = reinterpret_cast<Timer *>(pvTimerGetTimerID(handle));
	(timer->object->*(timer->function))(timer);
}
