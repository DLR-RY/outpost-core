/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include "thread.h"

#include <cobc/rtos/failure_handler.h>
#include <stdio.h>

using cobc::rtos::Thread;

// ----------------------------------------------------------------------------
void
Thread::wrapper(rtems_task_argument object)
{
    Thread* thread = reinterpret_cast<Thread *>(object);
    thread->run();

    // Returning from a RTEMS thread is a fatal error, nothing more to
    // do here than call the fatal error handler.
    rtos::FailureHandler::fatal(rtos::FailureCode::returnFromThread());
}

// ----------------------------------------------------------------------------
// RTEMS supports priorities between 1..255. Lower values represent a higher
// priority, 1 is the highest and 255 the lowest priority.
//
// These RTEMS are priorities are mapped to 0..255 priority map with 0
// representing the lowest priority and 255 the highest. Because RTEMS
// has only 255 steps, both 0 and 1 represent the same priority.
//
static uint8_t
toRtemsPriority(uint8_t priority)
{
    if (priority == 0)
    {
        return 255;
    }

    return (256 - static_cast<int16_t>(priority));
}

static uint8_t
fromRtemsPriority(uint8_t rtemsPriority)
{
    // a RTEMS priority of 0 is invalid and not checked here.
    return (256 - static_cast<int16_t>(rtemsPriority));
}

// ----------------------------------------------------------------------------
Thread::Thread(uint8_t priority,
               size_t stack,
               const char* name)
{
    rtems_name taskName = 0;
    if (name == 0)
    {
        // taskName = 0 is not allowed.
        taskName = rtems_build_name('T', 'H', 'D', '-');
    }
    else
    {
        for (uint_fast8_t i = 0; i < 4; ++i)
        {
            taskName <<= 8;
            if (name != 0)
            {
                taskName |= *name++;
            }
        }
    }

    rtems_task_priority rtemsPriority = toRtemsPriority(priority);
    rtems_status_code status = rtems_task_create(taskName, rtemsPriority, stack,
            RTEMS_DEFAULT_MODES, RTEMS_DEFAULT_ATTRIBUTES, &tid);

    if (status != RTEMS_SUCCESSFUL)
    {
        rtos::FailureHandler::fatal(rtos::FailureCode::resourceAllocationFailed());
    }
}

Thread::~Thread()
{
    rtems_task_delete(tid);
}

// ----------------------------------------------------------------------------
Thread::Identifier
Thread::getIdentifier() const
{
    return tid;
}

Thread::Identifier
Thread::getCurrentThreadIdentifier()
{
    rtems_id current;
    if (rtems_task_ident(RTEMS_SELF, OBJECTS_SEARCH_LOCAL_NODE, &current) != RTEMS_SUCCESSFUL)
    {
        return 0;
    }

    return current;
}

// ----------------------------------------------------------------------------
void
Thread::start()
{
    rtems_task_start(tid, wrapper, reinterpret_cast<rtems_task_argument>(this));
}

// ----------------------------------------------------------------------------
void
Thread::setPriority(uint8_t priority)
{
    rtems_task_priority old;
    rtems_task_set_priority(tid, toRtemsPriority(priority), &old);
}

uint8_t
Thread::getPriority() const
{
    rtems_task_priority priority;
    rtems_task_set_priority(tid, RTEMS_CURRENT_PRIORITY, &priority);

    return fromRtemsPriority(priority);
}
