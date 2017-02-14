/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 *
 * This file is part of outpost 0.6.
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

#ifndef COBC_DEQUE_IMPL_H
#define COBC_DEQUE_IMPL_H

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
outpost::Deque<T>::Deque(outpost::BoundedArray<T> backendBuffer) :
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
