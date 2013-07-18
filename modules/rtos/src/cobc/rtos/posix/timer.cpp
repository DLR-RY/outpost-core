/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include "timer.h"

#include <cstring>
#include "../failure_handler.h"

// ----------------------------------------------------------------------------
cobc::rtos::Timer::~Timer()
{
	// TODO generates a Segementation fault?
	//timer_delete(tid);
}

// ----------------------------------------------------------------------------
void
cobc::rtos::Timer::start(time::Duration duration)
{
	struct itimerspec time;
	uint64_t nanoseconds = duration.microseconds() * 1000;

	// interval
	time.it_interval.tv_sec = 0;
	time.it_interval.tv_nsec = 0;

	// initial expiration
	time.it_value.tv_sec = static_cast<time_t>(nanoseconds / 1000000000);// seconds
	time.it_value.tv_nsec = static_cast<long int>(nanoseconds % 1000000000);// nanoseconds

	std::memcpy(&interval, &time, sizeof(struct itimerspec));

	if (timer_settime(tid, 0, &time, NULL) != 0) {
		// Could not set the timer value
		FailureHandler::fatal(FailureCode::resourceAllocationFailed());
	}
}

void
cobc::rtos::Timer::reset()
{
	if (timer_settime(tid, 0, &interval, NULL) != 0) {
		// Could not set the timer value
		FailureHandler::fatal(FailureCode::resourceAllocationFailed());
	}
}

/*
 * Disarm timer by
 */
void
cobc::rtos::Timer::cancel()
{
	struct itimerspec time;

	// interval
	time.it_interval.tv_sec = 0;
	time.it_interval.tv_nsec = 0;

	// initial expiration
	time.it_value.tv_sec = 0;
	time.it_value.tv_nsec = 0;

	if (timer_settime(tid, 0, &time, NULL) != 0) {
		// Could not set the timer value
		FailureHandler::fatal(FailureCode::resourceAllocationFailed());
	}
}

// ----------------------------------------------------------------------------
void
cobc::rtos::Timer::startTimerDaemonThread(uint8_t priority, size_t stack)
{
	// not used
	(void) priority;
	(void) stack;
}

// ----------------------------------------------------------------------------
void
cobc::rtos::Timer::createTimer(const char* name)
{
	(void) name;

	struct sigevent event;

	// Set the sigevent structure to cause the signal to be
	// delivered by creating a new thread.
	event.sigev_notify = SIGEV_THREAD;
	event.sigev_value.sival_ptr = this;
	event.sigev_notify_function = Timer::invokeTimer;
	event.sigev_notify_attributes = NULL;

	if (timer_create(CLOCK_MONOTONIC, &event, &tid) != 0) {
		// Could not allocate a new timer
		FailureHandler::fatal(FailureCode::resourceAllocationFailed());
	}
}

// ----------------------------------------------------------------------------
void
cobc::rtos::Timer::invokeTimer(union sigval parameter)
{
	Timer * timer = reinterpret_cast<Timer *>(parameter.sival_ptr);
	(timer->object->*(timer->function))(timer);
}
