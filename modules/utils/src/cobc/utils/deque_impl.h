/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.4.
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
cobc::Deque<T>::Deque(T* backendBuffer,
                      size_t n) :
    mBuffer(backendBuffer),
    mMaxSize(n),
    mHead(0),
    mTail(1),
    mSize(0)
{
}

template<typename T>
cobc::Deque<T>::Deque(cobc::BoundedArray<T> backendBuffer) :
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
cobc::Deque<T>::isEmpty() const
{
    return (mSize == 0);
}

template<typename T>
bool
cobc::Deque<T>::isFull() const
{
    return (mSize == mMaxSize);
}

template<typename T>
typename cobc::Deque<T>::Size
cobc::Deque<T>::getSize() const
{
    return mSize;
}

template<typename T>
typename cobc::Deque<T>::Size
cobc::Deque<T>::getMaxSize() const
{
    return mMaxSize;
}

template<typename T>
typename cobc::Deque<T>::Size
cobc::Deque<T>::getAvailableSpace() const
{
    return (mMaxSize - mSize);
}

// ----------------------------------------------------------------------------
template<typename T>
void
cobc::Deque<T>::clear()
{
    mHead = 0;
    mTail = 1;
    mSize = 0;
}

// ----------------------------------------------------------------------------
template<typename T>
T&
cobc::Deque<T>::getFront()
{
    return mBuffer[mTail];
}


template<typename T>
const T&
cobc::Deque<T>::getFront() const
{
    return mBuffer[mTail];
}

// ----------------------------------------------------------------------------
template<typename T>
T&
cobc::Deque<T>::getBack()
{
    return mBuffer[mHead];
}

template<typename T>
const T&
cobc::Deque<T>::getBack() const
{
    return mBuffer[mHead];
}

// ----------------------------------------------------------------------------
template<typename T>
bool
cobc::Deque<T>::append(const T& value)
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
cobc::Deque<T>::removeBack()
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
cobc::Deque<T>::prepend(const T& value)
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
cobc::Deque<T>::removeFront()
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

#endif // COBC_DEQUE_IMPL_H
