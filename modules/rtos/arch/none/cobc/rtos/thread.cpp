/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * 
 * This file is part of libCOBC 0.3-pre.
 *
 * It is distributed under the terms of the GNU General Public License with a
 * linking exception. See the file "LICENSE" for the full license governing
 * this code.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 */
// ----------------------------------------------------------------------------

#include "thread.h"

#include <cobc/rtos/failure_handler.h>
#include <stdio.h>

// ----------------------------------------------------------------------------
cobc::rtos::Thread::Thread(uint8_t priority, size_t stack,
        const char* name)
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
