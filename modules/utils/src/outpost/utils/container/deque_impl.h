/*
 * Copyright (c) 2013-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2013-2018, Fabian Greif (DLR RY-AVS)
 * - 2018, Olaf Maibaum (DLR SC-OSS)
 */

#ifndef OUTPOST_DEQUE_IMPL_H
#define OUTPOST_DEQUE_IMPL_H

#include "deque.h"

namespace outpost
{
template <typename T, DequeAppendStrategy strategy>
Deque<T, strategy>::Deque(T* backendBuffer, size_t n) :
    mBuffer(backendBuffer), mMaxSize(n), mHead(0), mTail(1), mSize(0)
{
}

template <typename T, DequeAppendStrategy strategy>
Deque<T, strategy>::Deque(outpost::Slice<T> backendBuffer) :
    mBuffer(backendBuffer.begin()),
    mMaxSize(backendBuffer.getNumberOfElements()),
    mHead(0),
    mTail(1),
    mSize(0)
{
}

template <typename T, DequeAppendStrategy strategy>
bool
Deque<T, strategy>::isEmpty() const
{
    return (mSize == 0);
}

template <typename T, DequeAppendStrategy strategy>
bool
Deque<T, strategy>::isFull() const
{
    return (mSize == mMaxSize);
}

template <typename T, DequeAppendStrategy strategy>
typename Deque<T, strategy>::Size
Deque<T, strategy>::getSize() const
{
    return mSize;
}

template <typename T, DequeAppendStrategy strategy>
typename Deque<T, strategy>::Size
Deque<T, strategy>::getMaxSize() const
{
    return mMaxSize;
}

template <typename T, DequeAppendStrategy strategy>
typename Deque<T, strategy>::Size
Deque<T, strategy>::getAvailableSpace() const
{
    return (mMaxSize - mSize);
}

template <typename T, DequeAppendStrategy strategy>
void
Deque<T, strategy>::clear()
{
    mHead = 0;
    mTail = 1;
    mSize = 0;
}

template <typename T, DequeAppendStrategy strategy>
T&
Deque<T, strategy>::getFront()
{
    return mBuffer[mTail];
}

template <typename T, DequeAppendStrategy strategy>
const T&
Deque<T, strategy>::getFront() const
{
    return mBuffer[mTail];
}

template <typename T, DequeAppendStrategy strategy>
T&
Deque<T, strategy>::getBack()
{
    return mBuffer[mHead];
}

template <typename T, DequeAppendStrategy strategy>
const T&
Deque<T, strategy>::getBack() const
{
    return mBuffer[mHead];
}

template <typename T, DequeAppendStrategy strategy>
bool
Deque<T, strategy>::append(const T& value)
{
    bool result = false;
    if (!isFull())
    {
        if (mHead >= (mMaxSize - 1))
        {
            mHead = 0;
        }
        else
        {
            mHead++;
        }

        mBuffer[mHead] = value;
        mSize++;

        result = true;
    }

    return result;
}

template <typename T, DequeAppendStrategy strategy>
size_t
Deque<T, strategy>::append(outpost::Slice<T> values)
{
    Size elementsToAppend = values.getNumberOfElements();
    if (elementsToAppend && !isFull())
    {
        // Get number of elements to copy until overflow happens
        if (elementsToAppend > (mMaxSize - mSize))
        {
            if (strategy == DequeAppendStrategy::partial)
            {
                elementsToAppend = mMaxSize - mSize;
            }
            else
            {
                // Do not append anything if not everything can be added.
                return 0;
            }
        }

        Index head = mHead + 1;

        // Get number of elements until end of ring buffer
        Size elementsUntilEndOfBuffer = mMaxSize - head;

        // Two cases:
        // 1. All elements has place in the end of the ring buffer, or fill up
        // all slots in the end and remaining elements should copy to the begin
        // of the ring buffer.
        if (elementsToAppend <= elementsUntilEndOfBuffer)
        {
            // Copy only to the end of the ring buffer
            appendMultipleElements(head, values.first(elementsToAppend));
            mHead = head + elementsToAppend - 1;
        }
        else
        {
            // 2. Split the elements to copy into two slices, one in the end,
            // other in the begin of array.
            appendMultipleElements(head, values.first(elementsUntilEndOfBuffer));
            appendMultipleElements(
                    0, values.first(elementsToAppend).skipFirst(elementsUntilEndOfBuffer));
            mHead = elementsToAppend - elementsUntilEndOfBuffer - 1;
        }

        mSize += elementsToAppend;
    }

    return elementsToAppend;
}

template <typename T, DequeAppendStrategy strategy>
void
Deque<T, strategy>::removeBack()
{
    if (mHead == 0)
    {
        mHead = mMaxSize - 1;
    }
    else
    {
        mHead--;
    }
    mSize--;
}

template <typename T, DequeAppendStrategy strategy>
bool
Deque<T, strategy>::prepend(const T& value)
{
    bool result = false;
    if (!isFull())
    {
        if (mTail == 0)
        {
            mTail = mMaxSize - 1;
        }
        else
        {
            mTail--;
        }

        mBuffer[mTail] = value;
        mSize++;

        result = true;
    }

    return result;
}

template <typename T, DequeAppendStrategy strategy>
void
Deque<T, strategy>::removeFront()
{
    if (mTail >= (mMaxSize - 1))
    {
        mTail = 0;
    }
    else
    {
        mTail++;
    }

    mSize--;
}
}  // namespace outpost

#endif
