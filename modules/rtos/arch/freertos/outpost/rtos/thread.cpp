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

#include "thread.h"

#include "thread_priorities.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <outpost/rtos/failure_handler.h>

#include <stdio.h>

/// Minimum stack size configured through the FreeRTOS configuration.
static const size_t minimumStackSize = configMINIMAL_STACK_SIZE * sizeof(portSTACK_TYPE);

// ----------------------------------------------------------------------------
void
outpost::rtos::Thread::wrapper(void* object)
{
    Thread* thread = reinterpret_cast<Thread*>(object);
    thread->run();

    // Returning from a FreeRTOS thread is a fatal error, nothing more to
    // do here than call the fatal error handler.
    rtos::FailureHandler::fatal(rtos::FailureCode::returnFromThread());
}

// ----------------------------------------------------------------------------
outpost::rtos::Thread::Thread(uint8_t priority,
                              size_t stack,
                              const char* name,
                              FloatingPointSupport /*floatingPointSupport*/) :
    mHandle(0),
    mPriority(priority),
    mStackSize(stack),
    mName(name)
{
    if (mStackSize < minimumStackSize)
    {
        mStackSize = minimumStackSize;
    }
}

outpost::rtos::Thread::~Thread()
{
    if (mHandle != 0)
    {
        vTaskDelete(mHandle);
    }
}

// ----------------------------------------------------------------------------
void
outpost::rtos::Thread::start()
{
    if (mHandle == 0)
    {
        int status = xTaskCreate(&Thread::wrapper,
                                 (const signed char*) mName,
                                 (mStackSize / sizeof(portSTACK_TYPE)) + 1,
                                 this,
                                 static_cast<unsigned portBASE_TYPE>(
                                         toFreeRtosPriority(mPriority, configMAX_PRIORITIES)),
                                 // 3,
                                 &mHandle);

        if (status != pdPASS)
        {
            FailureHandler::fatal(FailureCode::resourceAllocationFailed(Resource::thread));
        }
    }
}

// ----------------------------------------------------------------------------
outpost::rtos::Thread::Identifier
outpost::rtos::Thread::getIdentifier() const
{
    if (mHandle == 0)
    {
        return invalidIdentifier;
    }
    else
    {
        return reinterpret_cast<Identifier>(mHandle);
    }
}

outpost::rtos::Thread::Identifier
outpost::rtos::Thread::getCurrentThreadIdentifier()
{
    return reinterpret_cast<Identifier>(xTaskGetCurrentTaskHandle());
}

// ----------------------------------------------------------------------------
void
outpost::rtos::Thread::setPriority(uint8_t priority)
{
    vTaskPrioritySet(mHandle, toFreeRtosPriority(priority, configMAX_PRIORITIES));
}

uint8_t
outpost::rtos::Thread::getPriority() const
{
    return fromFreeRtosPriority(uxTaskPriorityGet(mHandle), configMAX_PRIORITIES);
}

// ----------------------------------------------------------------------------
void
outpost::rtos::Thread::yield()
{
    taskYIELD();
}

// ----------------------------------------------------------------------------
void
outpost::rtos::Thread::sleep(::outpost::time::Duration duration)
{
    vTaskDelay((duration.milliseconds() * configTICK_RATE_HZ) / 1000);
}

// ----------------------------------------------------------------------------
void
outpost::rtos::Thread::startScheduler()
{
#if !defined(GOMSPACE)
    vTaskStartScheduler();
    FailureHandler::fatal(FailureCode::returnFromThread());
#else
    FailureHandler::fatal(FailureCode::genericRuntimeError(Resource::thread));
#endif
}
