/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include "timer.h"

#include <cobc/rtos/failure_handler.h>

// ----------------------------------------------------------------------------
cobc::rtos::Timer::~Timer()
{
	rtems_timer_delete(tid);
}

// ----------------------------------------------------------------------------
void
cobc::rtos::Timer::start(time::Duration duration)
{
	rtems_timer_server_fire_after(tid,
      duration.microseconds()/rtems_configuration_get_microseconds_per_tick(),
      &Timer::invokeTimer,
      (void *) this);
}

void
cobc::rtos::Timer::reset()
{
	rtems_timer_reset(tid);
}

void
cobc::rtos::Timer::cancel()
{
	rtems_timer_cancel(tid);
}

// ----------------------------------------------------------------------------
void
cobc::rtos::Timer::startTimerDaemonThread(uint8_t priority, size_t stack)
{
	rtems_status_code result = rtems_timer_initiate_server(priority, stack, RTEMS_DEFAULT_ATTRIBUTES);
	if (result != RTEMS_SUCCESSFUL) {
		rtos::FailureHandler::fatal(rtos::FailureCode::resourceAllocationFailed());
	}
}

// ----------------------------------------------------------------------------
void
cobc::rtos::Timer::createTimer(const char* name)
{
	rtems_name taskName = 0;
	if (name == 0)
	{
		// taskName = 0 is not allowed.
		taskName = rtems_build_name('T', 'I', 'M', '-');;
	}
	else {
		for (uint_fast8_t i = 0; i < 4; ++i) {
			if (name != 0) {
				taskName |= *name++;
			}
			taskName <<= 8;
		}
	}

	rtems_status_code result = rtems_timer_create(taskName, &tid);
	if (result != RTEMS_SUCCESSFUL) {
		rtos::FailureHandler::fatal(rtos::FailureCode::resourceAllocationFailed());
	}
}

// ----------------------------------------------------------------------------
void
cobc::rtos::Timer::invokeTimer(rtems_id id, void * parameter)
{
	Timer * timer = reinterpret_cast<Timer *>(parameter);
	(timer->object->*(timer->function))(timer);
}
