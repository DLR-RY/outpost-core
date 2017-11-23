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
// ----------------------------------------------------------------------------

#ifndef OUTPOST_RTOS_RTEMS_QUEUE_H
#define OUTPOST_RTOS_RTEMS_QUEUE_H

#include <stddef.h>
#include <stdint.h>
#include <type_traits>

#include <outpost/time/duration.h>

namespace outpost
{
namespace rtos
{

/**
 * Atomic Queue.
 *
 * Can be used to exchange data between different threads.
 *
 * \warning
 *      Limited to POD types (see http://en.cppreference.com/w/cpp/concept/PODType)
 *      for compatibility with the FreeRTOS and RTEMS implementations.
 *
 * \author  Fabian Greif
 * \ingroup rtos
 */
template <typename T>
class Queue
{
    static_assert(std::is_pod<T>::value, "T must be POD");

public:
    /**
     * Create a Queue.
     *
     * Uses \p new to allocate the storage space for the queue.
     *
     * \param numberOfItems
     *      The maximum number of items that the queue can contain.
     */
    Queue(size_t numberOfItems);

    /**
     * Destroy the queue.
     */
    ~Queue();

    /**
     * Send data to the queue.
     *
     * \param data
     *      Reference to the item that is to be placed on the queue.
     *
     * \retval true     Value was successfully stored in the queue.
     * \retval false    Queue is full and data could not be appended.
     */
    bool
    send(const T& data);

    /**
     * Receive data from the queue.
     *
     * \param data
     *      Reference to the buffer into which the received item will be copied.
     * \param timeout
     *      Ignored. Function always returns immediately.
     *
     * \retval true     Value was received correctly and put in \p data.
     * \retval false    Queue empty, no data was retrieved.
     */
    bool
    receive(T& data, outpost::time::Duration timeout = outpost::time::Duration::infinity());

private:
    // disable copy constructor
    Queue(const Queue& other);

    // disable assignment operator
    Queue&
    operator=(const Queue& other);

    size_t
    increment(size_t index);

    T* buffer;
    const size_t maximumSize;
    size_t itemsInBuffer;
    size_t head;
    size_t tail;
};

}
}

#include "queue_impl.h"

#endif
