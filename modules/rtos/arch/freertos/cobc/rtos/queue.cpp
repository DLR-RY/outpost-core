/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------
#include "queue.h"
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

#include <cobc/rtos/failure_handler.h>

// ----------------------------------------------------------------------------

cobc::rtos::Queue::Queue(size_t queueLength, size_t itemSize)
{
    handle = xQueueCreate(queueLength, sizeof(itemSize));

    if (handle == 0)
    {
        rtos::FailureHandler::fatal(
                rtos::FailureCode::resourceAllocationFailed());
    }
}

// ----------------------------------------------------------------------------

cobc::rtos::Queue::~Queue()
{
    vQueueDelete(handle);
}

// ----------------------------------------------------------------------------

bool
cobc::rtos::Queue::receive(void* data, cobc::time::Duration timeout)
{
    const portTickType ticks = (timeout.milliseconds() * configTICK_RATE_HZ)
            / 1000;
    return xQueueReceive(handle, &data, ticks);
}

// ----------------------------------------------------------------------------

bool
cobc::rtos::Queue::send(const void* data, cobc::time::Duration timeout)
{
    const portTickType ticks = (timeout.milliseconds() * configTICK_RATE_HZ)
            / 1000;
    return xQueueSend(handle, data, ticks);
}
