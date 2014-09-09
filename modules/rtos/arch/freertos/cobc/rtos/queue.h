/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------
#ifndef QUEUE_H_
#define QUEUE_H_

#include <cstddef>
#include <stdint.h>

#include <cobc/time/duration.h>

namespace cobc
{
    namespace rtos
    {
        /**
         * Queues.
         *
         * \author    Norbert Toth
         * \ingroup    rtos
         */
        class Queue
        {
            /**
             * Create a Queue.
             *
             * \param queueLength The maximum number of items that the queue can contain.
             * \param itemSize The number of bytes each item in the queue will require.
             */

            explicit
            Queue(size_t queueLength, size_t itemSize);

            /**
             * Destroy the queue.
             */
            ~Queue();

            /**
             *
             * Receive data from the queue.
             *
             * \param data Pointer to the buffer into which the received item will be copied.
             * \param timeout Timeout in milliseconds resolution.
             * \return
             */
            bool
            receive(void* data, time::Duration timeout);

            /**
             *
             * Send data to the queue.
             *
             * \param data A pointer to the item that is to be placed on the queue.
             * \param timeout Timeout in milliseconds resolution.
             * \return
             */
            bool
            send(const void* data, time::Duration timeout);

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

#endif // QUEUE_H_
