/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.3-pre.
 *
 * It is distributed under the terms of the GNU General Public License with
 * a linking exception. See the file "LICENSE" for the full license governing
 * this code.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "semaphore.h"

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include <cobc/rtos/failure_handler.h>

// ----------------------------------------------------------------------------
cobc::rtos::Semaphore::Semaphore(uint32_t count)
{
    handle = xSemaphoreCreateCounting(static_cast<unsigned portBASE_TYPE>(-1), count);
    if (handle == 0) {
        rtos::FailureHandler::fatal(rtos::FailureCode::resourceAllocationFailed());
    }
}

cobc::rtos::Semaphore::~Semaphore()
{
    // As semaphores are based on queues we use the queue functions to delete
    // the semaphore
    vQueueDelete(handle);
}

bool
cobc::rtos::Semaphore::acquire()
{
    // wait forever
    return (xSemaphoreTake(this->handle, portMAX_DELAY) == pdTRUE);
}

bool
cobc::rtos::Semaphore::acquire(time::Duration timeout)
{
    const portTickType ticks = (timeout.milliseconds() * configTICK_RATE_HZ) / 1000;
    return (xSemaphoreTake(this->handle, ticks) == pdTRUE);
}

void
cobc::rtos::Semaphore::release()
{
    xSemaphoreGive(handle);
}

// ----------------------------------------------------------------------------
cobc::rtos::BinarySemaphore::BinarySemaphore(State::Type initial)
{
    vSemaphoreCreateBinary(handle);

    if (handle == 0) {
        rtos::FailureHandler::fatal(rtos::FailureCode::resourceAllocationFailed());
    }

    if (initial == State::acquired) {
        acquire();
    }
}

cobc::rtos::BinarySemaphore::~BinarySemaphore()
{
    // As semaphores are based on queues we use the queue functions to delete
    // the semaphore
    vQueueDelete(handle);
}

bool
cobc::rtos::BinarySemaphore::acquire()
{
    // wait forever
    return (xSemaphoreTake(this->handle, portMAX_DELAY) == pdTRUE);
}

bool
cobc::rtos::BinarySemaphore::acquire(time::Duration timeout)
{
    const portTickType ticks = (timeout.milliseconds() * configTICK_RATE_HZ) / 1000;
    return (xSemaphoreTake(this->handle, ticks) == pdTRUE);
}

void
cobc::rtos::BinarySemaphore::release()
{
    xSemaphoreGive(handle);
}
