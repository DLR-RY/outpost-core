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

#ifndef COBC_RTOS_POSIX_QUEUE_IMPL_H
#define COBC_RTOS_POSIX_QUEUE_IMPL_H

#include "queue.h"

#include <outpost/rtos/failure_handler.h>

#include "internal/time.h"

template <typename T>
outpost::rtos::Queue<T>::Queue(size_t numberOfItems) :
    buffer(new T[numberOfItems]),
    maximumSize(numberOfItems),
    itemsInBuffer(0),
    head(0),
    tail(0)
{
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&signal, NULL);
}

template <typename T>
outpost::rtos::Queue<T>::~Queue()
{
    pthread_mutex_lock(&mutex);

    itemsInBuffer = 0;
    head = 0;
    tail = 0;
    delete[] buffer;

    pthread_mutex_unlock(&mutex);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&signal);
}

template <typename T>
bool
outpost::rtos::Queue<T>::send(const T& data)
{
    bool itemStored = false;
    pthread_mutex_lock(&mutex);

    if (itemsInBuffer < maximumSize)
    {
        head = increment(head);

        buffer[head] = data;
        itemsInBuffer++;
        itemStored = true;

        pthread_cond_signal(&signal);
    }

    pthread_mutex_unlock(&mutex);
    return itemStored;
}

template <typename T>
bool
outpost::rtos::Queue<T>::receive(T& data, outpost::time::Duration timeout)
{
    bool itemRetrieved = false;
    bool timeoutOccured = false;
    timespec time = toAbsoluteTime(timeout);

    pthread_mutex_lock(&mutex);
    while ((itemsInBuffer == 0) && !timeoutOccured)
    {
        if (pthread_cond_timedwait(&signal, &mutex, &time) != 0)
        {
            // Timeout or other error has occurred
            timeoutOccured = true;
        }
    }

    if (!timeoutOccured)
    {
        tail = increment(tail);

        data = buffer[tail];
        itemsInBuffer--;
        itemRetrieved = true;
    }

    pthread_mutex_unlock(&mutex);
    return itemRetrieved;
}

template<typename T>
size_t
outpost::rtos::Queue<T>::increment(size_t index)
{
    if (index >= (maximumSize - 1))
    {
        index = 0;
    }
    else
    {
        index++;
    }

    return index;
}

#endif // COBC_RTOS_POSIX_QUEUE_IMPL_H
