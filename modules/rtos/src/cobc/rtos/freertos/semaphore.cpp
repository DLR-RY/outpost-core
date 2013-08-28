/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include "semaphore.h"

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include "../failure_handler.h"

// ----------------------------------------------------------------------------
cobc::rtos::Semaphore::Semaphore(uint32_t count)
{
	handle = xSemaphoreCreateCounting(static_cast<unsigned portBASE_TYPE>(-1), count);
	if (handle == 0) {
		rtos::FailureHandler::fatal(rtos::FailureCode::resourceAllocationFailed());
	}
}

cobc::rtos::Semaphore::~Semaphore()
{
	// As semaphores are based on queues we use the queue functions to delete
	// the semaphore
	vQueueDelete(handle);
}

bool
cobc::rtos::Semaphore::acquire()
{
	// wait forever
	return (xSemaphoreTake(this->handle, portMAX_DELAY) == pdTRUE);
}

bool
cobc::rtos::Semaphore::acquire(time::Duration timeout)
{
	const portTickType ticks = (timeout.milliseconds() * 1000) / portTICK_RATE_MS;
	return (xSemaphoreTake(this->handle, ticks) == pdTRUE);
}

void
cobc::rtos::Semaphore::release()
{
	xSemaphoreGive(handle);
}

// ----------------------------------------------------------------------------
cobc::rtos::BinarySemaphore::BinarySemaphore(bool initial)
{
	vSemaphoreCreateBinary(handle);

	if (handle == 0) {
		rtos::FailureHandler::fatal(rtos::FailureCode::resourceAllocationFailed());
	}

	if (!initial) {
		acquire();
	}
}

cobc::rtos::BinarySemaphore::~BinarySemaphore()
{
	// As semaphores are based on queues we use the queue functions to delete
	// the semaphore
	vQueueDelete(handle);
}

bool
cobc::rtos::BinarySemaphore::acquire()
{
	// wait forever
	return (xSemaphoreTake(this->handle, portMAX_DELAY) == pdTRUE);
}

bool
cobc::rtos::BinarySemaphore::acquire(time::Duration timeout)
{
	const portTickType ticks = (timeout.milliseconds() * 1000) / portTICK_RATE_MS;
	return (xSemaphoreTake(this->handle, ticks) == pdTRUE);
}

void
cobc::rtos::BinarySemaphore::release()
{
	xSemaphoreGive(handle);
}
