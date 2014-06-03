/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

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
