/*
 * Copyright (c) 2013-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2013-2017, Fabian Greif (DLR RY-AVS)
 */

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

void __attribute__((weak)) outpost::rtos::Thread::sleep(::outpost::time::Duration duration)
{
    (void) duration;
}
