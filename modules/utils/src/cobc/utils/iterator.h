/*
 * Copyright (c) 2017, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.6.
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

#ifndef COBC_UTILS_ITERATOR_H
#define COBC_UTILS_ITERATOR_H

namespace cobc
{

// forward declaration
template <typename T>
struct ConstArrayIterator;

template <typename T>
struct DefaultConstArrayIterator;

template <typename T>
struct ArrayIterator
{
    explicit inline
    ArrayIterator(T* parameters);

    inline ArrayIterator(const ArrayIterator& other);

    inline ArrayIterator& operator=(const ArrayIterator& other);
    inline ArrayIterator& operator++();
    inline ArrayIterator& operator--();
    inline bool operator==(ArrayIterator other) const;
    inline bool operator!=(ArrayIterator other) const;

protected:
    friend struct ConstArrayIterator<T>;

    T* mPos;
};

template <typename T>
struct DefaultArrayIterator : public ArrayIterator<T>
{
    explicit inline
    DefaultArrayIterator(T* parameters);

    inline T& operator*();
    inline T* operator->();

private:
    friend struct DefaultConstArrayIterator<T>;
};


// ----------------------------------------------------------------------------
/**
 * Iterator for immutable arrays.
 *
 * \author  Fabian Greif
 */
template <typename T>
struct ConstArrayIterator
{
    explicit inline
    ConstArrayIterator(const T* parameters);

    inline ConstArrayIterator(const ArrayIterator<T>& other);
    inline ConstArrayIterator(const ConstArrayIterator& other);

    inline ConstArrayIterator& operator=(const ConstArrayIterator& other);
    inline ConstArrayIterator& operator++();
    inline ConstArrayIterator& operator--();
    inline bool operator==(ConstArrayIterator other) const;
    inline bool operator!=(ConstArrayIterator other) const;

protected:
    const T* mPos;
};

template <typename T>
struct DefaultConstArrayIterator : public ConstArrayIterator<T>
{
    explicit inline
    DefaultConstArrayIterator(const T* parameters);

    inline
    DefaultConstArrayIterator(const DefaultArrayIterator<T>& other);

    inline const T& operator*() const;
    inline const T* operator->() const;
};

}

#include "iterator_impl.h"

#endif
