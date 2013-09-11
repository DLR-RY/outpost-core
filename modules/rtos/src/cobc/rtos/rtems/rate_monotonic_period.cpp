/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include "rate_monotonic_period.h"

using namespace cobc::rtos;

RateMonotonicPeriod::RateMonotonicPeriod() :
	id()
{
	rtems_name name = rtems_build_name('P', 'E', 'R', 'D');
	rtems_status_code status = rtems_rate_monotonic_create(name, &id);

	if (status != RTEMS_SUCCESSFUL) {
		rtos::FailureHandler::fatal(rtos::FailureCode::resourceAllocationFailed());
	}
}

RateMonotonicPeriod::~RateMonotonicPeriod()
{
	rtems_status_code status = rtems_rate_monotonic_delete(id);
	if (status != RTEMS_SUCCESSFUL)	{
		// TODO create different error code?
		rtos::FailureHandler::fatal(rtos::FailureCode::resourceAllocationFailed());
	}
}
