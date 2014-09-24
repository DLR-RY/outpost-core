/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
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
}

#endif // COBC_RTOS_NONE_QUEUE_IMPL_H
