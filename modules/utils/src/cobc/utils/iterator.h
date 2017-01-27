/*
 * Copyright (c) 2017, German Aerospace Center (DLR)
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

#ifndef COBC_UTILS_ITERATOR_H
#define COBC_UTILS_ITERATOR_H

namespace cobc
{

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

    inline const T& operator*();
    inline const T* operator->();
};

}

#include "iterator_impl.h"

#endif
