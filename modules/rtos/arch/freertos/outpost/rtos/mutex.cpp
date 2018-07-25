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
 * - 2015, Jan Sommer (DLR SC-SRV)
 */

#include "mutex.h"

#include <outpost/rtos/failure_handler.h>

// ----------------------------------------------------------------------------
outpost::rtos::Mutex::Mutex()
{
    mHandle = xSemaphoreCreateRecursiveMutex();

    if (mHandle == 0)
    {
        FailureHandler::fatal(FailureCode::resourceAllocationFailed(Resource::mutex));
    }
}

outpost::rtos::Mutex::~Mutex()
{
    vQueueDelete(mHandle);
}

// ----------------------------------------------------------------------------
bool
outpost::rtos::Mutex::acquire()
{
    // wait indefinitely
    return (xSemaphoreTakeRecursive(mHandle, portMAX_DELAY) == pdTRUE);
}

bool
outpost::rtos::Mutex::acquire(time::Duration timeout)
{
    return (xSemaphoreTakeRecursive(mHandle, timeout.milliseconds() * configTICK_RATE_HZ / 1000)
            == pdTRUE);
}

void
outpost::rtos::Mutex::release()
{
    xSemaphoreGiveRecursive(mHandle);
}
