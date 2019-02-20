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

#ifndef OUTPOST_RTOS_NONE_QUEUE_IMPL_H
#define OUTPOST_RTOS_NONE_QUEUE_IMPL_H

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
}

template <typename T>
outpost::rtos::Queue<T>::~Queue()
{
    delete[] mBuffer;
}

template <typename T>
bool
outpost::rtos::Queue<T>::send(const T& data)
{
    bool itemStored = false;
    if (mItemsInBuffer < mMaximumSize)
    {
        mHead = increment(mHead);

        mBuffer[mHead] = data;
        mItemsInBuffer++;
        itemStored = true;
    }

    return itemStored;
}

template <typename T>
bool
outpost::rtos::Queue<T>::receive(T& data, outpost::time::Duration)
{
    bool itemRetrieved = false;
    if (mItemsInBuffer > 0)
    {
        mTail = increment(mTail);

        data = mBuffer[mTail];
        mItemsInBuffer--;
        itemRetrieved = true;
    }

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
