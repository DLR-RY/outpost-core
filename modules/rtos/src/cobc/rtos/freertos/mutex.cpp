/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include "mutex.h"

#include "../failure_handler.h"

// ----------------------------------------------------------------------------
cobc::rtos::Mutex::Mutex()
{
	handle = xSemaphoreCreateMutex();

	if (handle == 0) {
		rtos::FailureHandler::fatal(rtos::FailureCode::resourceAllocationFailed());
	}
}

cobc::rtos::Mutex::~Mutex()
{
	vQueueDelete(handle);
}

// ----------------------------------------------------------------------------
bool
cobc::rtos::Mutex::acquire()
{
	// wait indefinitely
	return (xSemaphoreTake(handle, portMAX_DELAY) == pdTRUE);
}

bool
cobc::rtos::Mutex::acquire(uint32_t timeout)
{
	return (xSemaphoreTake(handle, (timeout * 1000) / portTICK_RATE_MS) == pdTRUE);
}
