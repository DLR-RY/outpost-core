/*
 * Copyright (c) 2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2017, Fabian Greif (DLR RY-AVS)
 */
// ----------------------------------------------------------------------------

#ifndef OUTPOST_UTILS_ITERATOR_IMPL_H
#define OUTPOST_UTILS_ITERATOR_IMPL_H

#include "iterator.h"

namespace outpost
{
// Implementation of inline functions
template <typename T>
ArrayIterator<T>::ArrayIterator(T* parameters) :
    mPos(parameters)
{
}

template <typename T>
ArrayIterator<T>::ArrayIterator(const ArrayIterator& other) :
    mPos(other.mPos)
{
}

template <typename T>
ArrayIterator<T>&
ArrayIterator<T>::operator=(const ArrayIterator& other)
{
    mPos = other.mPos;
    return *this;
}

template <typename T>
ArrayIterator<T>&
ArrayIterator<T>::operator++()
{
    ++mPos;
    return *this;
}

template <typename T>
ArrayIterator<T>&
ArrayIterator<T>::operator--()
{
    --mPos;
    return *this;
}

template <typename T>
bool
ArrayIterator<T>::operator==(ArrayIterator other) const
{
    return (mPos == other.mPos);
}

template <typename T>
bool
ArrayIterator<T>::operator!=(ArrayIterator other) const
{
    return (mPos != other.mPos);
}

// ----------------------------------------------------------------------------
template <typename T>
DefaultArrayIterator<T>::DefaultArrayIterator(T* parameters) :
    ArrayIterator<T>(parameters)
{
}

template <typename T>
T&
DefaultArrayIterator<T>::operator*()
{
    return *this->mPos;
}

template <typename T>
T*
DefaultArrayIterator<T>::operator->()
{
    return this->mPos;
}

// ----------------------------------------------------------------------------
template <typename T>
ConstArrayIterator<T>::ConstArrayIterator(const T* parameters) :
    mPos(parameters)
{
}

template <typename T>
ConstArrayIterator<T>::ConstArrayIterator(const ArrayIterator<T>& other) :
    mPos(other.mPos)
{
}

template <typename T>
ConstArrayIterator<T>::ConstArrayIterator(const ConstArrayIterator& other) :
    mPos(other.mPos)
{
}

template <typename T>
ConstArrayIterator<T>&
ConstArrayIterator<T>::operator=(const ConstArrayIterator& other)
{
    mPos = other.mPos;
    return *this;
}

template <typename T>
ConstArrayIterator<T>&
ConstArrayIterator<T>::operator++()
{
    ++mPos;
    return *this;
}

template <typename T>
ConstArrayIterator<T>&
ConstArrayIterator<T>::operator--()
{
    --mPos;
    return *this;
}

template <typename T>
bool
ConstArrayIterator<T>::operator==(ConstArrayIterator other) const
{
    return (mPos == other.mPos);
}

template <typename T>
bool
ConstArrayIterator<T>::operator!=(ConstArrayIterator other) const
{
    return (mPos != other.mPos);
}

// ----------------------------------------------------------------------------
template <typename T>
DefaultConstArrayIterator<T>::DefaultConstArrayIterator(const T* parameters) :
    ConstArrayIterator<T>(parameters)
{
}

template <typename T>
DefaultConstArrayIterator<T>::DefaultConstArrayIterator(const DefaultArrayIterator<T>& other) :
    ConstArrayIterator<T>(other)
{
}

template <typename T>
const T&
DefaultConstArrayIterator<T>::operator*() const
{
    return *this->mPos;
}

template <typename T>
const T*
DefaultConstArrayIterator<T>::operator->() const
{
    return this->mPos;
}

}

#endif
