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

#include "semaphore.h"

// ----------------------------------------------------------------------------
outpost::rtos::Semaphore::Semaphore(uint32_t count)
{
    (void) count;
}

outpost::rtos::Semaphore::~Semaphore()
{
}

// ----------------------------------------------------------------------------
bool
outpost::rtos::Semaphore::acquire()
{
    return true;
}

bool
outpost::rtos::Semaphore::acquire(time::Duration timeout)
{
    (void) timeout;

    return true;
}

void
outpost::rtos::Semaphore::release()
{
}

// ----------------------------------------------------------------------------
outpost::rtos::BinarySemaphore::BinarySemaphore(State::Type initial)
{
    (void) initial;
}

outpost::rtos::BinarySemaphore::~BinarySemaphore()
{
}

// ----------------------------------------------------------------------------
bool
outpost::rtos::BinarySemaphore::acquire()
{
    return true;
}

bool
outpost::rtos::BinarySemaphore::acquire(time::Duration timeout)
{
    (void) timeout;

    return true;
}

void
outpost::rtos::BinarySemaphore::release()
{
}
