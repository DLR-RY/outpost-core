/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include "rate_monotonic_period.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "traits.h"

using namespace cobc::rtos;

RateMonotonicPeriod::RateMonotonicPeriod() :
	running(false),
	lastWakeTime(),
	currentPeriod()
{
}

RateMonotonicPeriod::~RateMonotonicPeriod()
{
}

RateMonotonicPeriod::Status
RateMonotonicPeriod::nextPeriod(time::Duration period)
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

RateMonotonicPeriod::Status
RateMonotonicPeriod::status()
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
RateMonotonicPeriod::cancel()
{
	running = false;
}
