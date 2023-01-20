/*
 * Copyright (c) 2014-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2014-2017, Fabian Greif (DLR RY-AVS)
 */

#ifndef OUTPOST_RTOS_FREERTOS_QUEUE_IMPL_H
#define OUTPOST_RTOS_FREERTOS_QUEUE_IMPL_H

#include "queue.h"

#include <outpost/rtos/failure_handler.h>

template <typename T>
outpost::rtos::Queue<T>::Queue(size_t numberOfItems)
{
    mHandle = xQueueCreate(numberOfItems, sizeof(T));

    if (mHandle == 0)
    {
        FailureHandler::fatal(FailureCode::resourceAllocationFailed(Resource::messageQueue));
    }
}

template <typename T>
outpost::rtos::Queue<T>::~Queue()
{
    vQueueDelete(static_cast<QueueHandle_t>(mHandle));
}

template <typename T>
bool
outpost::rtos::Queue<T>::send(const T& data)
{
    const TickType_t ticks = 0;
    return xQueueSend(mHandle, &data, ticks);
}

template <typename T>
bool
outpost::rtos::Queue<T>::sendFromISR(const T& data, bool& hasWokenTask)
{
    BaseType_t baseType = 0;
    bool res = xQueueSendFromISR(static_cast<QueueHandle_t>(mHandle), &data, &baseType);
    hasWokenTask = (baseType == 0);
    return res;
}

template <typename T>
bool
outpost::rtos::Queue<T>::receive(T& data, outpost::time::Duration timeout)
{
    const TickType_t ticks = timeout.milliseconds() > 1000 * 1000
                                     ? (timeout.milliseconds() * configTICK_RATE_HZ) / 1000
                                     : (timeout.milliseconds() / 1000) * configTICK_RATE_HZ;
    return xQueueReceive(static_cast<QueueHandle_t>(mHandle), &data, ticks);
}

template <typename T>
bool
outpost::rtos::Queue<T>::receiveFromISR(T& data, bool& hasWokenTask)
{
    BaseType_t baseType = 0;
    bool res = xQueueReceiveFromISR(static_cast<QueueHandle_t>(mHandle), &data, &baseType);
    hasWokenTask = (baseType != 0);
    return res;
}

#endif
