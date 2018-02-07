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

#ifndef OUTPOST_TRAITS_H
#define OUTPOST_TRAITS_H

#include <stdint.h>

namespace outpost
{
/**
 * Arithmetic Traits.
 *
 * Traits give types more information e.g. the number of bytes needed
 * to represent the given type.
 *
 * \author    Fabian Greif
 */
template <typename T>
struct Traits
{
};

template <>
struct Traits<uint8_t>
{
    typedef int8_t SignedType;
    typedef uint8_t UnsignedType;

    static const int width = 1;
};

template <>
struct Traits<int8_t>
{
    typedef int8_t SignedType;
    typedef uint8_t UnsignedType;

    static const int width = 1;
};

template <>
struct Traits<uint16_t>
{
    typedef int16_t SignedType;
    typedef uint16_t UnsignedType;

    static const int width = 2;
};

template <>
struct Traits<int16_t>
{
    typedef int16_t SignedType;
    typedef uint16_t UnsignedType;

    static const int width = 2;
};

template <>
struct Traits<uint32_t>
{
    typedef int32_t SignedType;
    typedef uint32_t UnsignedType;

    static const int width = 4;
};

template <>
struct Traits<int32_t>
{
    typedef int32_t SignedType;
    typedef uint32_t UnsignedType;

    static const int width = 4;
};

template <>
struct Traits<uint64_t>
{
    typedef int64_t SignedType;
    typedef uint64_t UnsignedType;

    static const int width = 8;
};

template <>
struct Traits<int64_t>
{
    typedef int64_t SignedType;
    typedef uint64_t UnsignedType;

    static const int width = 8;
};
}  // namespace outpost

#endif
