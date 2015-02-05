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

#ifndef COBC_RTOS_FREERTOS_QUEUE_H
#define COBC_RTOS_FREERTOS_QUEUE_H

#include <stddef.h>
#include <stdint.h>

#include <cobc/time/duration.h>

namespace cobc
{
namespace rtos
{

/**
 * Atomic Queue.
 *
 * Can be used to exchange data between different threads.
 *
 * \author  Norbert Toth
 * \author  Fabian Greif
 * \ingroup rtos
 */
template <typename T>
class Queue
{
public:
    /**
     * Create a Queue.
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
     * \retval false    Queue is full, data could not be appended.
     */
    bool
    send(const T& data);

    /**
     * Receive data from the queue.
     *
     * \param data
     *      Reference to the buffer into which the received item will be copied.
     * \param timeout
     *      Timeout in milliseconds resolution.
     *
     * \retval true     Value was received correctly and put in \p data.
     * \retval false    Timeout occurred, \p data was not changed.
     */
    bool
    receive(T& data, cobc::time::Duration timeout = cobc::time::Duration::infinity());

private:
    // disable copy constructor
    Queue(const Queue& other);

    // disable assignment operator
    Queue&
    operator=(const Queue& other);

    void* handle;
};

}
}

#include "queue_impl.h"

#endif // COBC_RTOS_FREERTOS_QUEUE_H
