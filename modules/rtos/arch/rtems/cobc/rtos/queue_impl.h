/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.5.
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
#include "rtems/interval.h"

#include <cobc/rtos/failure_handler.h>

template <typename T>
cobc::rtos::Queue<T>::Queue(size_t numberOfItems) :
    id()
{
    rtems_attribute attributes = RTEMS_FIFO | RTEMS_LOCAL;
    rtems_status_code result = rtems_message_queue_create("RTQ", numberOfItems, sizeof(T), attributes, &id);

    if (result != RTEMS_SUCCESSFUL)
    {
        FailureHandler::fatal(FailureCode::resourceAllocationFailed(Resource::messageQueue));
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

template <typename T>
bool
cobc::rtos::Queue<T>::receive(T& data, cobc::time::Duration timeout)
{
    size_t size;
    rtems_option options = RTEMS_WAIT;
    rtems_interval interval = rtems::getInterval(timeout);
    rtems_status_code result = rtems_message_queue_receive(id, &data, &size, options, interval);
    bool success = (result == RTEMS_SUCCESSFUL);

    return success;
}

#endif // COBC_RTOS_RTEMS_QUEUE_IMPL_H
