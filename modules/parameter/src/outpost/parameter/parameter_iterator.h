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

#ifndef OUTPOST_PARAMETER_PARAMETER_ITERATOR_H_
#define OUTPOST_PARAMETER_PARAMETER_ITERATOR_H_

#include "parameter.h"

namespace outpost
{
namespace parameter
{
/**
 * A parameter iterator to use for a array of array pointer
 */
template <typename Friend>
class ParameterIterator
{
    friend Friend;

public:
    using DiffType = int;

    ParameterIterator(const ParameterIterator&) = default;
    ParameterIterator&
    operator=(const ParameterIterator&) = default;

    inline bool
    operator==(const ParameterIterator& o) const;
    inline bool
    operator!=(const ParameterIterator& o) const;

    inline ParameterIterator&
    operator+=(DiffType i);
    inline ParameterIterator&
    operator-=(DiffType i);

    inline ParameterIterator
    operator+(DiffType i) const;
    inline ParameterIterator
    operator-(DiffType i) const;

    inline ParameterIterator&
    operator++();
    inline ParameterIterator&
    operator--();
    inline ParameterIterator
    operator++(int);
    inline ParameterIterator
    operator--(int);

    inline DiffType
    operator-(const ParameterIterator& o) const;

    inline const ParameterBase*
    operator->() const;
    inline const ParameterBase&
    operator*() const;

private:
    // For friend
    ParameterIterator() = default;

    const ParameterBase** mPointer = nullptr;
};
}  // namespace parameter
}  // namespace outpost

#include "parameter_iterator_impl.h"

#endif
