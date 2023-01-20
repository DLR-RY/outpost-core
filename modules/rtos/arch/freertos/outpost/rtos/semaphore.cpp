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
 * - 2013, Norbert Toth (DLR RY-AVS)
 */

#include "semaphore.h"

#include <outpost/rtos/failure_handler.h>

// ----------------------------------------------------------------------------
outpost::rtos::Semaphore::Semaphore(uint32_t count)
{
    mHandle = xSemaphoreCreateCounting(static_cast<unsigned portBASE_TYPE>(-1), count);
    if (mHandle == 0)
    {
        FailureHandler::fatal(FailureCode::resourceAllocationFailed(Resource::semaphore));
    }
}

outpost::rtos::Semaphore::~Semaphore()
{
    // As semaphores are based on queues we use the queue functions to delete
    // the semaphore
    vQueueDelete(mHandle);
}

bool
outpost::rtos::Semaphore::acquire()
{
    // wait forever
    return (xSemaphoreTake(mHandle, portMAX_DELAY) == pdTRUE);
}

bool
outpost::rtos::Semaphore::acquire(time::Duration timeout)
{
    if (timeout >= outpost::time::Duration::myriad())
    {
        return acquire();
    }
    const TickType_t ticks = (timeout.milliseconds() * configTICK_RATE_HZ) / 1000;
    return (xSemaphoreTake(mHandle, ticks) == pdTRUE);
}

bool
outpost::rtos::Semaphore::acquireFromISR(bool& hasWokenTask)
{
    // wait forever
    BaseType_t baseType = 0;
    bool res = (xSemaphoreTakeFromISR(this->mHandle, &baseType) == pdTRUE);

    hasWokenTask = (baseType != 0);
    return res;
}

void
outpost::rtos::Semaphore::release()
{
    xSemaphoreGive(mHandle);
}

void
outpost::rtos::Semaphore::releaseFromISR(bool& hasWokenTask)
{
    BaseType_t baseType = 0;
    xSemaphoreGiveFromISR(mHandle, &baseType);
    hasWokenTask = (baseType != 0);
}

// ----------------------------------------------------------------------------
outpost::rtos::BinarySemaphore::BinarySemaphore()
{
    vSemaphoreCreateBinary(mHandle);

    if (mHandle == 0)
    {
        FailureHandler::fatal(FailureCode::resourceAllocationFailed(Resource::semaphore));
    }
}

outpost::rtos::BinarySemaphore::BinarySemaphore(State::Type initial)
{
    vSemaphoreCreateBinary(mHandle);

    if (mHandle == 0)
    {
        FailureHandler::fatal(FailureCode::resourceAllocationFailed(Resource::semaphore));
    }

    if (initial == State::acquired)
    {
        acquire();
    }
}

outpost::rtos::BinarySemaphore::~BinarySemaphore()
{
    // As semaphores are based on queues we use the queue functions to delete
    // the semaphore
    vQueueDelete(mHandle);
}

bool
outpost::rtos::BinarySemaphore::acquire()
{
    // wait forever
    return (xSemaphoreTake(this->mHandle, portMAX_DELAY) == pdTRUE);
}

bool
outpost::rtos::BinarySemaphore::acquire(time::Duration timeout)
{
    const TickType_t ticks = (timeout.milliseconds() * configTICK_RATE_HZ) / 1000;
    return (xSemaphoreTake(this->mHandle, ticks) == pdTRUE);
}

bool
outpost::rtos::BinarySemaphore::acquireFromISR(bool& hasWokenTask)
{
    // wait forever
    BaseType_t baseType = 0;
    bool res = (xSemaphoreTakeFromISR(this->mHandle, &baseType) == pdTRUE);
    hasWokenTask = (baseType != 0);
    return res;
}

void
outpost::rtos::BinarySemaphore::release()
{
    xSemaphoreGive(mHandle);
}

void
outpost::rtos::BinarySemaphore::releaseFromISR(bool& hasWokenTask)
{
    BaseType_t baseType = 0;
    xSemaphoreGiveFromISR(mHandle, &baseType);
    hasWokenTask = (baseType != 0);
}
