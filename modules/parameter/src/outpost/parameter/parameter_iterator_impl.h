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
 * - 2020, Jan Malburg (DLR RY-AVS)
 */

#ifndef OUTPOST_PARAMETER_PARAMETER_ITERATOR_IMPL_H_
#define OUTPOST_PARAMETER_PARAMETER_ITERATOR_IMPL_H_

#include "parameter_iterator.h"

namespace outpost
{
namespace parameter
{
template <typename Friend>
inline bool
ParameterIterator<Friend>::operator==(const ParameterIterator& o) const
{
    return mPointer == o.mPointer;
}

template <typename Friend>
inline bool
ParameterIterator<Friend>::operator!=(const ParameterIterator& o) const
{
    return mPointer != o.mPointer;
}

template <typename Friend>
inline ParameterIterator<Friend>&
ParameterIterator<Friend>::operator+=(DiffType i)
{
    mPointer += i;
    return *this;
}

template <typename Friend>
inline ParameterIterator<Friend>&
ParameterIterator<Friend>::operator-=(DiffType i)
{
    mPointer -= i;
    return *this;
}

template <typename Friend>
inline ParameterIterator<Friend>
ParameterIterator<Friend>::operator+(DiffType i) const
{
    ParameterIterator ret = *this;
    ret += i;
    return ret;
}

template <typename Friend>
inline ParameterIterator<Friend>
ParameterIterator<Friend>::operator-(DiffType i) const
{
    ParameterIterator ret = *this;
    ret -= i;
    return ret;
}

template <typename Friend>
inline ParameterIterator<Friend>&
ParameterIterator<Friend>::operator++()
{
    return (*this) += 1;
}

template <typename Friend>
inline ParameterIterator<Friend>&
ParameterIterator<Friend>::operator--()
{
    return (*this) -= 1;
}

template <typename Friend>
inline ParameterIterator<Friend>
ParameterIterator<Friend>::operator++(int)
{
    ParameterIterator ret = *this;
    (*this) += 1;
    return ret;
}

template <typename Friend>
inline ParameterIterator<Friend>
ParameterIterator<Friend>::operator--(int)
{
    ParameterIterator ret = *this;
    (*this) -= 1;
    return ret;
}

template <typename Friend>
inline typename ParameterIterator<Friend>::DiffType
ParameterIterator<Friend>::operator-(const ParameterIterator& o) const
{
    return this->mPointer - o.mPointer;
}

template <typename Friend>
inline const ParameterBase*
ParameterIterator<Friend>::operator->() const
{
    return *mPointer;
}

template <typename Friend>
inline const ParameterBase&
ParameterIterator<Friend>::operator*() const
{
    return **mPointer;
}

}  // namespace parameter
}  // namespace outpost

#endif
