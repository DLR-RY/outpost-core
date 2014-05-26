/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include "thread.h"

// for the access to gettid
#include <sys/syscall.h>
#include <unistd.h>

#include "../failure_handler.h"

// ----------------------------------------------------------------------------
void *
cobc::rtos::Thread::wrapper(void * object)
{
	Thread* thread = reinterpret_cast<Thread *>(object);

	thread->tid = Thread::getCurrentThreadIdentifier();
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
	pthreadId(),
	tid()
{
}

cobc::rtos::Thread::~Thread()
{
	if (isRunning) {
		pthread_cancel(pthreadId);
		pthread_join(pthreadId, NULL);
	}
}

// ----------------------------------------------------------------------------
cobc::rtos::Thread::Identifier
cobc::rtos::Thread::getIdentifier() const
{
    return 0;
}

cobc::rtos::Thread::Identifier
cobc::rtos::Thread::getCurrentThreadIdentifier()
{
#ifdef SYS_gettid
    pid_t tid = syscall(SYS_gettid);
    return tid;
#else
#   error "SYS_gettid unavailable on this system"
#endif
}

// ----------------------------------------------------------------------------
void
cobc::rtos::Thread::start()
{
	isRunning = true;
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	int ret = pthread_create(&pthreadId, NULL, &Thread::wrapper, reinterpret_cast<void *>(this));
	if (ret != 0) {
	    rtos::FailureHandler::fatal(rtos::FailureCode::resourceAllocationFailed());
	}
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
