/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include "thread.h"

#include "../failure_handler.h"

// ----------------------------------------------------------------------------
void *
cobc::rtos::Thread::wrapper(void * object)
{
	Thread* thread = reinterpret_cast<Thread *>(object);
	thread->run();

	// Returning from a thread is a fatal error, nothing more to
	// do here than call the fatal error handler.
	rtos::FailureHandler::fatal(rtos::FailureCode::returnFromThread());

	pthread_exit(0);
	return NULL;
}

// ----------------------------------------------------------------------------
cobc::rtos::Thread::Thread(uint8_t, size_t, const char *) :
	isRunning(false),
	tid()
{
}

cobc::rtos::Thread::~Thread()
{
	if (isRunning) {
		pthread_cancel(tid);
		pthread_join(tid, NULL);
	}
}

// ----------------------------------------------------------------------------
void
cobc::rtos::Thread::start()
{
	isRunning = true;
	pthread_create(&tid, NULL, &Thread::wrapper, reinterpret_cast<void *>(this));
}

// ----------------------------------------------------------------------------
void
cobc::rtos::Thread::setPriority(uint8_t priority)
{
	// No priority defined for POSIX!
	(void) priority;
}

uint8_t
cobc::rtos::Thread::getPriority() const
{
	return 0;
}
