/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include "semaphore.h"

#include <time.h>
#include "../failure_handler.h"

// ----------------------------------------------------------------------------
cobc::rtos::Semaphore::Semaphore(uint32_t count) : sid()
{
	// shared semaphores are disabled
	if (sem_init(&sid, 0, count) != 0) {
		rtos::FailureHandler::fatal(rtos::FailureCode::resourceAllocationFailed());
	}
}

cobc::rtos::Semaphore::~Semaphore()
{
	sem_destroy(&sid);
}

bool
cobc::rtos::Semaphore::acquire(time::Duration timeout)
{
	uint64_t nanoseconds = timeout.microseconds() * 1000;

	const timespec t = {
		static_cast<time_t>(nanoseconds / 1000000000),	// seconds
		static_cast<long int>(nanoseconds % 1000000000)	// nanoseconds
	};

	return (sem_timedwait(&sid, &t) == 0);
}

// ----------------------------------------------------------------------------
cobc::rtos::BinarySemaphore::BinarySemaphore(State initial) :
	value(initial)
{
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&signal, NULL);
}

cobc::rtos::BinarySemaphore::~BinarySemaphore()
{

}

bool
cobc::rtos::BinarySemaphore::acquire()
{
	pthread_mutex_lock(&mutex);
	while (value == ACQUIRED) {
		pthread_cond_wait(&signal, &mutex);
	}
	value = ACQUIRED;
	pthread_mutex_unlock(&mutex);

	return true;
}

bool
cobc::rtos::BinarySemaphore::acquire(time::Duration timeout)
{
	uint64_t nanoseconds = timeout.microseconds() * 1000;

	// convert to absolute time for pthread_cond_timedwait()
	timespec time;
	clock_gettime(CLOCK_REALTIME, &time);

	time.tv_nsec += static_cast<time_t>(nanoseconds % 1000000000);
	if (time.tv_nsec >= 1000000000) {
		time.tv_sec += 1;
		time.tv_nsec = time.tv_nsec - 1000000000;
	}
	time.tv_sec += static_cast<long int>(nanoseconds / 1000000000);

	pthread_mutex_lock(&mutex);
	while (value == ACQUIRED) {
		if (pthread_cond_timedwait(&signal, &mutex, &time) != 0) {
			// Timeout or other error has occurred
			// => semaphore can't be acquired
			pthread_mutex_unlock(&mutex);
			return false;
		}
	}
	value = ACQUIRED;
	pthread_mutex_unlock(&mutex);

	return true;
}

void
cobc::rtos::BinarySemaphore::release()
{
	pthread_mutex_lock(&mutex);
	value = RELEASED;
	pthread_cond_signal(&signal);
	pthread_mutex_unlock(&mutex);
}
