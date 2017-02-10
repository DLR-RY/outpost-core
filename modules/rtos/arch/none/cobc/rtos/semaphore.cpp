/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.6.
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

#include "semaphore.h"

// ----------------------------------------------------------------------------
cobc::rtos::Semaphore::Semaphore(uint32_t count)
{
    (void) count;
}

cobc::rtos::Semaphore::~Semaphore()
{
}

// ----------------------------------------------------------------------------
bool
cobc::rtos::Semaphore::acquire()
{
    return true;
}

bool
cobc::rtos::Semaphore::acquire(time::Duration timeout)
{
    (void) timeout;

    return true;
}

void
cobc::rtos::Semaphore::release()
{
}

// ----------------------------------------------------------------------------
cobc::rtos::BinarySemaphore::BinarySemaphore(State::Type initial)
{
    (void) initial;
}

cobc::rtos::BinarySemaphore::~BinarySemaphore()
{
}

// ----------------------------------------------------------------------------
bool
cobc::rtos::BinarySemaphore::acquire()
{
    return true;
}

bool
cobc::rtos::BinarySemaphore::acquire(time::Duration timeout)
{
    (void) timeout;

    return true;
}

void
cobc::rtos::BinarySemaphore::release()
{
}
