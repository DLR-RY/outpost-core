/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 * 
 * This file is part of libCOBC 0.3-pre.
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

#ifndef COBC_RTOS_RTEMS_QUEUE_IMPL_H
#define COBC_RTOS_RTEMS_QUEUE_IMPL_H

#include "queue.h"

#include <cobc/rtos/failure_handler.h>

template <typename T>
cobc::rtos::Queue<T>::Queue(size_t numberOfItems) :
    id()
{
    rtems_attribute attributes = RTEMS_FIFO | RTEMS_LOCAL;
    rtems_status_code result = rtems_message_queue_create("RTQ", numberOfItems, sizeof(T), attributes, &id);

    if (result != RTEMS_SUCCESSFUL)
    {
        rtos::FailureHandler::fatal(rtos::FailureCode::resourceAllocationFailed());
    }
}

template <typename T>
cobc::rtos::Queue<T>::~Queue()
{
    rtems_message_queue_delete(id);
}

template <typename T>
bool
cobc::rtos::Queue<T>::send(const T& data)
{
    rtems_status_code result = rtems_message_queue_send(id, &data, sizeof(T));
    bool success = (result == RTEMS_SUCCESSFUL);

    return success;
}

/**
 * Converts a duration into a RTEMS interval.
 *
 * An RTEMS interval needs to be at least 1 tick long, otherwise it would
 * specify an infinite interval.
 */
static inline rtems_interval
toRtemsInterval(cobc::time::Duration timeout)
{
    rtems_interval interval = timeout.milliseconds();
    if (interval < 1)
    {
        interval = 1;
    }
    return interval;
}

template <typename T>
bool
cobc::rtos::Queue<T>::receive(T& data, cobc::time::Duration timeout)
{
    size_t size;
    rtems_option options = RTEMS_WAIT;

    rtems_interval interval = 0;
    if (timeout != cobc::time::Duration::infinity())
    {
        interval = toRtemsInterval(timeout);
    }

    rtems_status_code result = rtems_message_queue_receive(id, &data, &size, options, interval);
    bool success = (result == RTEMS_SUCCESSFUL);

    return success;
}

#endif // COBC_RTOS_RTEMS_QUEUE_IMPL_H
