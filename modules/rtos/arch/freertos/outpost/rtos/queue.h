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
 * - 2014-2015, Norbert Toth (DLR RY-AVS)
 * - 2014-2017, Fabian Greif (DLR RY-AVS)
 */

#ifndef OUTPOST_RTOS_FREERTOS_QUEUE_H
#define OUTPOST_RTOS_FREERTOS_QUEUE_H

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

#include <outpost/time/duration.h>

#include <stddef.h>
#include <stdint.h>

#include <type_traits>

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
 *      Only works for POD types (see http://en.cppreference.com/w/cpp/concept/PODType)
 *      due to limitations of the used FreeRTOS queue which doesn't invoke
 *      constructors/destructors.
 *
 * \author  Norbert Toth
 * \author  Fabian Greif
 * \ingroup rtos
 */
template <typename T>
class Queue
{
    static_assert(std::is_trivial<T>::value && std::is_standard_layout<T>::value, "T must be POD");

public:
    /**
     * Create a Queue.
     *
     * \param numberOfItems
     *      The maximum number of items that the queue can contain.
     */
    explicit Queue(size_t numberOfItems);

    // disable copy constructor
    Queue(const Queue& other) = delete;

    // disable assignment operator
    Queue&
    operator=(const Queue& other) = delete;

    /**
     * Destroy the queue.
     */
    ~Queue();

    /**
     * Send data to the queue. May not be used from ISRs.
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
     * Send data to the queue. Only to be used from within ISRs.
     *
     * \param data
     *      Reference to the item that is to be placed on the queue.
     * \param hasWokenTask
     *      Is set to true if the send operation wakes a higher priority task.
     *      In that case, a yield should be executed before exiting the ISR.
     * \retval true     Value was successfully stored in the queue.
     * \retval false    Queue is full, data could not be appended.
     */
    bool
    sendFromISR(const T& data, bool& hasWokenTask);

    /**
     * Receive data from the queue. May not be used from ISRs.
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
    receive(T& data, outpost::time::Duration timeout);

    /**
     * Receive data from the queue. Only to be used from within ISRs.
     *
     * \param data
     *      Reference to the buffer into which the received item will be copied.
     * \param hasWokenTask
     *      Is set to true if the receive operation wakes a higher priority task.
     *      In that case, a yield should be executed before exiting the ISR.
     *
     * \retval true     Value was received correctly and put in \p data.
     * \retval false    No data available or some other error occured, hence \p data was not
     * changed.
     */
    bool
    receiveFromISR(T& data, bool& hasWokenTask);

private:
    QueueDefinition* mHandle;
};

}  // namespace rtos
}  // namespace outpost

#include "queue_impl.h"

#endif
