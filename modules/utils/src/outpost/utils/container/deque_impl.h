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
 * - 2013-2017, Fabian Greif (DLR RY-AVS)
 */
// ----------------------------------------------------------------------------

#ifndef OUTPOST_DEQUE_IMPL_H
#define OUTPOST_DEQUE_IMPL_H

#include "deque.h"

template<typename T>
outpost::Deque<T>::Deque(T* backendBuffer,
                      size_t n) :
    mBuffer(backendBuffer),
    mMaxSize(n),
    mHead(0),
    mTail(1),
    mSize(0)
{
}

template<typename T>
outpost::Deque<T>::Deque(outpost::Slice<T> backendBuffer) :
    mBuffer(backendBuffer.begin()),
    mMaxSize(backendBuffer.getNumberOfElements()),
    mHead(0),
    mTail(1),
    mSize(0)
{
}

// ----------------------------------------------------------------------------
template<typename T>
bool
outpost::Deque<T>::isEmpty() const
{
    return (mSize == 0);
}

template<typename T>
bool
outpost::Deque<T>::isFull() const
{
    return (mSize == mMaxSize);
}

template<typename T>
typename outpost::Deque<T>::Size
outpost::Deque<T>::getSize() const
{
    return mSize;
}

template<typename T>
typename outpost::Deque<T>::Size
outpost::Deque<T>::getMaxSize() const
{
    return mMaxSize;
}

template<typename T>
typename outpost::Deque<T>::Size
outpost::Deque<T>::getAvailableSpace() const
{
    return (mMaxSize - mSize);
}

// ----------------------------------------------------------------------------
template<typename T>
void
outpost::Deque<T>::clear()
{
    mHead = 0;
    mTail = 1;
    mSize = 0;
}

// ----------------------------------------------------------------------------
template<typename T>
T&
outpost::Deque<T>::getFront()
{
    return mBuffer[mTail];
}


template<typename T>
const T&
outpost::Deque<T>::getFront() const
{
    return mBuffer[mTail];
}

// ----------------------------------------------------------------------------
template<typename T>
T&
outpost::Deque<T>::getBack()
{
    return mBuffer[mHead];
}

template<typename T>
const T&
outpost::Deque<T>::getBack() const
{
    return mBuffer[mHead];
}

// ----------------------------------------------------------------------------
template<typename T>
bool
outpost::Deque<T>::append(const T& value)
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

template<typename T>
void
outpost::Deque<T>::removeBack()
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

// ----------------------------------------------------------------------------
template<typename T>
bool
outpost::Deque<T>::prepend(const T& value)
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

template<typename T>
void
outpost::Deque<T>::removeFront()
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

#endif
