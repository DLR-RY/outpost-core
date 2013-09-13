/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include "periodic_task_manager.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "traits.h"

using namespace cobc::rtos;

PeriodicTaskManager::PeriodicTaskManager() :
	running(false),
	lastWakeTime(),
	currentPeriod()
{
}

PeriodicTaskManager::~PeriodicTaskManager()
{
}

PeriodicTaskManager::Status
PeriodicTaskManager::nextPeriod(time::Duration period)
{
	Status status = RUNNING;

	const portTickType nextPeriod = (period.milliseconds() * configTICK_RATE_HZ) / 1000;
	if (running) {
		if (static_cast<Traits<portTickType>::SignedType>(xTaskGetTickCount() - lastWakeTime) > currentPeriod) {
			status = TIMEOUT;
		}

		vTaskDelayUntil(&lastWakeTime, currentPeriod);
	}
	else {
		// period is started now, no need to wait
		lastWakeTime = xTaskGetTickCount();
		running = true;
	}

	currentPeriod = nextPeriod;
	return status;
}

PeriodicTaskManager::Status
PeriodicTaskManager::status()
{
	if (!running) {
		return IDLE;
	}
	else if (static_cast<Traits<portTickType>::SignedType>(xTaskGetTickCount() - lastWakeTime) > currentPeriod) {
		return TIMEOUT;
	}
	else {
		return RUNNING;
	}
}

void
PeriodicTaskManager::cancel()
{
	running = false;
}
