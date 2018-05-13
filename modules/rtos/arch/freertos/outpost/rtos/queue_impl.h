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

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

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
    vQueueDelete(mHandle);
}

template <typename T>
bool
outpost::rtos::Queue<T>::send(const T& data)
{
    const portTickType ticks = 0;
    return xQueueSend(mHandle, &data, ticks);
}

template <typename T>
bool
outpost::rtos::Queue<T>::receive(T& data, outpost::time::Duration timeout)
{
    const portTickType ticks = (timeout.milliseconds() * configTICK_RATE_HZ) / 1000;
    return xQueueReceive(mHandle, &data, ticks);
}

#endif
