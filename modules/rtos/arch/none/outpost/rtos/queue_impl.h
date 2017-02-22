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

#ifndef OUTPOST_RTOS_NONE_QUEUE_IMPL_H
#define OUTPOST_RTOS_NONE_QUEUE_IMPL_H

#include "queue.h"

#include <outpost/rtos/failure_handler.h>

template <typename T>
outpost::rtos::Queue<T>::Queue(size_t numberOfItems) :
    buffer(new T[numberOfItems]),
    maximumSize(numberOfItems),
    itemsInBuffer(0),
    head(0),
    tail(0)
{
}

template <typename T>
outpost::rtos::Queue<T>::~Queue()
{
    delete[] buffer;
}

template <typename T>
bool
outpost::rtos::Queue<T>::send(const T& data)
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
outpost::rtos::Queue<T>::receive(T& data, outpost::time::Duration)
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

#endif
