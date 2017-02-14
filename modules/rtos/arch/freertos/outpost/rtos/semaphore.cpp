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

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

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
    const portTickType ticks = (timeout.milliseconds() * configTICK_RATE_HZ) / 1000;
    return (xSemaphoreTake(mHandle, ticks) == pdTRUE);
}

void
outpost::rtos::Semaphore::release()
{
    xSemaphoreGive(mHandle);
}

// ----------------------------------------------------------------------------
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
    const portTickType ticks = (timeout.milliseconds() * configTICK_RATE_HZ) / 1000;
    return (xSemaphoreTake(this->mHandle, ticks) == pdTRUE);
}

void
outpost::rtos::BinarySemaphore::release()
{
    xSemaphoreGive(mHandle);
}
