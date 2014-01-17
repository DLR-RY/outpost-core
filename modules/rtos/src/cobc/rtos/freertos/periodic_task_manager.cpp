/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include "periodic_task_manager.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "../mutex.h"
#include "traits.h"

using namespace cobc::rtos;

PeriodicTaskManager::PeriodicTaskManager() :
	mutex(),
	timerRunning(false),
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
	MutexGuard lock(mutex);
	Status status = running;

	const portTickType nextPeriod = (period.milliseconds() * configTICK_RATE_HZ) / 1000;
	if (timerRunning) {
		if (static_cast<Traits<portTickType>::SignedType>(xTaskGetTickCount() -
				lastWakeTime) > currentPeriod) {
			status = timeout;
		}

		vTaskDelayUntil(&lastWakeTime, currentPeriod);
	}
	else {
		// period is started now, no need to wait
		lastWakeTime = xTaskGetTickCount();
		timerRunning = true;
	}

	currentPeriod = nextPeriod;
	return status;
}

PeriodicTaskManager::Status
PeriodicTaskManager::status()
{
	MutexGuard lock(mutex);

	if (!timerRunning) {
		return idle;
	}
	else if (static_cast<Traits<portTickType>::SignedType>(xTaskGetTickCount() -
			lastWakeTime) > currentPeriod) {
		return timeout;
	}
	else {
		return running;
	}
}

void
PeriodicTaskManager::cancel()
{
	MutexGuard lock(mutex);
	timerRunning = false;
}
