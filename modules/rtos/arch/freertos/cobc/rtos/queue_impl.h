/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
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
