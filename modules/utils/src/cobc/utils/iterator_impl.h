/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.5.
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

#ifndef COBC_UTILS_ITERATOR_IMPL_H
#define COBC_UTILS_ITERATOR_IMPL_H

#include "iterator.h"

namespace cobc
{
// Implementation of inline functions

template <typename T>
ConstArrayIterator<T>::ConstArrayIterator(const T* parameters) :
    mPos(parameters)
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
const T&
DefaultConstArrayIterator<T>::operator*()
{
    return *this->mPos;
}

template <typename T>
const T*
DefaultConstArrayIterator<T>::operator->()
{
    return this->mPos;
}

}

#endif
