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

#ifndef OUTPOST_RTOS_POSIX_QUEUE_IMPL_H
#define OUTPOST_RTOS_POSIX_QUEUE_IMPL_H

#include "internal/time.h"
#include "queue.h"

#include <outpost/rtos/failure_handler.h>

template <typename T>
outpost::rtos::Queue<T>::Queue(size_t numberOfItems) :
    mBuffer(new T[numberOfItems]),
    mMaximumSize(numberOfItems),
    mItemsInBuffer(0),
    mHead(0),
    mTail(0)
{
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&signal, NULL);
}

template <typename T>
outpost::rtos::Queue<T>::~Queue()
{
    pthread_mutex_lock(&mutex);

    mItemsInBuffer = 0;
    mHead = 0;
    mTail = 0;
    delete[] mBuffer;

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

    if (mItemsInBuffer < mMaximumSize)
    {
        mHead = increment(mHead);

        mBuffer[mHead] = data;
        mItemsInBuffer++;
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
    while ((mItemsInBuffer == 0) && !timeoutOccured)
    {
        if (pthread_cond_timedwait(&signal, &mutex, &time) != 0)
        {
            // Timeout or other error has occurred
            timeoutOccured = true;
        }
    }

    if (!timeoutOccured)
    {
        mTail = increment(mTail);

        data = mBuffer[mTail];
        mItemsInBuffer--;
        itemRetrieved = true;
    }

    pthread_mutex_unlock(&mutex);
    return itemRetrieved;
}

template <typename T>
size_t
outpost::rtos::Queue<T>::increment(size_t index) const
{
    if (index >= (mMaximumSize - 1))
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
