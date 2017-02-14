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

#include "thread.h"

#include <stdio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "thread_priorities.h"

#include <outpost/rtos/failure_handler.h>

/// Minimum stack size configured through the FreeRTOS configuration.
static const size_t minimumStackSize = configMINIMAL_STACK_SIZE * sizeof(portSTACK_TYPE);

// ----------------------------------------------------------------------------
void
outpost::rtos::Thread::wrapper(void* object)
{
    Thread* thread = reinterpret_cast<Thread *>(object);
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
        int status = xTaskCreate(
                &Thread::wrapper,
                (const signed char*) mName,
                (mStackSize / sizeof(portSTACK_TYPE)) + 1,
                this,
                static_cast<unsigned portBASE_TYPE>(toFreeRtosPriority(mPriority, configMAX_PRIORITIES)),
                //3,
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
