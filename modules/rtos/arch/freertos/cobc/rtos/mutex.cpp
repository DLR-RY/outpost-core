/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * 
 * This file is part of libCOBC 0.4.
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

#include "mutex.h"

#include <cobc/rtos/failure_handler.h>

// ----------------------------------------------------------------------------
cobc::rtos::Mutex::Mutex()
{
    mHandle = xSemaphoreCreateMutex();

    if (mHandle == 0) {
        rtos::FailureHandler::fatal(rtos::FailureCode::resourceAllocationFailed());
    }
}

cobc::rtos::Mutex::~Mutex()
{
    vQueueDelete(mHandle);
}

// ----------------------------------------------------------------------------
bool
cobc::rtos::Mutex::acquire()
{
    // wait indefinitely
    return (xSemaphoreTake(mHandle, portMAX_DELAY) == pdTRUE);
}

bool
cobc::rtos::Mutex::acquire(time::Duration timeout)
{
    return (xSemaphoreTake(mHandle, timeout.milliseconds() * configTICK_RATE_HZ / 1000) == pdTRUE);
}

void
cobc::rtos::Mutex::release()
{
    xSemaphoreGive(mHandle);
}
