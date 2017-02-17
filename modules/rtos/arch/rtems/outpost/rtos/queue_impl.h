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
 * - 2016, Jan-Gerd Mess (DLR RY-AVS)
 */
// ----------------------------------------------------------------------------

#ifndef COBC_RTOS_RTEMS_QUEUE_IMPL_H
#define COBC_RTOS_RTEMS_QUEUE_IMPL_H

#include "queue.h"
#include "rtems/interval.h"

#include <outpost/rtos/failure_handler.h>

template <typename T>
outpost::rtos::Queue<T>::Queue(size_t numberOfItems) :
    id()
{
    rtems_attribute attributes = RTEMS_FIFO | RTEMS_LOCAL;
    rtems_status_code result = rtems_message_queue_create(rtems_build_name('R', 'T', 'Q', '0'), numberOfItems, sizeof(T), attributes, &id);

    if (result != RTEMS_SUCCESSFUL)
    {
        FailureHandler::fatal(FailureCode::resourceAllocationFailed(Resource::messageQueue));
    }
}

template <typename T>
outpost::rtos::Queue<T>::~Queue()
{
    rtems_message_queue_delete(id);
}

template <typename T>
bool
outpost::rtos::Queue<T>::send(const T& data)
{
    rtems_status_code result = rtems_message_queue_send(id, &data, sizeof(T));
    bool success = (result == RTEMS_SUCCESSFUL);

    return success;
}

template <typename T>
bool
outpost::rtos::Queue<T>::receive(T& data, outpost::time::Duration timeout)
{
    size_t size;
    rtems_option options = RTEMS_WAIT;
    rtems_interval interval = rtems::getInterval(timeout);
    rtems_status_code result = rtems_message_queue_receive(id, &data, &size, options, interval);
    bool success = (result == RTEMS_SUCCESSFUL);

    return success;
}

#endif // COBC_RTOS_RTEMS_QUEUE_IMPL_H
