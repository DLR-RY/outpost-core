/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include "mutex.h"

#include <time.h>

using namespace cobc::rtos;

bool
Mutex::acquire(cobc::time::Duration timeout)
{
	struct timespec time;
	uint64_t nanoseconds = timeout.microseconds() * 1000;

	// seconds
	time.tv_sec = static_cast<time_t>(nanoseconds / 1000000000);

	// nanoseconds
	time.tv_nsec = static_cast<long int>(nanoseconds % 1000000000);

	return (pthread_mutex_timedlock(&mutex, &time) == 0);
}
