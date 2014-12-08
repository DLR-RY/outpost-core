/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.3-pre.
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
    buffer(backendBuffer),
    maxSize(n),
    head(0),
    tail(1),
    size(0)
{
}

// ----------------------------------------------------------------------------
template<typename T>
bool
cobc::Deque<T>::isEmpty() const
{
    return (size == 0);
}

template<typename T>
bool
cobc::Deque<T>::isFull() const
{
    return (size == maxSize);
}

template<typename T>
typename cobc::Deque<T>::Size
cobc::Deque<T>::getSize() const
{
    return size;
}

template<typename T>
typename cobc::Deque<T>::Size
cobc::Deque<T>::getMaxSize() const
{
    return maxSize;
}

template<typename T>
typename cobc::Deque<T>::Size
cobc::Deque<T>::getAvailableSpace() const
{
    return (maxSize - size);
}

// ----------------------------------------------------------------------------
template<typename T>
void
cobc::Deque<T>::clear()
{
    head = 0;
    tail = 1;
    size = 0;
}

// ----------------------------------------------------------------------------
template<typename T>
T&
cobc::Deque<T>::getFront()
{
    return buffer[tail];
}


template<typename T>
const T&
cobc::Deque<T>::getFront() const
{
    return buffer[tail];
}

// ----------------------------------------------------------------------------
template<typename T>
T&
cobc::Deque<T>::getBack()
{
    return buffer[head];
}

template<typename T>
const T&
cobc::Deque<T>::getBack() const
{
    return buffer[head];
}

// ----------------------------------------------------------------------------
template<typename T>
bool
cobc::Deque<T>::append(const T& value)
{
    bool result = false;
    if (!isFull())
    {
        if (head >= (maxSize - 1))
        {
            head = 0;
        }
        else
        {
            head++;
        }

        buffer[head] = value;
        size++;

        result = true;
    }

    return result;
}

template<typename T>
void
cobc::Deque<T>::removeBack()
{
    if (head == 0)
    {
        head = maxSize - 1;
    }
    else
    {
        head--;
    }
    size--;
}

// ----------------------------------------------------------------------------
template<typename T>
bool
cobc::Deque<T>::prepend(const T& value)
{
    bool result = false;
    if (!isFull())
    {
        if (tail == 0)
        {
            tail = maxSize - 1;
        }
        else
        {
            tail--;
        }

        buffer[tail] = value;
        size++;

        result = true;
    }

    return result;
}

template<typename T>
void
cobc::Deque<T>::removeFront()
{
    if (tail >= (maxSize - 1))
    {
        tail = 0;
    }
    else
    {
        tail++;
    }

    size--;
}

#endif // COBC_DEQUE_IMPL_H
