/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include "thread.h"

#include <stdio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "thread_priorities.h"

#include <cobc/rtos/failure_handler.h>

/// Minimum stack size configured through the FreeRTOS configuration.
static const size_t minimumStackSize = configMINIMAL_STACK_SIZE * sizeof(portSTACK_TYPE);

// ----------------------------------------------------------------------------
void
cobc::rtos::Thread::wrapper(void* object)
{
    Thread* thread = reinterpret_cast<Thread *>(object);
    thread->run();

    // Returning from a FreeRTOS thread is a fatal error, nothing more to
    // do here than call the fatal error handler.
    rtos::FailureHandler::fatal(rtos::FailureCode::returnFromThread());
}

// ----------------------------------------------------------------------------
cobc::rtos::Thread::Thread(uint8_t priority, size_t stack, const char* name) :
    handle(0),
    priority(priority),
    stackSize(stack),
    name(name)
{
    if (stackSize < minimumStackSize) {
        stackSize = minimumStackSize;
    }
}

cobc::rtos::Thread::~Thread()
{
    if (handle != 0) {
        vTaskDelete(handle);
    }
}

// ----------------------------------------------------------------------------
void
cobc::rtos::Thread::start()
{
    if (handle == 0)
    {
        int status = xTaskCreate(
                &Thread::wrapper,
                (const signed char*) name,
                (stackSize / sizeof(portSTACK_TYPE)) + 1,
                this,
                static_cast<unsigned portBASE_TYPE>(toFreeRtosPriority(priority, configMAX_PRIORITIES)),
                //3,
                &handle);

        if (status != pdPASS) {
            rtos::FailureHandler::fatal(rtos::FailureCode::resourceAllocationFailed());
        }
    }
}

// ----------------------------------------------------------------------------
cobc::rtos::Thread::Identifier
cobc::rtos::Thread::getIdentifier() const
{
    if (handle == 0) {
        return invalidIdentifier;
    }
    else {
        return reinterpret_cast<Identifier>(handle);
    }
}

cobc::rtos::Thread::Identifier
cobc::rtos::Thread::getCurrentThreadIdentifier()
{
    return reinterpret_cast<Identifier>(xTaskGetCurrentTaskHandle());
}

// ----------------------------------------------------------------------------
void
cobc::rtos::Thread::setPriority(uint8_t priority)
{
    vTaskPrioritySet(handle, toFreeRtosPriority(priority, configMAX_PRIORITIES));
}

uint8_t
cobc::rtos::Thread::getPriority() const
{
    return fromFreeRtosPriority(uxTaskPriorityGet(handle), configMAX_PRIORITIES);
}

// ----------------------------------------------------------------------------
void
cobc::rtos::Thread::yield()
{
    taskYIELD();
}

// ----------------------------------------------------------------------------
void
cobc::rtos::Thread::sleep(::cobc::time::Duration duration)
{
    vTaskDelay((duration.milliseconds() * configTICK_RATE_HZ) / 1000);
}

// ----------------------------------------------------------------------------
void
cobc::rtos::Thread::startScheduler()
{
#if !defined(GOMSPACE)
    vTaskStartScheduler();
    rtos::FailureHandler::fatal(rtos::FailureCode::returnFromThread());
#else
    rtos::FailureHandler::fatal(rtos::FailureCode::genericRuntimeError());
#endif
}
