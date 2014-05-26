/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include "thread.h"

#include "../failure_handler.h"
#include <stdio.h>

// ----------------------------------------------------------------------------
cobc::rtos::Thread::Thread(uint8_t priority, size_t stack,
		const char * name)
{
	(void) priority;
	(void) stack;
	(void) name;
}

cobc::rtos::Thread::~Thread()
{
}

// ----------------------------------------------------------------------------
void
cobc::rtos::Thread::start()
{
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
    return 0;
}

// ----------------------------------------------------------------------------
void
cobc::rtos::Thread::setPriority(uint8_t priority)
{
	(void) priority;
}

uint8_t
cobc::rtos::Thread::getPriority() const
{
	return 0;
}
