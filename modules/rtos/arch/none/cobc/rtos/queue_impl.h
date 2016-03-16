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

#ifndef COBC_RTOS_NONE_QUEUE_IMPL_H
#define COBC_RTOS_NONE_QUEUE_IMPL_H

#include "queue.h"

#include <cobc/rtos/failure_handler.h>

template <typename T>
cobc::rtos::Queue<T>::Queue(size_t numberOfItems) :
    buffer(new T[numberOfItems]),
    maximumSize(numberOfItems),
    itemsInBuffer(0),
    head(0),
    tail(0)
{
}

template <typename T>
cobc::rtos::Queue<T>::~Queue()
{
    delete[] buffer;
}

template <typename T>
bool
cobc::rtos::Queue<T>::send(const T& data)
{
    bool itemStored = false;
    if (itemsInBuffer < maximumSize)
    {
        head = increment(head);

        buffer[head] = data;
        itemsInBuffer++;
        itemStored = true;
    }

    return itemStored;
}

template <typename T>
bool
cobc::rtos::Queue<T>::receive(T& data, cobc::time::Duration)
{
    bool itemRetrieved = false;
    if (itemsInBuffer > 0)
    {
        tail = increment(tail);

        data = buffer[tail];
        itemsInBuffer--;
        itemRetrieved = true;
    }

    return itemRetrieved;
}

template<typename T>
size_t
cobc::rtos::Queue<T>::increment(size_t index)
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

#endif // COBC_RTOS_NONE_QUEUE_IMPL_H
