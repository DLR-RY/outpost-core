/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include "clock.h"

#include <time.h>

#ifdef UNITTEST
static cobc::time::TimePoint currentTime;
#endif

cobc::time::TimePoint
cobc::rtos::Clock::now()
{
#ifdef UNITTEST
	return currentTime;
#else
	struct timespec time;
	clock_gettime(CLOCK_MONOTONIC, &time);

	// convert to microseconds
	uint64_t microseconds = (time.tv_nsec / 1000) + (time.tv_sec * 1000000);

	return cobc::time::TimePoint(microseconds);
#endif
}

#ifdef UNITTEST
void
cobc::rtos::TestingClock::setTime(time::TimePoint time)
{
	currentTime = time;
}
#endif
