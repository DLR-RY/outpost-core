/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#ifndef COBC_RTOS_FREERTOS_QUEUE_IMPL_H
#define COBC_RTOS_FREERTOS_QUEUE_IMPL_H

#include "queue.h"

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

#include <cobc/rtos/failure_handler.h>

template <typename T>
cobc::rtos::Queue<T>::Queue(size_t numberOfItems)
{
    handle = xQueueCreate(numberOfItems, sizeof(T));

    if (handle == 0)
    {
        rtos::FailureHandler::fatal(rtos::FailureCode::resourceAllocationFailed());
    }
}

template <typename T>
cobc::rtos::Queue<T>::~Queue()
{
    vQueueDelete(handle);
}

template <typename T>
bool
cobc::rtos::Queue<T>::send(const T& data)
{
    const portTickType ticks = 0;
    return xQueueSend(handle, &data, ticks);
}

template <typename T>
bool
cobc::rtos::Queue<T>::receive(T& data, cobc::time::Duration timeout)
{
    const portTickType ticks = (timeout.milliseconds() * configTICK_RATE_HZ) / 1000;
    return xQueueReceive(handle, &data, ticks);
}

#endif // COBC_RTOS_FREERTOS_QUEUE_IMPL_H
