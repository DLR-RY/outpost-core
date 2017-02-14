/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 *
 * This file is part of outpost 0.6.
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

#include <outpost/rtos/failure_handler.h>
#include <stdio.h>

// ----------------------------------------------------------------------------
outpost::rtos::Thread::Thread(uint8_t priority,
                           size_t stack,
                           const char* name,
                           FloatingPointSupport /*floatingPointSupport*/)
{
    (void) priority;
    (void) stack;
    (void) name;
}

outpost::rtos::Thread::~Thread()
{
}

// ----------------------------------------------------------------------------
void
outpost::rtos::Thread::start()
{
}

// ----------------------------------------------------------------------------
outpost::rtos::Thread::Identifier
outpost::rtos::Thread::getIdentifier() const
{
    return 0;
}

outpost::rtos::Thread::Identifier
outpost::rtos::Thread::getCurrentThreadIdentifier()
{
    return 0;
}

// ----------------------------------------------------------------------------
void
outpost::rtos::Thread::setPriority(uint8_t priority)
{
    (void) priority;
}

uint8_t
outpost::rtos::Thread::getPriority() const
{
    return 0;
}
